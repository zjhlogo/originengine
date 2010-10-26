/*!
 * \file UITestWindow.cpp
 * \date 10-26-2010 0:48:48
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UITestWindow.h"
#include <OEUI/IOEUIResMgr.h>
#include <OEUI/IOEUIRendererMgr.h>
#include <OECore/IOETextureMgr.h>

CUITestWindow::CUITestWindow(COEUIWindow* pParent)
:COEUIWindow(pParent)
{
	m_pFont = NULL;
	m_pString = NULL;
	m_pTexture = NULL;
	m_pImage = NULL;

	m_bOK = Init();
}

CUITestWindow::~CUITestWindow()
{
	// TODO: 
}

bool CUITestWindow::Init()
{
	m_pFont = g_pOEUIResMgr->CreateBitmapFont(TS("12px_Tahoma.fnt"));
	if (!m_pFont) return false;

	m_pString = g_pOEUIRendererMgr->CreateStringRenderer(m_pFont);
	if (!m_pString) return false;

	m_pTexture = g_pOETextureMgr->CreateTextureFromFile(TS("brick_diffuse.png"));
	if (!m_pTexture) return false;

	m_pImage = g_pOEUIRendererMgr->CreateImageRenderer();
	if (!m_pImage) return false;

	m_pString->SetText(TS("HANDLE g_hTickEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);"));
	m_pString->SetPosition(CPoint(550.0f, 100.0f));

	m_pImage->SetTexture(m_pTexture, true);
	m_pImage->SetPosition(CPoint(10.0f, 10.0f));
	return true;
}

void CUITestWindow::Destroy()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pString);
	SAFE_RELEASE(m_pFont);
}

void CUITestWindow::Update(float fDetailTime)
{
	// TODO: 
}

void CUITestWindow::Render(float fDetailTime)
{
	m_pString->Render(fDetailTime);
	m_pImage->Render(fDetailTime);
}
