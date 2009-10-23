/*!
 * \file OEHolder.cpp
 * \date 24-5-2009 0:37:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEHolder.h>
#include <OEOS.h>

// ����ģ��
#include <IOEFileMgr.h>
#include <IOELogFileMgr.h>
#include <IOEXmlMgr.h>
#include <IOEMessageMgr.h>

// ����ģ��
#include <IOECore.h>
#include <IOEConfigFileMgr.h>
#include <IOEMeshMgr.h>
#include <IOEModelMgr.h>

// ��Ⱦģ��
#include <IOEDevice.h>
#include <IOERenderer.h>
#include <IOETextureMgr.h>
#include <IOEShaderMgr.h>

// UIģ��
#include <OEUI/IOEUIRenderer.h>
#include <OEUI/IOEUIFontMgr.h>
#include <OEUI/IOEUIStringMgr.h>

// �û���
#include <IOEApp.h>

// ����ģ��
IOEFileMgr* g_pOEFileMgr = NULL;
IOELogFileMgr* g_pOELogFileMgr = NULL;
IOEXmlMgr* g_pOEXmlMgr = NULL;
IOEMessageMgr* g_pOEMessageMgr = NULL;

// ����ģ��
IOECore* g_pOECore = NULL;
IOEConfigFileMgr* g_pOEConfigFileMgr = NULL;
IOEMeshMgr* g_pOEMeshMgr = NULL;
IOEModelMgr* g_pOEModelMgr = NULL;

// ��Ⱦģ��
IOEDevice* g_pOEDevice = NULL;
IOERenderer* g_pOERenderer = NULL;
IOETextureMgr* g_pOETextureMgr = NULL;
IOEShaderMgr* g_pOEShaderMgr = NULL;

// UIģ��
IOEUIRenderer* g_pOEUIRenderer = NULL;
IOEUIFontMgr* g_pOEUIFontMgr = NULL;
IOEUIStringMgr* g_pOEUIStringMgr = NULL;

// �û���
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
	// ����ģ��
	AddInterface(t("IOEFileMgr"), (void**)&g_pOEFileMgr);
	AddInterface(t("IOELogFileMgr"), (void**)&g_pOELogFileMgr);
	AddInterface(t("IOEXmlMgr"), (void**)&g_pOEXmlMgr);
	AddInterface(t("IOEMessageMgr"), (void**)&g_pOEMessageMgr);

	// ����ģ��
	AddInterface(t("IOECore"), (void**)&g_pOECore);
	AddInterface(t("IOEConfigFileMgr"), (void**)&g_pOEConfigFileMgr);
	AddInterface(t("IOEMeshMgr"), (void**)&g_pOEMeshMgr);
	AddInterface(t("IOEModelMgr"), (void**)&g_pOEModelMgr);

	// ��Ⱦģ��
	AddInterface(t("IOEDevice"), (void**)&g_pOEDevice);
	AddInterface(t("IOERenderer"), (void**)&g_pOERenderer);
	AddInterface(t("IOETextureMgr"), (void**)&g_pOETextureMgr);
	AddInterface(t("IOEShaderMgr"), (void**)&g_pOEShaderMgr);

	// UIģ��
	AddInterface(t("IOEUIRenderer"), (void**)&g_pOEUIRenderer);
	AddInterface(t("IOEUIFontMgr"), (void**)&g_pOEUIFontMgr);
	AddInterface(t("IOEUIStringMgr"), (void**)&g_pOEUIStringMgr);

	// �û���
	AddInterface(t("IOEApp"), (void**)&g_pOEApp);
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
