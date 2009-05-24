/*!
 * \file OpenGLModule.cpp
 * \date 24-5-2009 10:55:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OpenGLModule.h"
#include "OEOpenGLDevice_Impl.h"

bool CreateSingleton()
{
	COEOpenGLDevice_Impl* pOEOpenGLDevice_Impl = new COEOpenGLDevice_Impl();
	if (!pOEOpenGLDevice_Impl) return false;

	return true;
}

void DestroySingleton()
{
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
