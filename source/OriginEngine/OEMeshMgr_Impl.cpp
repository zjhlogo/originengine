/*!
 * \file OEMeshMgr_Impl.cpp
 * \date 30-7-2009 10:20:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMeshMgr_Impl.h"
#include "OEMesh_Impl.h"
#include <OEInterfaces.h>

COEMeshMgr_Impl::COEMeshMgr_Impl()
{
	g_pOEMeshMgr = this;
	Init();
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
	// TODO: 
}

IOEMesh* COEMeshMgr_Impl::CreateMeshFromFile(const tstring& strFileName)
{
	// TODO: check whether the mesh created
	COEMesh_Impl* pMesh = new COEMesh_Impl(strFileName);
	if (!pMesh || !pMesh->IsOK())
	{
		LOGOUT(_T("IOEMeshMgr::CreateMeshFromFile Failed"));
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	return pMesh;
}
