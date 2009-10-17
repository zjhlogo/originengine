/*!
 * \file OECore_Impl.cpp
 * \date 24-5-2009 9:25:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OECore_Impl.h"
#include <IOEDevice.h>
#include <IOELogFileMgr.h>
#include <IOEMessageMgr.h>
#include <OEMessageID.h>

COECore_Impl::COECore_Impl()
{
	g_pOECore = this;
	Init();
	m_bOK = true;
}

COECore_Impl::~COECore_Impl()
{
	Destroy();
	g_pOECore = NULL;
}

void COECore_Impl::Init()
{
	m_bRunning = false;
}

void COECore_Impl::Destroy()
{
	// TODO: 
}

bool COECore_Impl::Initialize()
{
	if (!g_pOEDevice->CreateDevice())
	{
		LOGOUT(t("IOECore::Initialize Failed"));
		return false;
	}

	LOGOUT(t("IOECore::Initialize OK"));

	return true;
}

void COECore_Impl::Terminate()
{
	End();
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

void COECore_Impl::Update()
{
	// dispatch the receive message
	g_pOEMessageMgr->DispatchRecvMessage();

	// send perform once message
	COEMessage msg(OMI_PERFORM_ONCE);
	float fCurrFPS = g_pOEDevice->GetFPS();
	msg.Write(fCurrFPS);
	g_pOEMessageMgr->SendMessage(&msg);
}
