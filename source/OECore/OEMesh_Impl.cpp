/*!
 * \file OEMesh_Impl.cpp
 * \date 30-7-2009 16:50:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMesh_Impl.h"
#include "OEPiece_Impl.h"
#include <OEBase/IOEFileMgr.h>
#include <OECore/OEFmtMesh.h>

COEMesh_Impl::COEMesh_Impl(const tstring& strFile)
:IOEMesh(TS("COEMesh_Impl"))
{
	Init();
	m_bOK = CreatePieces(strFile);
}

COEMesh_Impl::~COEMesh_Impl()
{
	Destroy();
}

void COEMesh_Impl::Init()
{
	m_vBoundingBoxMin.Init(OEMATH_FLOAT_MAX, OEMATH_FLOAT_MAX, OEMATH_FLOAT_MAX);
	m_vBoundingBoxMax.Init(OEMATH_FLOAT_MIN, OEMATH_FLOAT_MIN, OEMATH_FLOAT_MIN);
}

void COEMesh_Impl::Destroy()
{
	DestroyPieces();
}

int COEMesh_Impl::GetNumPieces() const
{
	return (int)m_vPiece.size();
}

IOEPiece* COEMesh_Impl::GetPiece(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_vPiece.size()) return NULL;
	return m_vPiece[nIndex];
}

IOEPiece* COEMesh_Impl::FindPiece(const tstring& strName) const
{
	for (TV_PIECE::const_iterator it = m_vPiece.begin(); it != m_vPiece.end(); ++it)
	{
		if ((*it)->GetName() == strName) return (*it);
	}

	return NULL;
}

const CVector3& COEMesh_Impl::GetBoundingBoxMin() const
{
	return m_vBoundingBoxMin;
}

const CVector3& COEMesh_Impl::GetBoundingBoxMax() const
{
	return m_vBoundingBoxMax;
}

bool COEMesh_Impl::CreatePieces(const tstring& strFile)
{
	DestroyPieces();

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile);
	if (!pFile) return false;

	COEFmtMesh::FILE_HEADER Header;
	pFile->Read(&Header, sizeof(Header));

	if (Header.nMagicNumber != COEFmtMesh::MAGIC_NUMBER
		|| Header.nVersion != COEFmtMesh::CURRENT_VERSION)
	{
		SAFE_RELEASE(pFile);
		return false;
	}

	m_vBoundingBoxMin.Init(Header.fBoundingBoxMin[0], Header.fBoundingBoxMin[1], Header.fBoundingBoxMin[2]);
	m_vBoundingBoxMax.Init(Header.fBoundingBoxMax[0], Header.fBoundingBoxMax[1], Header.fBoundingBoxMax[2]);

	// read piece info
	std::vector<COEFmtMesh::PIECE> vPieces;
	if (Header.nNumPieces > 0)
	{
		vPieces.resize(Header.nNumPieces);
		pFile->Read(&vPieces[0], sizeof(COEFmtMesh::PIECE)*Header.nNumPieces);
	}

	// create pieces
	for (int i = 0; i < Header.nNumPieces; ++i)
	{
		COEPiece_Impl* pPiece = new COEPiece_Impl(vPieces[i], pFile);
		if (!pPiece || !pPiece->IsOK())
		{
			SAFE_RELEASE(pFile);
			DestroyPieces();
			return false;
		}
		m_vPiece.push_back(pPiece);
	}

	SAFE_RELEASE(pFile);
	return true;
}

void COEMesh_Impl::DestroyPieces()
{
	for (TV_PIECE::iterator it = m_vPiece.begin(); it != m_vPiece.end(); ++it)
	{
		IOEPiece* pPiece = (*it);
		SAFE_RELEASE(pPiece);
	}
	m_vPiece.clear();
}
