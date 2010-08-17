/*!
 * \file IOEApp.cpp
 * \date 24-5-2009 9:11:36
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <libOEBase/IOEApp.h>
#include <OECore/IOECore.h>
#include <libOEBase/OEHolder.h>
#include <libOEBase/OEOS.h>
#include <assert.h>

static COEOS::OEMODULE g_hModuleBase = NULL;
static COEOS::OEMODULE g_hModuleCore = NULL;
static COEOS::OEMODULE g_hModuleRenderSystem = NULL;
static COEOS::OEMODULE g_hModuleUI = NULL;

static bool LoadModules(COEHolder& Holder)
{
	// load base module
	g_hModuleBase = COEOS::LoadOEModule(MODULE_BASE, Holder);
	if (!g_hModuleBase) return false;

	// load core module
	g_hModuleCore = COEOS::LoadOEModule(MODULE_CORE, Holder);
	if (!g_hModuleCore) return false;

	// load renderer module
	g_hModuleRenderSystem = COEOS::LoadOEModule(MODULE_RENDERSYSTEM, Holder);
	if (!g_hModuleRenderSystem) return false;

	// load ui module
	g_hModuleUI = COEOS::LoadOEModule(MODULE_UI, Holder);
	if (!g_hModuleUI) return false;

	COEOS::SyncModuleInterfaces(g_hModuleBase, Holder);
	COEOS::SyncModuleInterfaces(g_hModuleCore, Holder);
	COEOS::SyncModuleInterfaces(g_hModuleRenderSystem, Holder);
	COEOS::SyncModuleInterfaces(g_hModuleUI, Holder);
	return true;
}

static void FreeModules(COEHolder& Holder)
{
	// free ui module
	COEOS::FreeOEModule(g_hModuleUI, Holder);
	g_hModuleUI = NULL;

	// free renderer module
	COEOS::FreeOEModule(g_hModuleRenderSystem, Holder);
	g_hModuleRenderSystem = NULL;

	// free core module
	COEOS::FreeOEModule(g_hModuleCore, Holder);
	g_hModuleCore = NULL;

	// free base module
	COEOS::FreeOEModule(g_hModuleBase, Holder);
	g_hModuleBase = NULL;
}

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
	static COEHolder s_OEHolder;

	if (LoadModules(s_OEHolder))
	{
		if (g_pOECore->Initialize())
		{
			g_pOECore->Run();
		}

		g_pOECore->Terminate();
	}

	FreeModules(s_OEHolder);
}
