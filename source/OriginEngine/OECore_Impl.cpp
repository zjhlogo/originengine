/*!
 * \file OECore_Impl.cpp
 * \date 24-5-2009 9:25:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OECore_Impl.h"
#include <OEInterfaces.h>

COECore_Impl::COECore_Impl()
{
	g_pOECore = this;
	Init();
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
		// TODO: logout
		return false;
	}

	// TODO: logout

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
