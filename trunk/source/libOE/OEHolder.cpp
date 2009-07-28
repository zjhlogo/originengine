/*!
 * \file OEHolder.cpp
 * \date 24-5-2009 0:37:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEHolder.h>
#include <OEInterfaces.h>
#include <OEOS.h>
#include <OEUI/OEUI.h>

// 核心模块
IOECore* g_pOECore = NULL;
IOEFileMgr* g_pOEFileMgr = NULL;
IOELogFileMgr* g_pOELogFileMgr = NULL;
IOEXmlMgr* g_pOEXmlMgr = NULL;
IOEConfigFileMgr* g_pOEConfigFileMgr = NULL;

// 渲染模块
IOEDevice* g_pOEDevice = NULL;
IOERenderer* g_pOERenderer = NULL;
IOETextureMgr* g_pOETextureMgr = NULL;
IOEShaderMgr* g_pOEShaderMgr = NULL;

// UI模块
IOEUIRenderer* g_pOEUIRenderer = NULL;
IOEUIFontMgr* g_pOEUIFontMgr = NULL;
IOEUIStringMgr* g_pOEUIStringMgr = NULL;

// 用户层
IOEApp* g_pOEApp = NULL;

static COEOS::OEMODULE g_hModuleOE = NULL;
COEHolder g_OEHolder;

COEHolder::COEHolder()
{
	// TODO: 
}

COEHolder::~COEHolder()
{
	// TODO: 
}

void COEHolder::SetupInterfaces()
{
	// 核心模块
	AddInterface(_T("IOEFileMgr"), (void**)&g_pOEFileMgr);
	AddInterface(_T("IOELogFileMgr"), (void**)&g_pOELogFileMgr);
	AddInterface(_T("IOEXmlMgr"), (void**)&g_pOEXmlMgr);
	AddInterface(_T("IOEConfigFileMgr"), (void**)&g_pOEConfigFileMgr);
	AddInterface(_T("IOECore"), (void**)&g_pOECore);

	// 渲染模块
	AddInterface(_T("IOEDevice"), (void**)&g_pOEDevice);
	AddInterface(_T("IOERenderer"), (void**)&g_pOERenderer);
	AddInterface(_T("IOETextureMgr"), (void**)&g_pOETextureMgr);
	AddInterface(_T("IOEShaderMgr"), (void**)&g_pOEShaderMgr);

	// UI模块
	AddInterface(_T("IOEUIRenderer"), (void**)&g_pOEUIRenderer);
	AddInterface(_T("IOEUIFontMgr"), (void**)&g_pOEUIFontMgr);
	AddInterface(_T("IOEUIStringMgr"), (void**)&g_pOEUIStringMgr);

	// 用户层
	AddInterface(_T("IOEApp"), (void**)&g_pOEApp);
}

bool COEHolder::Initialize()
{
	SetupInterfaces();

	// load module OriginEngine
	g_hModuleOE = COEOS::LoadOEModule(MODULE_ORIGINENGINE);
	if (!g_hModuleOE) return false;

	COEOS::SyncModuleInterfaces(g_hModuleOE);
	return true;
}

void COEHolder::Terminate()
{
	// free module OriginEngine
	COEOS::FreeOEModule(g_hModuleOE);
	g_hModuleOE = NULL;
}

void COEHolder::AddInterface(const tstring& strClassName, void** ppInterface)
{
	INTERFACE_INFO Info;
	Info.strClassName = strClassName;
	Info.ppInterface = ppInterface;

	INTERFACE_INFO_MAP::iterator itfound = m_InfoMap.find(Info.strClassName);
	if (itfound != m_InfoMap.end()) return;

	m_InfoMap.insert(std::make_pair(Info.strClassName, Info));
}

void COEHolder::MergeInterface(const COEHolder& Holder)
{
	for (INTERFACE_INFO_MAP::const_iterator it = Holder.m_InfoMap.begin(); it != Holder.m_InfoMap.end(); ++it)
	{
		const INTERFACE_INFO& Info = it->second;

		INTERFACE_INFO_MAP::iterator itfound = m_InfoMap.find(Info.strClassName);
		if (itfound != m_InfoMap.end())
		{
			INTERFACE_INFO& FoundInfo = itfound->second;
			*(FoundInfo.ppInterface) = *(Info.ppInterface);
		}
	}
}
