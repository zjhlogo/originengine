/*!
 * \file OEHolder.cpp
 * \date 24-5-2009 0:37:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEHolder.h>
#include <OEOS.h>

// 基础模块
#include <IOEFileMgr.h>
#include <IOELogFileMgr.h>
#include <IOEXmlMgr.h>
#include <IOEMsgMgr.h>

// 核心模块
#include <IOECore.h>
#include <IOEConfigFileMgr.h>
#include <IOEResMgr.h>
#include <IOEControlMgr.h>
#include <IOERenderMgr.h>

// 渲染模块
#include <IOEDevice.h>
#include <IOERenderSystem.h>
#include <IOETextureMgr.h>
#include <IOEShaderMgr.h>

// UI模块
#include <OEUI/IOEUIRenderer.h>
#include <OEUI/IOEUIFontMgr.h>
#include <OEUI/IOEUIStringMgr.h>

// 用户层
#include <IOEApp.h>

// 基础模块
IOEFileMgr* g_pOEFileMgr = NULL;
IOELogFileMgr* g_pOELogFileMgr = NULL;
IOEXmlMgr* g_pOEXmlMgr = NULL;
IOEMsgMgr* g_pOEMsgMgr = NULL;

// 核心模块
IOECore* g_pOECore = NULL;
IOEConfigFileMgr* g_pOEConfigFileMgr = NULL;
IOEResMgr* g_pOEResMgr = NULL;
IOEControlMgr* g_pOEControlMgr = NULL;
IOERenderMgr* g_pOERenderMgr = NULL;

// 渲染模块
IOEDevice* g_pOEDevice = NULL;
IOERenderSystem* g_pOERenderSystem = NULL;
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
	// 基础模块
	AddInterface(TS("IOEFileMgr"), (void**)&g_pOEFileMgr);
	AddInterface(TS("IOELogFileMgr"), (void**)&g_pOELogFileMgr);
	AddInterface(TS("IOEXmlMgr"), (void**)&g_pOEXmlMgr);
	AddInterface(TS("IOEMsgMgr"), (void**)&g_pOEMsgMgr);

	// 核心模块
	AddInterface(TS("IOECore"), (void**)&g_pOECore);
	AddInterface(TS("IOEConfigFileMgr"), (void**)&g_pOEConfigFileMgr);
	AddInterface(TS("IOEResMgr"), (void**)&g_pOEResMgr);
	AddInterface(TS("IOEControlMgr"), (void**)&g_pOEControlMgr);
	AddInterface(TS("IOERenderMgr"), (void**)&g_pOERenderMgr);

	// 渲染模块
	AddInterface(TS("IOEDevice"), (void**)&g_pOEDevice);
	AddInterface(TS("IOERenderSystem"), (void**)&g_pOERenderSystem);
	AddInterface(TS("IOETextureMgr"), (void**)&g_pOETextureMgr);
	AddInterface(TS("IOEShaderMgr"), (void**)&g_pOEShaderMgr);

	// UI模块
	AddInterface(TS("IOEUIRenderer"), (void**)&g_pOEUIRenderer);
	AddInterface(TS("IOEUIFontMgr"), (void**)&g_pOEUIFontMgr);
	AddInterface(TS("IOEUIStringMgr"), (void**)&g_pOEUIStringMgr);

	// 用户层
	AddInterface(TS("IOEApp"), (void**)&g_pOEApp);
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
