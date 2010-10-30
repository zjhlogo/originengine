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
#include "OERenderData_Impl.h"
#include "OEAnimData_Impl.h"
#include "OEMaterialsList_Impl.h"

#include <OEBase/IOELogFileMgr.h>
#include <OECore/IOEConfigFileMgr.h>
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
	// TODO: 
}

bool COEResMgr_Impl::Initialize()
{
	tstring strDefaultMediaDir;
	g_pOEConfigFileMgr->GetValue(strDefaultMediaDir, TS("DefaultMediaDir"), TS("media"));
	SetDefaultDir(strDefaultMediaDir);

	return true;
}

void COEResMgr_Impl::Terminate()
{
	// TODO: check m_MeshMap/m_BonesMap whether is empty, and logout
}

IOERenderData* COEResMgr_Impl::CreateRenderData()
{
	COERenderData_Impl* pRenderData = new COERenderData_Impl();
	if (!pRenderData || !pRenderData->IsOK())
	{
		LOGOUT(TS("IOEModelMgr::CreateRenderData Failed"));
		SAFE_DELETE(pRenderData);
		return NULL;
	}

	return pRenderData;
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

IOEAnimData* COEResMgr_Impl::CreateAnimData(const tstring& strFile)
{
	COEAnimData_Impl* pAnimData = new COEAnimData_Impl(strFile);
	if (!pAnimData || !pAnimData->IsOK())
	{
		LOGOUT(TS("IOEModelMgr::CreateAnimData Failed"));
		SAFE_DELETE(pAnimData);
		return NULL;
	}

	return pAnimData;
}

IOEMaterialsList* COEResMgr_Impl::CreateMaterialsList(IOEXmlNode* pXmlMaterials)
{
	COEMaterialsList_Impl* pMaterialsList = new COEMaterialsList_Impl(pXmlMaterials);
	if (!pMaterialsList || !pMaterialsList->IsOK())
	{
		LOGOUT(TS("IOEModelMgr::CreateMaterialsList Failed"));
		SAFE_DELETE(pMaterialsList);
		return NULL;
	}

	return pMaterialsList;
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
