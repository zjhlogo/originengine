/*!
 * \file OEBaseModule.cpp
 * \date 3-3-2010 7:42:58
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEBaseModule.h"

#include "OEFileMgr_Impl.h"
#include "OELogFileMgr_Impl.h"
#include "OEXmlMgr_Impl.h"
#include "OEMsgMgr_Impl.h"

COEFileMgr_Impl* g_pOEFileMgr_Impl = NULL;
COELogFileMgr_Impl* g_pOELogFileMgr_Impl = NULL;
COEXmlMgr_Impl* g_pOEXmlMgr_Impl = NULL;
COEMsgMgr_Impl* g_pOEMsgMgr_Impl = NULL;
static COEHolder g_OEHolder;

bool CreateSingleton()
{
	if (!g_pOEFileMgr_Impl)
	{
		g_pOEFileMgr_Impl = new COEFileMgr_Impl();
		if (!g_pOEFileMgr_Impl || !g_pOEFileMgr_Impl->IsOK())
		{
			SAFE_DELETE(g_pOEFileMgr_Impl);
			return false;
		}
	}

	if (!g_pOELogFileMgr_Impl)
	{
		g_pOELogFileMgr_Impl = new COELogFileMgr_Impl();
		if (!g_pOELogFileMgr_Impl || !g_pOELogFileMgr_Impl->IsOK())
		{
			SAFE_DELETE(g_pOELogFileMgr_Impl);
			return false;
		}
	}

	if (!g_pOEXmlMgr_Impl)
	{
		g_pOEXmlMgr_Impl = new COEXmlMgr_Impl();
		if (!g_pOEXmlMgr_Impl || !g_pOEXmlMgr_Impl->IsOK())
		{
			SAFE_DELETE(g_pOEXmlMgr_Impl);
			return false;
		}
	}

	if (!g_pOEMsgMgr_Impl)
	{
		g_pOEMsgMgr_Impl = new COEMsgMgr_Impl();
		if (!g_pOEMsgMgr_Impl || !g_pOEMsgMgr_Impl->IsOK())
		{
			SAFE_DELETE(g_pOEMsgMgr_Impl);
			return false;
		}
	}

	return true;
}

void DestroySingleton()
{
	SAFE_DELETE(g_pOEMsgMgr_Impl);
	SAFE_DELETE(g_pOEXmlMgr_Impl);
	SAFE_DELETE(g_pOELogFileMgr_Impl);
	SAFE_DELETE(g_pOEFileMgr_Impl);
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
