/*!
 * \file OEFPSPrinter.cpp
 * \date 10-4-2010 23:33:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEFPSPrinter.h"
#include <OECOre/IOEDevice.h>
#include <libOEBase/OEOS.h>

COEFPSPrinter::COEFPSPrinter()
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

COEFPSPrinter::~COEFPSPrinter()
{
	SAFE_RELEASE(m_pStringFPS);
	SAFE_RELEASE(m_pFontFPS);
}

void COEFPSPrinter::Update(float fDetailTime)
{
	// calculate fps
	CalculateFPS();
}

void COEFPSPrinter::Render(float fDetailTime)
{
	// render fps
	m_pStringFPS->Render(OEUI_ZERO_POINT);
}

void COEFPSPrinter::CalculateFPS()
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

