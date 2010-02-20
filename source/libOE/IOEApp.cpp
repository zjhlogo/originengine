/*!
 * \file IOEApp.cpp
 * \date 24-5-2009 9:11:36
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <IOEApp.h>
#include <IOECore.h>
#include <OEHolder.h>
#include <assert.h>

IOEApp::IOEApp()
{
	assert(!g_pOEApp);
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
			if (this->Initialize())
			{
				g_pOECore->Run();
			}

			this->Terminate();
		}

		g_pOECore->Terminate();
	}

	g_OEHolder.Terminate();
}
