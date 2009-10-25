/*!
 * \file OEMesh_Impl.cpp
 * \date 30-7-2009 16:50:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMesh_Impl.h"

#include <OEFmtMesh.h>
#include <IOEFileMgr.h>

COEMesh_Impl::COEMesh_Impl(const tstring& strFileName)
{
	Init();
	m_bOK = Create(strFileName);
}

COEMesh_Impl::~COEMesh_Impl()
{
	Destroy();
}

void COEMesh_Impl::Init()
{
	m_fTimeLength = 0.0f;
}

void COEMesh_Impl::Destroy()
{
	for (TV_PIECE::iterator it = m_vPiece.begin(); it != m_vPiece.end(); ++it)
	{
		IOEPiece* pMeshPiece = (*it);
		SAFE_RELEASE(pMeshPiece);
	}
	m_vPiece.clear();

	for (TV_BONE::iterator it = m_vBone.begin(); it != m_vBone.end(); ++it)
	{
		IOEBone* pMeshBone = (*it);
		SAFE_RELEASE(pMeshBone);
	}
	m_vBone.clear();
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

int COEMesh_Impl::GetNumBones() const
{
	return (int)m_vBone.size();
}

IOEBone* COEMesh_Impl::GetBone(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_vBone.size()) return NULL;
	return m_vBone[nIndex];
}

IOEBone* COEMesh_Impl::FindBone(const tstring& strName) const
{
	for (TV_BONE::const_iterator it = m_vBone.begin(); it != m_vBone.end(); ++it)
	{
		if ((*it)->GetName() == strName) return (*it);
	}

	return NULL;
}

bool COEMesh_Impl::Create(const tstring& strFile)
{
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

	// read piece info
	std::vector<COEFmtMesh::PIECE> vPieces;
	if (Header.nNumPieces > 0)
	{
		vPieces.resize(Header.nNumPieces);
		pFile->Read(&vPieces[0], sizeof(COEFmtMesh::PIECE)*Header.nNumPieces);
	}

	// read bone info
	std::vector<COEFmtMesh::BONE> vBones;
	if (Header.nNumBones > 0)
	{
		vBones.resize(Header.nNumBones);
		pFile->Read(&vBones[0], sizeof(COEFmtMesh::BONE)*Header.nNumBones);
	}

	// create pieces
	for (int i = 0; i < Header.nNumPieces; ++i)
	{
		COEPiece_Impl* pMeshPiece = new COEPiece_Impl(vPieces[i], pFile);
		if (!pMeshPiece || !pMeshPiece->IsOK())
		{
			SAFE_RELEASE(pMeshPiece);
			return false;
		}
		m_vPiece.push_back(pMeshPiece);
	}

	// create bones
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		COEBone_Impl* pMeshBone = new COEBone_Impl(vBones[i], i, pFile);
		if (!pMeshBone || !pMeshBone->IsOK())
		{
			SAFE_RELEASE(pMeshBone);
			return false;
		}
		m_vBone.push_back(pMeshBone);

		if (m_fTimeLength < pMeshBone->GetTimeLength()) m_fTimeLength = pMeshBone->GetTimeLength();
	}

	// build bone matrix
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		int nParentID = m_vBone[i]->GetParentID();
		if (nParentID != COEFmtMesh::INVALID_BONE_ID)
		{
			m_vBone[i]->SetWorldMatrix(m_vBone[i]->GetLocalMatrix() * m_vBone[nParentID]->GetWorldMatrix());
		}
		else
		{
			m_vBone[i]->SetWorldMatrix(m_vBone[i]->GetLocalMatrix());
		}
	}

	return true;
}

IOEBone* COEMesh_Impl::GetRootBone() const
{
	if (m_vBone.empty()) return NULL;
	return m_vBone[0];
}

float COEMesh_Impl::GetTimeLength() const
{
	return m_fTimeLength;
}
