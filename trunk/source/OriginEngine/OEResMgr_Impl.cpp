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
#include "OESkeleton_Impl.h"
#include "OEMaterial_Impl.h"

#include <IOELogFileMgr.h>
#include <assert.h>

COEResMgr_Impl::COEResMgr_Impl()
{
	assert(!g_pOEResMgr);
	g_pOEResMgr = this;
	m_bOK = Init();
}

COEResMgr_Impl::~COEResMgr_Impl()
{
	Destroy();
	g_pOEResMgr = NULL;
}

bool COEResMgr_Impl::Init()
{
	// TODO: 
	return true;
}

void COEResMgr_Impl::Destroy()
{
	// TODO: check m_MeshMap/m_BonesMap whether is empty, and logout
}

IOEModel* COEResMgr_Impl::CreateModel(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!GetMediaFilePath(strFilePath, strFile)) return NULL;

	COEModel_Impl* pModel = new COEModel_Impl(strFilePath);
	if (!pModel || !pModel->IsOK())
	{
		LOGOUT(TS("IOEModelMgr::CreateModel Failed %s"), strFilePath.c_str());
		SAFE_DELETE(pModel);
		return NULL;
	}

	return pModel;
}

IOEMesh* COEResMgr_Impl::CreateMesh(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!GetMediaFilePath(strFilePath, strFile)) return NULL;

	// check whether the mesh created
	TM_MESH::iterator itfound = m_MeshMap.find(strFilePath);
	if (itfound != m_MeshMap.end())
	{
		IOEMesh* pMesh = itfound->second;
		pMesh->IncRef();
		return pMesh;
	}

	// create new
	COEMesh_Impl* pMesh = new COEMesh_Impl(strFilePath);
	if (!pMesh || !pMesh->IsOK())
	{
		LOGOUT(TS("IOEMeshMgr::CreateMesh Failed %s"), strFilePath.c_str());
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	m_MeshMap.insert(std::make_pair(strFilePath, pMesh));
	return pMesh;
}

IOESkeleton* COEResMgr_Impl::CreateSkeleton(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!GetMediaFilePath(strFilePath, strFile)) return NULL;

	// check whether the bones created
	TM_BONES::iterator itfound = m_BonesMap.find(strFilePath);
	if (itfound != m_BonesMap.end())
	{
		IOESkeleton* pBones = itfound->second;
		pBones->IncRef();
		return pBones;
	}

	// create new
	COESkeleton_Impl* pBones = new COESkeleton_Impl(strFilePath);
	if (!pBones || !pBones->IsOK())
	{
		LOGOUT(TS("IOEMeshMgr::CreateMesh Failed %s"), strFilePath.c_str());
		SAFE_RELEASE(pBones);
		return NULL;
	}

	m_BonesMap.insert(std::make_pair(strFilePath, pBones));
	return pBones;
}


IOEMaterial* COEResMgr_Impl::CreateMaterial(IOEXmlNode* pXmlMaterial)
{
	COEMaterial_Impl* pMaterial = new COEMaterial_Impl(pXmlMaterial);
	if (!pMaterial || !pMaterial->IsOK())
	{
		LOGOUT(TS("IOEModelMgr::CreateMaterial Failed"));
		SAFE_DELETE(pMaterial);
		return NULL;
	}

	return pMaterial;
}

void COEResMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& COEResMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

bool COEResMgr_Impl::GetMediaFilePath(tstring& strFilePathOut, const tstring& strFile)
{
	strFilePathOut = m_strDefaultDir + TS("\\") + strFile;
	COEOS::tolower(strFilePathOut, strFilePathOut);
	return true;
}

