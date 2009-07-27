/*!
 * \file D3DModule.cpp
 * \date 24-5-2009 10:42:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "D3DModule.h"
#include "OED3DDevice_Impl.h"
#include "OED3DRenderer_Impl.h"
#include "OED3DTextureMgr_Impl.h"
#include "OED3DShaderMgr_Impl.h"

COED3DDevice_Impl* g_pOED3DDevice_Impl = NULL;
COED3DRenderer_Impl* g_pOED3DRenderer_Impl = NULL;
COED3DTextureMgr_Impl* g_pOED3DTextureMgr_Impl = NULL;
COED3DShaderMgr_Impl* g_pOED3DShaderMgr_Impl = NULL;

bool CreateSingleton()
{
	g_pOED3DDevice_Impl = new COED3DDevice_Impl();
	if (!g_pOED3DDevice_Impl) return false;

	g_pOED3DRenderer_Impl = new COED3DRenderer_Impl();
	if (!g_pOED3DRenderer_Impl) return false;

	g_pOED3DTextureMgr_Impl = new COED3DTextureMgr_Impl();
	if (!g_pOED3DTextureMgr_Impl) return false;

	g_pOED3DShaderMgr_Impl = new COED3DShaderMgr_Impl();
	if (!g_pOED3DShaderMgr_Impl) return false;

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOED3DShaderMgr_Impl);
	SAFE_DELETE(g_pOED3DTextureMgr_Impl);
	SAFE_DELETE(g_pOED3DRenderer_Impl);
	SAFE_DELETE(g_pOED3DDevice_Impl);
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

	return true;
}

void OEModuleTerm(COEHolder& Holder)
{
	DestroySingleton();

	Holder.MergeInterface(g_OEHolder);
}
