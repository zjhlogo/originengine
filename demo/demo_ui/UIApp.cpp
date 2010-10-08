/*!
 * \file UIApp.cpp
 * \date 27-7-2009 15:38:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "UIApp.h"
#include "../common/AppHelper.h"
#include <OEUI/IOEUIRenderSystem.h>
#include "StringWindow.h"

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
	// TODO: 
}

void CUIApp::Destroy()
{
	// TODO: 
}

bool CUIApp::Initialize()
{
	if (!CBaseApp::Initialize()) return false;

	CStringWindow* pStringWindow = new CStringWindow(g_pOEUIRenderSystem->GetScreen());

	return true;
}

void CUIApp::Terminate()
{
	CBaseApp::Terminate();
}
