/*!
 * \file OEResMgr_Impl.cpp
 * \date 13-2-2010 9:42:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEResMgr_Impl.h"
#include "OEModel_Impl.h"
#include "OEMesh_Impl.h"
#include "OEPiece_Impl.h"
#include "OEBone_Impl.h"

#include <IOELogFileMgr.h>
#include <IOEFileMgr.h>
#include <OEFmtMesh.h>
#include <OEFmtBone.h>

COEResMgr_Impl::COEResMgr_Impl()
{
	g_pOEResMgr = this;
	Init();
}

COEResMgr_Impl::~COEResMgr_Impl()
{
	Destroy();
	g_pOEResMgr = NULL;
}

void COEResMgr_Impl::Init()
{
	m_bOK = true;
}

void COEResMgr_Impl::Destroy()
{
	// TODO: check m_MeshMap whether is empty, and logout
}

IOEModel* COEResMgr_Impl::CreateModel(const tstring& strFile)
{
	// transform string to lower
	tstring strLowName;
	COEOS::tolower(strLowName, strFile);

	COEModel_Impl* pModel = new COEModel_Impl(strLowName);
	if (!pModel || !pModel->IsOK())
	{
		LOGOUT(TS("IOEModelMgr::CreateModel Failed"));
		SAFE_DELETE(pModel);
		return NULL;
	}

	return pModel;
}

IOEMesh* COEResMgr_Impl::CreateMesh(const tstring& strFile)
{
	// transform string to lower
	tstring strLowName;
	COEOS::tolower(strLowName, strFile);

	// check whether the mesh created
	MESH_MAP::iterator itfound = m_MeshMap.find(strLowName);
	if (itfound != m_MeshMap.end())
	{
		IOEMesh* pMesh = itfound->second;
		pMesh->IncRef();
		return pMesh;
	}

	// create new
	COEMesh_Impl* pMesh = new COEMesh_Impl(strLowName);
	if (!pMesh || !pMesh->IsOK())
	{
		LOGOUT(TS("IOEMeshMgr::CreateMesh Failed"));
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	m_MeshMap.insert(std::make_pair(strLowName, pMesh));

	return pMesh;
}

bool COEResMgr_Impl::CreatePieces(TV_PIECE& vPiecesOut, const tstring& strFile)
{
	// transform string to lower
	tstring strLowName;
	COEOS::tolower(strLowName, strFile);

	vPiecesOut.clear();

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strLowName);
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

	// create pieces
	for (int i = 0; i < Header.nNumPieces; ++i)
	{
		COEPiece_Impl* pPiece = new COEPiece_Impl(vPieces[i], pFile);
		if (!pPiece || !pPiece->IsOK())
		{
			SAFE_RELEASE(pFile);
			DestroyPieces(vPiecesOut);
			return false;
		}
		vPiecesOut.push_back(pPiece);
	}

	SAFE_RELEASE(pFile);
	return true;
}

void COEResMgr_Impl::DestroyPieces(TV_PIECE& vPieces)
{
	for (TV_PIECE::iterator it = vPieces.begin(); it != vPieces.end(); ++it)
	{
		IOEPiece* pPiece = (*it);
		SAFE_RELEASE(pPiece);
	}
	vPieces.clear();
}

bool COEResMgr_Impl::CreateBones(TV_BONE& vBonesOut, const tstring& strFile)
{
	// transform string to lower
	tstring strLowName;
	COEOS::tolower(strLowName, strFile);

	vBonesOut.clear();

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strLowName);
	if (!pFile) return false;

	COEFmtBone::FILE_HEADER Header;
	pFile->Read(&Header, sizeof(Header));

	if (Header.nMagicNumber != COEFmtBone::MAGIC_NUMBER
		|| Header.nVersion != COEFmtBone::CURRENT_VERSION)
	{
		SAFE_RELEASE(pFile);
		return false;
	}

	// read bone info
	std::vector<COEFmtBone::BONE> vBones;
	if (Header.nNumBones > 0)
	{
		vBones.resize(Header.nNumBones);
		pFile->Read(&vBones[0], sizeof(COEFmtBone::BONE)*Header.nNumBones);
	}

	// create bones
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		COEBone_Impl* pBone = new COEBone_Impl(vBones[i], i, pFile);
		if (!pBone || !pBone->IsOK())
		{
			SAFE_RELEASE(pFile);
			DestroyBones(vBonesOut);
			return false;
		}
		vBonesOut.push_back(pBone);
	}

	SAFE_RELEASE(pFile);

	// build bone matrix
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		int nParentID = vBonesOut[i]->GetParentID();
		if (nParentID != COEFmtBone::INVALID_BONE_ID)
		{
			COEBone_Impl* pCurrBone = (COEBone_Impl*)vBonesOut[i];
			COEBone_Impl* pParentBone = (COEBone_Impl*)vBonesOut[nParentID];

			pCurrBone->SetWorldMatrix(pCurrBone->GetLocalMatrix() * pParentBone->GetWorldMatrix());
		}
		else
		{
			COEBone_Impl* pCurrBone = (COEBone_Impl*)vBonesOut[i];
			pCurrBone->SetWorldMatrix(pCurrBone->GetLocalMatrix());
		}
	}

	return true;
}

void COEResMgr_Impl::DestroyBones(TV_BONE& vBones)
{
	for (TV_BONE::iterator it = vBones.begin(); it != vBones.end(); ++it)
	{
		SAFE_RELEASE(*it);
	}
	vBones.clear();
}
