/*!
 * \file OEUIModule.cpp
 * \date 27-7-2009 17:51:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIModule.h"
#include "OEUIRenderSystem_Impl.h"
#include "OEUIResMgr_Impl.h"
#include "OEUIRendererMgr_Impl.h"

COEUIRenderSystem_Impl* g_pOEUIRenderSystem_Impl = NULL;
COEUIResMgr_Impl* g_pOEUIResMgr_Impl = NULL;
COEUIRendererMgr_Impl* g_pOEUIRendererMgr_Impl = NULL;
static COEHolder g_OEHolder;

bool CreateSingleton()
{
	g_pOEUIRenderSystem_Impl = new COEUIRenderSystem_Impl();
	if (!g_pOEUIRenderSystem_Impl || !g_pOEUIRenderSystem_Impl->IsOK())
	{
		SAFE_DELETE(g_pOEUIRenderSystem_Impl);
		return false;
	}

	g_pOEUIResMgr_Impl = new COEUIResMgr_Impl();
	if (!g_pOEUIResMgr_Impl || !g_pOEUIResMgr_Impl->IsOK())
	{
		SAFE_DELETE(g_pOEUIResMgr_Impl);
		return false;
	}

	g_pOEUIRendererMgr_Impl = new COEUIRendererMgr_Impl();
	if (!g_pOEUIRendererMgr_Impl || !g_pOEUIRendererMgr_Impl->IsOK())
	{
		SAFE_DELETE(g_pOEUIRendererMgr_Impl);
		return false;
	}

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOEUIRendererMgr_Impl);
	SAFE_DELETE(g_pOEUIResMgr_Impl);
	SAFE_DELETE(g_pOEUIRenderSystem_Impl);
}

bool OEModuleInit(COEHolder& Holder)
{
	g_OEHolder.MergeFrom(Holder);

	if (!CreateSingleton())
	{
		DestroySingleton();
		return false;
	}

	Holder.MergeFrom(g_OEHolder);

	return true;
}

void OEModuleTerm(COEHolder& Holder)
{
	DestroySingleton();
	Holder.MergeFrom(g_OEHolder);
}

void OEModuleSyncInterfaces(COEHolder& Holder)
{
	g_OEHolder.MergeFrom(Holder);
}
