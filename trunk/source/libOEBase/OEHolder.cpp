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
#include <IOEMsgMgr.h>

// ����ģ��
#include <IOECore.h>
#include <IOEConfigFileMgr.h>
#include <IOEResMgr.h>
#include <IOEControlMgr.h>
#include <IOERenderMgr.h>

// ��Ⱦģ��
#include <IOEDevice.h>
#include <IOERenderSystem.h>
#include <IOETextureMgr.h>
#include <IOEShaderMgr.h>

// UIģ��
#include <OEUI/IOEUIRenderSystem.h>
#include <OEUI/IOEUIFontMgr.h>
#include <OEUI/IOEUIStringMgr.h>

// �û���
#include <IOEApp.h>

// ����ģ��
IOEFileMgr* g_pOEFileMgr = NULL;
IOELogFileMgr* g_pOELogFileMgr = NULL;
IOEXmlMgr* g_pOEXmlMgr = NULL;
IOEMsgMgr* g_pOEMsgMgr = NULL;

// ����ģ��
IOECore* g_pOECore = NULL;
IOEConfigFileMgr* g_pOEConfigFileMgr = NULL;
IOEResMgr* g_pOEResMgr = NULL;
IOEControlMgr* g_pOEControlMgr = NULL;
IOERenderMgr* g_pOERenderMgr = NULL;

// ��Ⱦģ��
IOEDevice* g_pOEDevice = NULL;
IOERenderSystem* g_pOERenderSystem = NULL;
IOETextureMgr* g_pOETextureMgr = NULL;
IOEShaderMgr* g_pOEShaderMgr = NULL;

// UIģ��
IOEUIRenderSystem* g_pOEUIRenderSystem = NULL;
IOEUIFontMgr* g_pOEUIFontMgr = NULL;
IOEUIStringMgr* g_pOEUIStringMgr = NULL;

// �û���
IOEApp* g_pOEApp = NULL;

COEHolder::COEHolder()
{
	Init();
}

COEHolder::~COEHolder()
{
	Destroy();
}

void COEHolder::Init()
{
	// ����ģ��
	AddInterface(TS("IOEFileMgr"), (void**)&g_pOEFileMgr);
	AddInterface(TS("IOELogFileMgr"), (void**)&g_pOELogFileMgr);
	AddInterface(TS("IOEXmlMgr"), (void**)&g_pOEXmlMgr);
	AddInterface(TS("IOEMsgMgr"), (void**)&g_pOEMsgMgr);

	// ����ģ��
	AddInterface(TS("IOECore"), (void**)&g_pOECore);
	AddInterface(TS("IOEConfigFileMgr"), (void**)&g_pOEConfigFileMgr);
	AddInterface(TS("IOEResMgr"), (void**)&g_pOEResMgr);
	AddInterface(TS("IOEControlMgr"), (void**)&g_pOEControlMgr);
	AddInterface(TS("IOERenderMgr"), (void**)&g_pOERenderMgr);

	// ��Ⱦģ��
	AddInterface(TS("IOEDevice"), (void**)&g_pOEDevice);
	AddInterface(TS("IOERenderSystem"), (void**)&g_pOERenderSystem);
	AddInterface(TS("IOETextureMgr"), (void**)&g_pOETextureMgr);
	AddInterface(TS("IOEShaderMgr"), (void**)&g_pOEShaderMgr);

	// UIģ��
	AddInterface(TS("IOEUIRenderSystem"), (void**)&g_pOEUIRenderSystem);
	AddInterface(TS("IOEUIFontMgr"), (void**)&g_pOEUIFontMgr);
	AddInterface(TS("IOEUIStringMgr"), (void**)&g_pOEUIStringMgr);

	// �û���
	AddInterface(TS("IOEApp"), (void**)&g_pOEApp);
}

void COEHolder::Destroy()
{
	// TODO: 
}

void COEHolder::AddInterface(const tstring& strClassName, void** ppInterface)
{
	INTERFACE_INFO Info;
	Info.strClassName = strClassName;
	Info.ppInterface = ppInterface;

	TM_INTERFACE_INFO::iterator itfound = m_InterfaceInfoMap.find(Info.strClassName);
	if (itfound != m_InterfaceInfoMap.end()) return;

	m_InterfaceInfoMap.insert(std::make_pair(Info.strClassName, Info));
}

void COEHolder::MergeFrom(const COEHolder& Holder)
{
	for (TM_INTERFACE_INFO::const_iterator it = Holder.m_InterfaceInfoMap.begin(); it != Holder.m_InterfaceInfoMap.end(); ++it)
	{
		const INTERFACE_INFO& Info = it->second;

		TM_INTERFACE_INFO::iterator itfound = m_InterfaceInfoMap.find(Info.strClassName);
		if (itfound != m_InterfaceInfoMap.end())
		{
			INTERFACE_INFO& FoundInfo = itfound->second;
			*(FoundInfo.ppInterface) = *(Info.ppInterface);
		}
	}
}
