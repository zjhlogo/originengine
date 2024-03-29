/*!
 * \file RenderSystemD3DModule.cpp
 * \date 3-3-2010 11:00:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "RenderSystemD3DModule.h"
#include "OED3DDevice_Impl.h"
#include "OED3DRenderSystem_Impl.h"
#include "OED3DTextureMgr_Impl.h"
#include "OED3DShaderMgr_Impl.h"

COED3DDevice_Impl* g_pOED3DDevice_Impl = NULL;
COED3DRenderSystem_Impl* g_pOED3DRenderSystem_Impl = NULL;
COED3DTextureMgr_Impl* g_pOED3DTextureMgr_Impl = NULL;
COED3DShaderMgr_Impl* g_pOED3DShaderMgr_Impl = NULL;
static COEHolder g_OEHolder;

bool CreateSingleton()
{
	g_pOED3DDevice_Impl = new COED3DDevice_Impl();
	if (!g_pOED3DDevice_Impl || !g_pOED3DDevice_Impl->IsOK())
	{
		SAFE_DELETE(g_pOED3DDevice_Impl);
		return false;
	}

	g_pOED3DRenderSystem_Impl = new COED3DRenderSystem_Impl();
	if (!g_pOED3DRenderSystem_Impl || !g_pOED3DRenderSystem_Impl->IsOK())
	{
		SAFE_DELETE(g_pOED3DRenderSystem_Impl);
		return false;
	}

	g_pOED3DTextureMgr_Impl = new COED3DTextureMgr_Impl();
	if (!g_pOED3DTextureMgr_Impl || !g_pOED3DTextureMgr_Impl->IsOK())
	{
		SAFE_DELETE(g_pOED3DTextureMgr_Impl);
		return false;
	}

	g_pOED3DShaderMgr_Impl = new COED3DShaderMgr_Impl();
	if (!g_pOED3DShaderMgr_Impl || !g_pOED3DShaderMgr_Impl->IsOK())
	{
		SAFE_DELETE(g_pOED3DShaderMgr_Impl);
		return false;
	}

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOED3DShaderMgr_Impl);
	SAFE_DELETE(g_pOED3DTextureMgr_Impl);
	SAFE_DELETE(g_pOED3DRenderSystem_Impl);
	SAFE_DELETE(g_pOED3DDevice_Impl);
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
