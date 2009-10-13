/*!
 * \file OpenGLModule.cpp
 * \date 24-5-2009 10:55:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OpenGLModule.h"
#include "OEOpenGLDevice_Impl.h"

COEOpenGLDevice_Impl* g_pOEOpenGLDevice_Impl = NULL;

bool CreateSingleton()
{
	g_pOEOpenGLDevice_Impl = new COEOpenGLDevice_Impl();
	if (!g_pOEOpenGLDevice_Impl || !g_pOEOpenGLDevice_Impl->IsOK())
	{
		SAFE_DELETE(g_pOEOpenGLDevice_Impl);
		return false;
	}

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOEOpenGLDevice_Impl);
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

void OEModuleSyncInterfaces(COEHolder& Holder)
{
	g_OEHolder.MergeInterface(Holder);
}