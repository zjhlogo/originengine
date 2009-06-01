/*!
 * \file OEModule.cpp
 * \date 24-5-2009 10:25:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModule.h"
#include "OECore_Impl.h"
#include "OEFileMgr_Impl.h"
#include <OEOS.h>

COEOS::OEMODULE g_hModuleRenderer = NULL;

bool CreateSingleton()
{
	COECore_Impl* pOECore_Impl = new COECore_Impl();
	if (!pOECore_Impl) return false;

	COEFileMgr_Impl* pOEFileMgr_Impl = new COEFileMgr_Impl();
	if (!pOEFileMgr_Impl) return false;

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOECore);
	SAFE_DELETE(g_pOEFileMgr);
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

	g_hModuleRenderer = COEOS::LoadOEModule(MODULE_RENDERER);
	if (!g_hModuleRenderer) return false;
	Holder.MergeInterface(g_OEHolder);

	return true;
}

void OEModuleTerm(COEHolder& Holder)
{
	COEOS::FreeOEModule(g_hModuleRenderer);

	DestroySingleton();

	Holder.MergeInterface(g_OEHolder);
}
