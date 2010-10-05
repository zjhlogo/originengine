/*!
 * \file RenderSystemOGLModule.cpp
 * \date 3-3-2010 11:00:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "RenderSystemOGLModule.h"
#include "OEOGLDevice_Impl.h"

COEOGLDevice_Impl* g_pOEOGLDevice_Impl = NULL;
static COEHolder g_OEHolder;

bool CreateSingleton()
{
	g_pOEOGLDevice_Impl = new COEOGLDevice_Impl();
	if (!g_pOEOGLDevice_Impl || !g_pOEOGLDevice_Impl->IsOK())
	{
		SAFE_DELETE(g_pOEOGLDevice_Impl);
		return false;
	}

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOEOGLDevice_Impl);
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
