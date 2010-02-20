/*!
 * \file OECore_Impl.cpp
 * \date 24-5-2009 9:25:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OECore_Impl.h"
#include <IOEDevice.h>
#include <IOEControlMgr.h>
#include <IOERenderMgr.h>
#include <IOELogFileMgr.h>
#include <IOERenderSystem.h>
#include <IOEApp.h>
#include <OEMsgID.h>
#include <IOEMsgMgr.h>

COECore_Impl::COECore_Impl()
{
	g_pOECore = this;
	m_bOK = Init();
}

COECore_Impl::~COECore_Impl()
{
	Destroy();
	g_pOECore = NULL;
}

bool COECore_Impl::Init()
{
	m_bRunning = false;
	m_fCurrFPS = 0.0f;
	m_fLastFPSTime = 0.0f;
	m_nFPSCount = 0;
	m_pFontFPS = NULL;
	m_pStringFPS = NULL;
	return true;
}

void COECore_Impl::Destroy()
{
	// TODO: 
}

bool COECore_Impl::Initialize()
{
	if (!g_pOEDevice->CreateDevice())
	{
		LOGOUT(TS("IOECore::Initialize Failed"));
		return false;
	}

	if (!g_pOEControlMgr->Initialize())
	{
		LOGOUT(TS("IOECore::Initialize Failed"));
		return false;
	}

	if (!g_pOERenderMgr->Initialize())
	{
		LOGOUT(TS("IOECore::Initialize Failed"));
		return false;
	}

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_START_PERFORM, this, (MSG_FUNC)&COECore_Impl::OnStartPerform);
	g_pOEMsgMgr->RegisterMessage(OMI_PRE_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnPreUpdate);
	g_pOEMsgMgr->RegisterMessage(OMI_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnUpdate);
	g_pOEMsgMgr->RegisterMessage(OMI_POST_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnPostUpdate);
	g_pOEMsgMgr->RegisterMessage(OMI_PRE_RENDER, this, (MSG_FUNC)&COECore_Impl::OnPreRender);
	g_pOEMsgMgr->RegisterMessage(OMI_RENDER, this, (MSG_FUNC)&COECore_Impl::OnRender);
	g_pOEMsgMgr->RegisterMessage(OMI_POST_RENDER, this, (MSG_FUNC)&COECore_Impl::OnPostRender);

	// initialize fps string
	m_pFontFPS = g_pOEUIFontMgr->CreateBitmapFont(TS("12px_Tahoma.fnt"));
	m_pStringFPS = g_pOEUIStringMgr->CreateUIString(m_pFontFPS);

	LOGOUT(TS("IOECore::Initialize OK"));
	return true;
}

void COECore_Impl::Terminate()
{
	End();

	SAFE_RELEASE(m_pStringFPS);
	SAFE_RELEASE(m_pFontFPS);

	g_pOERenderMgr->Terminate();
	g_pOEControlMgr->Terminate();
	g_pOEDevice->DestroyDevice();
}

void COECore_Impl::Run()
{
	if (m_bRunning) return;
	m_bRunning = true;

	g_pOEDevice->StartPerform();
}

void COECore_Impl::End()
{
	if (!m_bRunning) return;
	m_bRunning = false;

	g_pOEDevice->EndPerform();
}

void COECore_Impl::CalculateFPS()
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

bool COECore_Impl::OnStartPerform(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_fLastFPSTime = g_pOEDevice->GetCurrTime();
	m_nFPSCount = 0;
	return true;
}

bool COECore_Impl::OnPreUpdate(uint nMsgID, COEDataBufferRead* pDBRead)
{
	// dispatch message
	g_pOEMsgMgr->DispatchMessage();
	return true;
}

bool COECore_Impl::OnUpdate(uint nMsgID, COEDataBufferRead* pDBRead)
{
	g_pOEApp->Update(g_pOEDevice->GetDetailTime());
	return true;
}

bool COECore_Impl::OnPostUpdate(uint nMsgID, COEDataBufferRead* pDBRead)
{
	// TODO: 
	return true;
}

bool COECore_Impl::OnPreRender(uint nMsgID, COEDataBufferRead* pDBRead)
{
	g_pOERenderSystem->SetShader(NULL);
	return true;
}

bool COECore_Impl::OnRender(uint nMsgID, COEDataBufferRead* pDBRead)
{
	// render app
	g_pOEApp->Render(g_pOEDevice->GetDetailTime());

	// render fps
	CalculateFPS();
	m_pStringFPS->Render(CPoint(0.0f, 0.0f));

	return true;
}

bool COECore_Impl::OnPostRender(uint nMsgID, COEDataBufferRead* pDBRead)
{
	g_pOEUIRenderSystem->FlushAll();
	g_pOERenderSystem->SetShader(NULL);
	return true;
}
