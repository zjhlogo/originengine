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

bool CreateSingleton()
{
	COED3DDevice_Impl* pOED3DDevice_Impl = new COED3DDevice_Impl();
	if (!pOED3DDevice_Impl) return false;

	COED3DRenderer_Impl* pOED3DRenderer_Impl = new COED3DRenderer_Impl();
	if (!pOED3DDevice_Impl) return false;

	COED3DTextureMgr_Impl* pOED3DTextureMgr_Impl = new COED3DTextureMgr_Impl();
	if (!pOED3DTextureMgr_Impl) return false;

	COED3DShaderMgr_Impl* pOED3DShaderMgr_Impl = new COED3DShaderMgr_Impl();
	if (!pOED3DShaderMgr_Impl) return false;

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOEShaderMgr);;
	SAFE_DELETE(g_pOETextureMgr);
	SAFE_DELETE(g_pOERenderer);
	SAFE_DELETE(g_pOEDevice);
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
