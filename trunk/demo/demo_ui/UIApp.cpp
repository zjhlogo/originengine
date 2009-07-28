/*!
 * \file UIApp.cpp
 * \date 27-7-2009 15:38:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "UIApp.h"
#include <OEInterfaces.h>
#include <OEOS.h>

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
	m_pFont = NULL;
	m_pString = NULL;
}

void CUIApp::Destroy()
{
	// TODO: 
}

bool CUIApp::Initialize()
{
	m_pFont = g_pOEUIFontMgr->CreateBitmapFont(_T("12px_Tahoma.fnt"));
	if (!m_pFont) return false;

	m_pString = g_pOEUIStringMgr->CreateUIString(m_pFont);
	if (!m_pString) return false;

	g_pOERenderer->SetSampleFilter(IOERenderer::SF_POINT);

	return true;
}

void CUIApp::Terminate()
{
	SAFE_RELEASE(m_pString);
	SAFE_RELEASE(m_pFont);
}

void CUIApp::Update(float fDetailTime)
{
	//static float s_fTotalTime = 0.0f;
	//s_fTotalTime += fDetailTime;

	//if (s_fTotalTime > 1.0f)
	//{
	//	tstring strFPS;
	//	float fFPS = g_pOEDevice->GetFPS();

	//	COEOS::strformat(strFPS, _T("%0.2f FPS"), fFPS);
	//	m_pString->SetText(strFPS);
	//	s_fTotalTime -= 1.0f;
	//}
}

void CUIApp::Render(float fDetailTime)
{
	//CPoint pos(0, 0);
	//if (m_pString) m_pString->Render(pos);
}
