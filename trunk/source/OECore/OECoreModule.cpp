/*!
 * \file OECoreModule.cpp
 * \date 3-3-2010 10:58:27
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OECoreModule.h"
#include "OECore_Impl.h"
#include "OEConfigFileMgr_Impl.h"
#include "OEResMgr_Impl.h"
#include "OEControlMgr_Impl.h"
#include "OERenderMgr_Impl.h"

#include <libOEBase/OEOS.h>

COECore_Impl* g_pOECore_Impl = NULL;
COEConfigFileMgr_Impl* g_pOEConfigFileMgr_Impl = NULL;
COEResMgr_Impl* g_pOEResMgr_Impl = NULL;
COEControlMgr_Impl* g_pOEControlMgr_Impl = NULL;
COERenderMgr_Impl* g_pOERenderMgr_Impl = NULL;
static COEHolder g_OEHolder;

bool CreateSingleton()
{
	g_pOECore_Impl = new COECore_Impl();
	if (!g_pOECore_Impl || !g_pOECore_Impl->IsOK())
	{
		SAFE_DELETE(g_pOECore_Impl);
		return false;
	}

	g_pOEConfigFileMgr_Impl = new COEConfigFileMgr_Impl();
	if (!g_pOEConfigFileMgr_Impl || !g_pOEConfigFileMgr_Impl->IsOK())
	{
		SAFE_DELETE(g_pOEConfigFileMgr_Impl);
		return false;
	}

	g_pOEResMgr_Impl = new COEResMgr_Impl();
	if (!g_pOEResMgr_Impl || !g_pOEResMgr_Impl->IsOK())
	{
		SAFE_DELETE(g_pOEResMgr_Impl);
		return false;
	}

	g_pOEControlMgr_Impl = new COEControlMgr_Impl();
	if (!g_pOEControlMgr_Impl || !g_pOEControlMgr_Impl->IsOK())
	{
		SAFE_DELETE(g_pOEControlMgr_Impl);
		return false;
	}

	g_pOERenderMgr_Impl = new COERenderMgr_Impl();
	if (!g_pOERenderMgr_Impl || !g_pOERenderMgr_Impl->IsOK())
	{
		SAFE_DELETE(g_pOERenderMgr_Impl);
		return false;
	}

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOERenderMgr_Impl);
	SAFE_DELETE(g_pOEControlMgr_Impl);
	SAFE_DELETE(g_pOEResMgr_Impl);
	SAFE_DELETE(g_pOEConfigFileMgr_Impl);
	SAFE_DELETE(g_pOECore_Impl);
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
