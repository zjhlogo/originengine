/*!
 * \file UIApp.cpp
 * \date 27-7-2009 15:38:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "UIApp.h"
#include "../common/AppHelper.h"

#include <OEUI/IOEUIFontMgr.h>
#include <OEUI/IOEUIStringMgr.h>

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
	m_pFont = NULL;
	m_pString = NULL;
	m_nPerformCount = 0;
}

void CUIApp::Destroy()
{
	// TODO: 
}

bool CUIApp::Initialize()
{
	if (!CBaseApp::Initialize()) return false;

	m_pFont = g_pOEUIFontMgr->CreateBitmapFont(TS("12px_Tahoma.fnt"));
	if (!m_pFont) return false;

	m_pString = g_pOEUIStringMgr->CreateUIString(m_pFont);
	if (!m_pString) return false;

	m_pString->SetText(TS("HANDLE g_hTickEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);"));

	return true;
}

void CUIApp::Terminate()
{
	SAFE_RELEASE(m_pString);
	SAFE_RELEASE(m_pFont);
	CBaseApp::Terminate();
}

void CUIApp::Render(float fDetailTime)
{
	m_pString->Render(CPoint(100.0f, 100.0f));
}
