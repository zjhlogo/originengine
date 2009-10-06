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
	m_pFont = g_pOEUIFontMgr->CreateBitmapFont(t("12px_Tahoma.fnt"));
	if (!m_pFont) return false;

	m_pString = g_pOEUIStringMgr->CreateUIString(m_pFont);
	if (!m_pString) return false;

	m_pString->SetText(t("HANDLE g_hTickEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);"));

	return true;
}

void CUIApp::Terminate()
{
	SAFE_RELEASE(m_pString);
	SAFE_RELEASE(m_pFont);
}

void CUIApp::Update(float fDetailTime)
{
	// TODO: 
}

void CUIApp::Render(float fDetailTime)
{
	if (m_pString) m_pString->Render(CPoint(100.0f, 100.0f));
}
