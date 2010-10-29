/*!
 * \file MeshApp.cpp
 * \date 31-7-2009 20:46:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "SkyApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/IOEResMgr.h>
#include <libOEMsg/OEMsgList.h>

IMPLEMENT_OEAPP(CSkyApp);

CSkyApp::CSkyApp()
{
	Init();
}

CSkyApp::~CSkyApp()
{
	Destroy();
}

void CSkyApp::Init()
{
	m_pModel = NULL;
}

void CSkyApp::Destroy()
{
	// TODO: 
}

bool CSkyApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("skybox.xml"));
	if (!m_pModel) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pModel);

	return true;
}

void CSkyApp::UserDataTerm()
{
	SAFE_RELEASE(m_pModel);
}

void CSkyApp::Update(float fDetailTime)
{
	// TODO: 
}
