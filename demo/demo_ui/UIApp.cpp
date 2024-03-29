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
}

void CUIApp::Destroy()
{
	// TODO: 
}

bool CUIApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("casual03.xml"));
	if (!m_pModel) return false;

	IOENode* pRootNode = g_pOECore->GetRootNode();
	pRootNode->AttachObject(m_pModel);

	IOENode* pLightNode = pRootNode->GetChildNode(TS("Light"));
	pLightNode->SetPosition(CVector3(0.0f, 0.0f, -300.0f));

	CUITestWindow* pTestWindow = new CUITestWindow(g_pOEUIRenderSystem->GetScreen());
	if (!pTestWindow) return false;

	ResetCameraPosRot(m_pModel);
	return true;
}

void CUIApp::UserDataTerm()
{
	SAFE_RELEASE(m_pModel);
}

void CUIApp::Update(float fDetailTime)
{
	// TODO: 
}
