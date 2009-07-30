/*!
 * \file OEMeshMgr_Impl.cpp
 * \date 30-7-2009 10:20:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMeshMgr_Impl.h"

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
	// TODO: 
	return NULL;
}
