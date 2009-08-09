/*!
 * \file OEModule.cpp
 * \date 24-5-2009 10:25:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModule.h"
#include "OECore_Impl.h"
#include "OEConfigFileMgr_Impl.h"
#include "OEMeshMgr_Impl.h"

#include <OEOS.h>

static COEOS::OEMODULE g_hModuleRenderer = NULL;
static COEOS::OEMODULE g_hModuleUI = NULL;

COECore_Impl* g_pOECore_Impl = NULL;
COEConfigFileMgr_Impl* g_pOEConfigFileMgr_Impl = NULL;
COEMeshMgr_Impl* g_pOEMeshMgr_Impl = NULL;

bool CreateSingleton()
{
	g_pOECore_Impl = new COECore_Impl();
	if (!g_pOECore_Impl) return false;

	g_pOEConfigFileMgr_Impl = new COEConfigFileMgr_Impl();
	if (!g_pOEConfigFileMgr_Impl) return false;

	g_pOEMeshMgr_Impl = new COEMeshMgr_Impl();
	if (!g_pOEMeshMgr_Impl) return false;

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOECore_Impl);
	SAFE_DELETE(g_pOEMeshMgr_Impl);
	SAFE_DELETE(g_pOEConfigFileMgr_Impl);
}

bool OEModuleInit(COEHolder& Holder)
{
	g_OEHolder.SetupInterfaces();

	g_OEHolder.MergeInterface(Holder);

	if (!CreateSingleton())
	{
		DestroySingleton();
		return false;
	}

	Holder.MergeInterface(g_OEHolder);

	// load renderer module
	g_hModuleRenderer = COEOS::LoadOEModule(MODULE_RENDERER);
	if (!g_hModuleRenderer) return false;
	Holder.MergeInterface(g_OEHolder);

	// load ui module
	g_hModuleUI = COEOS::LoadOEModule(MODULE_UI);
	if (!g_hModuleUI) return false;
	Holder.MergeInterface(g_OEHolder);

	return true;
}

void OEModuleTerm(COEHolder& Holder)
{
	// free ui module
	COEOS::FreeOEModule(g_hModuleUI);
	g_hModuleUI = NULL;

	// free renderer module
	COEOS::FreeOEModule(g_hModuleRenderer);
	g_hModuleRenderer = NULL;

	DestroySingleton();

	Holder.MergeInterface(g_OEHolder);
}

void OEModuleSyncInterfaces(COEHolder& Holder)
{
	g_OEHolder.MergeInterface(Holder);

	COEOS::SyncModuleInterfaces(g_hModuleRenderer);
	COEOS::SyncModuleInterfaces(g_hModuleUI);
}
