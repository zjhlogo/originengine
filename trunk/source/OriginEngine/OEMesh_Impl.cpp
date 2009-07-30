/*!
 * \file OEMesh_Impl.cpp
 * \date 30-7-2009 16:50:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMesh_Impl.h"
#include <OEInterfaces.h>

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
	// TODO: 
}

void COEMesh_Impl::Render(float fDetailTime)
{
	// TODO: 
}

bool COEMesh_Impl::Create(const tstring& strFileName)
{
	// TODO:
	return false;
}
