/*!
 * \file IOEApp.cpp
 * \date 24-5-2009 9:11:36
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <IOEApp.h>
#include <OEHolder.h>
#include <IOECore.h>

IOEApp::IOEApp()
{
	g_pOEApp = this;
	m_bOK = true;
}

IOEApp::~IOEApp()
{
	g_pOEApp = NULL;
}

void IOEApp::Run()
{
	if (g_OEHolder.Initialize())
	{
		if (g_pOECore->Initialize())
		{
			if (Initialize())
			{
				g_pOECore->Run();
			}

			Terminate();
		}

		g_pOECore->Terminate();
	}

	g_OEHolder.Terminate();
}
