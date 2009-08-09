/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"

COEModel_Impl::COEModel_Impl(const tstring& strFileName)
{
	Init();
	m_bOK = Create(strFileName);
}

COEModel_Impl::~COEModel_Impl()
{
	Destroy();
}

void COEModel_Impl::Init()
{
	m_pMesh = NULL;
}

void COEModel_Impl::Destroy()
{
	SAFE_RELEASE(m_pMesh);
}

void COEModel_Impl::Update(float fDetailTime)
{
	// TODO: 
}

void COEModel_Impl::Render(float fDetailTime)
{
	// TODO: 
}

bool COEModel_Impl::Create(const tstring& strFileName)
{
	m_pMesh = g_pOEMeshMgr->CreateMeshFromFile(strFileName);
	if (!m_pMesh) return false;

	return true;
}
