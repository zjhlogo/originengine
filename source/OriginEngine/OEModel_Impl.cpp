/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"
#include <IOEMeshMgr.h>
#include <IOEXmlMgr.h>
#include <IOEShaderMgr.h>
#include <IOETextureMgr.h>
#include <IOELogFileMgr.h>
#include <IOERenderSystem.h>
#include <IOEFileMgr.h>

COEModel_Impl::COEModel_Impl(const tstring& strFile)
{
	Init();
	m_bOK = Create(strFile);
}

COEModel_Impl::~COEModel_Impl()
{
	Destroy();
}

void COEModel_Impl::Init()
{
	m_pRenderData = NULL;
	m_pRender = NULL;
}

void COEModel_Impl::Destroy()
{
	// TODO: release data
}

bool COEModel_Impl::Create(const tstring& strFile)
{
	// TODO: create render data
	// TODO: create control
	// TODO: create render
	return true;
}

void COEModel_Impl::Update(float fDetailTime)
{
	// TODO: 
}

void COEModel_Impl::Render()
{
	// TODO: 
}
