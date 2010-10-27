/*!
 * \file UIApp.cpp
 * \date 27-7-2009 15:38:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "UIApp.h"
#include "../common/AppHelper.h"
#include "UITestWindow.h"
#include <OECore/IOECore.h>
#include <OECore/IOEResMgr.h>
#include <OECore/IOETextureMgr.h>
#include <OECore/IOERenderSystem.h>
#include <libOEMsg/OEMsgList.h>

#include <OEUI/IOEUIRenderSystem.h>

IMPLEMENT_OEAPP(CUIApp);

CUIApp::CUIApp()
{
	Init();
}

CUIApp::~CUIApp()
{
	Destroy();
}

void CUIApp::Init()
{
	m_pModel = NULL;
	m_pRenderTargetTexture = NULL;
}

void CUIApp::Destroy()
{
	// TODO: 
}

bool CUIApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("casual03.xml"));
	if (!m_pModel) return false;

	g_pOECore->GetRootNode()->AttachObject(m_pModel);
	ResetCameraPosRot(m_pModel);

	CUITestWindow* pTestWindow = new CUITestWindow(g_pOEUIRenderSystem->GetScreen());
	if (!pTestWindow) return false;

	m_pRenderTargetTexture = g_pOETextureMgr->CreateRenderTargetTexture(200, 150, TF_A8R8G8B8);
	if (!m_pRenderTargetTexture) return false;

	m_pModel->RegisterEvent(OMI_PRE_RENDER, this, (MSG_FUNC)&CUIApp::OnPreRender);
	m_pModel->RegisterEvent(OMI_POST_RENDER, this, (MSG_FUNC)&CUIApp::OnPostRender);

	pTestWindow->SetRenderTargetTexture(m_pRenderTargetTexture);

	ResetCameraPosRot(m_pModel);
	return true;
}

void CUIApp::UserDataTerm()
{
	SAFE_RELEASE(m_pRenderTargetTexture);
	SAFE_RELEASE(m_pModel);
}

void CUIApp::Update(float fDetailTime)
{
	// TODO: 
}

bool CUIApp::OnPreRender(COEMsgCommand& msg)
{
	//g_pOERenderSystem->BeginRenderTarget(m_pRenderTargetTexture);
	return true;
}

bool CUIApp::OnPostRender(COEMsgCommand& msg)
{
	//g_pOERenderSystem->EndRenderTarget();
	g_pOERenderSystem->CopyBackbuffer(m_pRenderTargetTexture);
	return true;
}
