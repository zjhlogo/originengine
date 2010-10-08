/*!
 * \file FPSWindow.cpp
 * \date 10-8-2010 9:40:34
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "FPSWindow.h"
#include <OECOre/IOEDevice.h>
#include <libOEBase/OEOS.h>

CFPSWindow::CFPSWindow(COEUIWindow* pParent)
:COEUIWindow(pParent)
{
	m_fCurrFPS = 0.0f;
	m_fLastFPSTime = 0.0f;
	m_nFPSCount = 0;
	m_pFontFPS = NULL;
	m_pStringFPS = NULL;

	// initialize fps string
	m_pFontFPS = g_pOEUIFontMgr->CreateBitmapFont(TS("12px_Tahoma.fnt"));
	m_pStringFPS = g_pOEUIStringMgr->CreateUIString(m_pFontFPS);

	m_fLastFPSTime = g_pOEDevice->GetCurrTime();
	m_nFPSCount = 0;
}

CFPSWindow::~CFPSWindow()
{
	SAFE_RELEASE(m_pStringFPS);
	SAFE_RELEASE(m_pFontFPS);
}

void CFPSWindow::UpdateSelf(float fDetailTime)
{
	// calculate fps
	CalculateFPS();
}

void CFPSWindow::RenderSelf(float fDetailTime)
{
	// render fps
	m_pStringFPS->Render(fDetailTime);
}

void CFPSWindow::CalculateFPS()
{
	++m_nFPSCount;

	float fCurrTime = g_pOEDevice->GetCurrTime();
	float fDetailFPS = fCurrTime - m_fLastFPSTime;

	if (fDetailFPS > 1.0f)
	{
		m_fCurrFPS = m_nFPSCount/fDetailFPS;
		m_nFPSCount = 0;
		m_fLastFPSTime = fCurrTime;

		tstring strText;
		COEOS::strformat(strText, TS("%.2f FPS"), m_fCurrFPS);
		m_pStringFPS->SetText(strText);
	}
}
