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
	// TODO: 
}

void COEMesh_Impl::Destroy()
{
	for (VMESH_PIECE::iterator it = m_vPiece.begin(); it != m_vPiece.end(); ++it)
	{
		IOEMeshPiece* pMeshPiece = (*it);
		SAFE_RELEASE(pMeshPiece);
	}
	m_vPiece.clear();

	for (VMESH_BONE::iterator it = m_vBone.begin(); it != m_vBone.end(); ++it)
	{
		IOEMeshBone* pMeshBone = (*it);
		SAFE_RELEASE(pMeshBone);
	}
	m_vBone.clear();
}

int COEMesh_Impl::GetNumPieces() const
{
	return (int)m_vPiece.size();
}

IOEMeshPiece* COEMesh_Impl::GetPiece(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_vPiece.size()) return NULL;
	return m_vPiece[nIndex];
}

IOEMeshPiece* COEMesh_Impl::FindPiece(const tstring& strName) const
{
	for (VMESH_PIECE::const_iterator it = m_vPiece.begin(); it != m_vPiece.end(); ++it)
	{
		if ((*it)->GetName() == strName) return (*it);
	}

	return NULL;
}

int COEMesh_Impl::GetNumBones() const
{
	return (int)m_vBone.size();
}

IOEMeshBone* COEMesh_Impl::GetBone(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_vBone.size()) return NULL;
	return m_vBone[nIndex];
}

IOEMeshBone* COEMesh_Impl::FindBone(const tstring& strName) const
{
	for (VMESH_BONE::const_iterator it = m_vBone.begin(); it != m_vBone.end(); ++it)
	{
		if ((*it)->GetName() == strName) return (*it);
	}

	return NULL;
}

bool COEMesh_Impl::Create(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName);
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
		COEMeshPiece_Impl* pMeshPiece = new COEMeshPiece_Impl(vPieces[i], pFile);
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
		COEMeshBone_Impl* pMeshBone = new COEMeshBone_Impl(vBones[i], i, pFile);
		if (!pMeshBone || !pMeshBone->IsOK())
		{
			SAFE_RELEASE(pMeshBone);
			return false;
		}
		m_vBone.push_back(pMeshBone);
	}

	// create parent/child relation
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		int nParentID = m_vBone[i]->GetParentID();
		if (nParentID == COEFmtMesh::INVALID_BONE_ID) continue;

		COEMeshBone_Impl* pParent = NULL;
		if (nParentID >= 0 && nParentID < Header.nNumBones) pParent = m_vBone[nParentID];

		m_vBone[i]->SetParent(pParent);
	}

	return true;
}

IOEMeshBone* COEMesh_Impl::GetRootBone() const
{
	if (m_vBone.empty()) return NULL;
	return m_vBone[0];
}