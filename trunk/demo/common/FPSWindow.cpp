/*!
 * \file FPSWindow.cpp
 * \date 10-8-2010 9:40:34
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "FPSWindow.h"
#include <OECOre/IOEDevice.h>
#include <OECore/IOECore.h>
#include <libOEBase/OEOS.h>

CFPSWindow::CFPSWindow(COEUIWindow* pParent)
:COEUIWindow(pParent)
{
	m_fCurrFPS = 0.0f;
	m_fLastFPSTime = 0.0f;
	m_nFPSCount = 0;
	m_pFont = NULL;
	m_pStringFPS = NULL;
	m_bShowFPS = true;
	m_bShowCameraPosRot = false;

	// initialize fps string
	m_pFont = g_pOEUIResMgr->CreateBitmapFont(TS("12px_Tahoma.fnt"));

	m_pStringFPS = g_pOEUIRendererMgr->CreateStringRenderer(m_pFont);
	m_pStringFPS->SetPosition(CPoint(0.0f, 0.0f));

	m_pStringCameraPos = g_pOEUIRendererMgr->CreateStringRenderer(m_pFont);
	m_pStringCameraPos->SetPosition(CPoint(0.0f, m_pFont->GetLineHeight()));

	m_fLastFPSTime = g_pOEDevice->GetCurrTime();
	m_nFPSCount = 0;
}

CFPSWindow::~CFPSWindow()
{
	SAFE_RELEASE(m_pStringFPS);
	SAFE_RELEASE(m_pStringCameraPos);
	SAFE_RELEASE(m_pFont);
}

void CFPSWindow::Update(float fDetailTime)
{
	// calculate fps
	if (m_bShowFPS) CalculateFPS();

	// calculate camera pos
	if (m_bShowCameraPosRot) CalculateCameraPos();
}

void CFPSWindow::Render(float fDetailTime)
{
	// render fps
	if (m_bShowFPS) m_pStringFPS->Render(fDetailTime);
	if (m_bShowCameraPosRot) m_pStringCameraPos->Render(fDetailTime);
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

void CFPSWindow::CalculateCameraPos()
{
	IOENode* pCameraNode = g_pOECore->GetRootNode()->GetChildNode(TS("Camera"));
	if (!pCameraNode) return;

	const CVector3& vPos = pCameraNode->GetPosition();
	const CQuaternion& qRot = pCameraNode->GetRotation();

	tstring strText;
	COEOS::strformat(strText, TS("Camera\n    pos:(%.2f, %.2f, %.2f)\n    rot:(%.2f, %.2f, %.2f, %.2f)"), vPos.x, vPos.y, vPos.z, qRot.x, qRot.y, qRot.z, qRot.w);
	m_pStringCameraPos->SetText(strText);
}

void CFPSWindow::ShowFPS(bool bShow)
{
	m_bShowFPS = bShow;
}

void CFPSWindow::ShowCameraPosRot(bool bShow)
{
	m_bShowCameraPosRot = bShow;
}
