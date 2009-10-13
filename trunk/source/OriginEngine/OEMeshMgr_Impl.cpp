/*!
 * \file OEMeshMgr_Impl.cpp
 * \date 30-7-2009 10:20:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMeshMgr_Impl.h"
#include "OEMesh_Impl.h"

#include <IOELogFileMgr.h>
#include <OEOS.h>

COEMeshMgr_Impl::COEMeshMgr_Impl()
{
	g_pOEMeshMgr = this;
	Init();
	m_bOK = true;
}

COEMeshMgr_Impl::~COEMeshMgr_Impl()
{
	Destroy();
	g_pOEMeshMgr = NULL;
}

void COEMeshMgr_Impl::Init()
{
	// TODO: 
}

void COEMeshMgr_Impl::Destroy()
{
	// TODO: check m_MeshMap whether is empty, and logout
}

IOEMesh* COEMeshMgr_Impl::CreateMeshFromFile(const tstring& strFileName)
{
	// transform string to lower
	tstring strLowName;
	COEOS::tolower(strLowName, strFileName);

	// check whether the mesh created
	MESH_MAP::iterator itfound = m_MeshMap.find(strLowName);
	if (itfound != m_MeshMap.end())
	{
		IOEMesh* pMesh = itfound->second;
		pMesh->IncRef();
		return pMesh;
	}

	// create new
	COEMesh_Impl* pMesh = new COEMesh_Impl(strFileName);
	if (!pMesh || !pMesh->IsOK())
	{
		LOGOUT(t("IOEMeshMgr::CreateMeshFromFile Failed"));
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	m_MeshMap.insert(std::make_pair(strLowName, pMesh));

	return pMesh;
}
