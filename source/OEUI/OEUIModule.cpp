/*!
 * \file OEUIModule.cpp
 * \date 27-7-2009 17:51:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIModule.h"
#include "OEUIRenderer_Impl.h"

COEUIRenderer_Impl* g_pOEUIRenderer_Impl = NULL;

bool CreateSingleton()
{
	g_pOEUIRenderer_Impl = new COEUIRenderer_Impl();
	if (!g_pOEUIRenderer_Impl || !g_pOEUIRenderer_Impl->IsOK()) return false;

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOEUIRenderer_Impl);
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
