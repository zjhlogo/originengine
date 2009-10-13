/*!
 * \file UIApp.cpp
 * \date 27-7-2009 15:38:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "UIApp.h"
#include <OEMessageID.h>
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
	m_nPerformCount = 0;
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

	g_pOEMessageMgr->RegisterMessage(OMI_PERFORM_ONCE, this, (FUNC_MESSAGE)&CUIApp::OnMessage);

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

bool CUIApp::OnMessage(uint nMsgID, COEDataBufferRead* pDBRead)
{
	switch (nMsgID)
	{
	case OMI_PERFORM_ONCE:
		{
			COEMessage msg(pDBRead);
			float fCurrFPS = 0.0f;
			msg.Read(fCurrFPS);

			++m_nPerformCount;
			if (m_pString)
			{
				tstring strTemp;
				COEOS::strformat(strTemp, t("Perform Count: %d, FPS: %f"), m_nPerformCount, fCurrFPS);
				m_pString->SetText(strTemp);
			}
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return true;
}
