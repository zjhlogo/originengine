/*!
 * \file StringWindow.cpp
 * \date 10-8-2010 9:52:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "StringWindow.h"
#include <OEUI/IOEUIFontMgr.h>
#include <OEUI/IOEUIStringMgr.h>

CStringWindow::CStringWindow(COEUIWindow* pParent)
:COEUIWindow(pParent)
{
	m_pFont = NULL;
	m_pString = NULL;

	m_bOK = Init();
}

CStringWindow::~CStringWindow()
{
	// TODO: 
}

bool CStringWindow::Init()
{
	m_pFont = g_pOEUIFontMgr->CreateBitmapFont(TS("12px_Tahoma.fnt"));
	if (!m_pFont) return false;

	m_pString = g_pOEUIStringMgr->CreateUIString(m_pFont);
	if (!m_pString) return false;

	m_pString->SetText(TS("HANDLE g_hTickEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);"));
	m_pString->SetPosition(CPoint(100.0f, 100.0f));
	return true;
}

void CStringWindow::Destroy()
{
	SAFE_RELEASE(m_pString);
	SAFE_RELEASE(m_pFont);
}

void CStringWindow::UpdateSelf(float fDetailTime)
{
	// TODO: 
}

void CStringWindow::RenderSelf(float fDetailTime)
{
	m_pString->Render(fDetailTime);
}
