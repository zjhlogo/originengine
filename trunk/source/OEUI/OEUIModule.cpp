/*!
 * \file OEUIModule.cpp
 * \date 27-7-2009 17:51:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIModule.h"
#include "OEUIRenderer_Impl.h"
#include "OEUIFontMgr_Impl.h"
#include "OEUIStringMgr_Impl.h"

COEUIRenderer_Impl* g_pOEUIRenderer_Impl = NULL;
COEUIFontMgr_Impl* g_pOEUIFontMgr_Impl = NULL;
COEUIStringMgr_Impl* g_pOEUIStringMgr_Impl = NULL;

bool CreateSingleton()
{
	g_pOEUIRenderer_Impl = new COEUIRenderer_Impl();
	if (!g_pOEUIRenderer_Impl) return false;

	g_pOEUIFontMgr_Impl = new COEUIFontMgr_Impl();
	if (!g_pOEUIFontMgr_Impl) return false;

	g_pOEUIStringMgr_Impl = new COEUIStringMgr_Impl();
	if (!g_pOEUIStringMgr_Impl) return false;

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOEUIStringMgr_Impl);
	SAFE_DELETE(g_pOEUIFontMgr_Impl);
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

void OEModuleSyncInterfaces(COEHolder& Holder)
{
	g_OEHolder.MergeInterface(Holder);
}
