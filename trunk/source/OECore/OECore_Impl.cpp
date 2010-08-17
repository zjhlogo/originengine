/*!
 * \file OECore_Impl.cpp
 * \date 24-5-2009 9:25:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OECore_Impl.h"

#include <OEBase/IOEFileMgr.h>
#include <OEBase/IOELogFileMgr.h>
#include <OECore/IOEConfigFileMgr.h>
#include <OEBase/IOEXmlMgr.h>
#include <OECore/IOEResMgr.h>
#include <OECore/IOEControlMgr.h>
#include <OECore/IOERenderMgr.h>
#include <OECore/IOEDevice.h>
#include <OECore/IOETextureMgr.h>
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOERenderSystem.h>
#include <OEUI/IOEUIRenderSystem.h>
#include <libOEBase/IOEApp.h>
#include <libOEBase/OEMsgID.h>
#include <OEBase/IOEMsgMgr.h>

COECore_Impl::COECore_Impl()
{
	g_pOECore = this;
	m_bOK = Init();
}

COECore_Impl::~COECore_Impl()
{
	Destroy();
	g_pOECore = NULL;
}

bool COECore_Impl::Init()
{
	m_bRunning = false;
	m_fCurrFPS = 0.0f;
	m_fLastFPSTime = 0.0f;
	m_nFPSCount = 0;
	m_pFontFPS = NULL;
	m_pStringFPS = NULL;
	return true;
}

void COECore_Impl::Destroy()
{
	// TODO: 
}

bool COECore_Impl::Initialize()
{
	if (!InitializeInterfaces())
	{
		LOGOUT(TS("IOECore::Initialize Failed"));
		return false;
	}

	// registe message
	RegisterMessage();

	// initialize fps string
	m_pFontFPS = g_pOEUIFontMgr->CreateBitmapFont(TS("12px_Tahoma.fnt"));
	m_pStringFPS = g_pOEUIStringMgr->CreateUIString(m_pFontFPS);

	LOGOUT(TS("IOECore::Initialize OK"));
	return true;
}

void COECore_Impl::Terminate()
{
	End();

	SAFE_RELEASE(m_pStringFPS);
	SAFE_RELEASE(m_pFontFPS);

	TerminateInterfaces();
}

void COECore_Impl::Run()
{
	if (m_bRunning) return;
	m_bRunning = true;

	g_pOEDevice->StartPerform();
}

void COECore_Impl::End()
{
	if (!m_bRunning) return;
	m_bRunning = false;

	g_pOEDevice->EndPerform();
}

bool COECore_Impl::InitializeInterfaces()
{
	// 文件管理类
	if (!g_pOEFileMgr->Initialize()) return false;

	// 日志管理类
	if (!g_pOELogFileMgr->Initialize()) return false;

	// Xml 管理类
	if (!g_pOEXmlMgr->Initialize()) return false;

	// 消息管理类
	if (!g_pOEMsgMgr->Initialize()) return false;

	// 配置文件管理类
	if (!g_pOEConfigFileMgr->Initialize()) return false;

	// 渲染设备管理类
	if (!g_pOEDevice->Initialize()) return false;

	// 渲染接口管理类
	if (!g_pOERenderSystem->Initialize()) return false;

	// 纹理管理类
	if (!g_pOETextureMgr->Initialize()) return false;

	// Shader 管理类
	if (!g_pOEShaderMgr->Initialize()) return false;

	// 核心资源理类
	if (!g_pOEResMgr->Initialize()) return false;

	// 模型控制器管理类
	if (!g_pOEControlMgr->Initialize()) return false;

	// 模型渲染器管理类
	if (!g_pOERenderMgr->Initialize()) return false;

	// UI 渲染管理类
	if (!g_pOEUIRenderSystem->Initialize()) return false;

	// UI 字体管理类
	if (!g_pOEUIFontMgr->Initialize()) return false;

	// UI 字符串管理类
	if (!g_pOEUIStringMgr->Initialize()) return false;

	// 应用程序接口类
	if (!g_pOEApp->Initialize()) return false;

	return true;
}

void COECore_Impl::TerminateInterfaces()
{
	// 应用程序接口类
	g_pOEApp->Initialize();

	// UI 字符串管理类
	g_pOEUIStringMgr->Initialize();

	// UI 字体管理类
	g_pOEUIFontMgr->Initialize();

	// UI 渲染管理类
	g_pOEUIRenderSystem->Initialize();

	// 模型渲染器管理类
	g_pOERenderMgr->Initialize();

	// 模型控制器管理类
	g_pOEControlMgr->Initialize();

	// 核心资源理类
	g_pOEResMgr->Initialize();

	// Shader 管理类
	g_pOEShaderMgr->Initialize();

	// 纹理管理类
	g_pOETextureMgr->Initialize();

	// 渲染接口管理类
	g_pOERenderSystem->Initialize();

	// 渲染设备管理类
	g_pOEDevice->Initialize();

	// 消息管理类
	g_pOEMsgMgr->Initialize();

	// Xml 管理类
	g_pOEXmlMgr->Initialize();

	// 配置文件管理类
	g_pOEConfigFileMgr->Initialize();

	// 日志管理类
	g_pOELogFileMgr->Initialize();

	// 文件管理类
	g_pOEFileMgr->Initialize();
}

void COECore_Impl::RegisterMessage()
{
	g_pOEMsgMgr->RegisterMessage(OMI_START_PERFORM, this, (MSG_FUNC)&COECore_Impl::OnStartPerform);
	g_pOEMsgMgr->RegisterMessage(OMI_PRE_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnPreUpdate);
	g_pOEMsgMgr->RegisterMessage(OMI_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnUpdate);
	g_pOEMsgMgr->RegisterMessage(OMI_POST_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnPostUpdate);
	g_pOEMsgMgr->RegisterMessage(OMI_PRE_RENDER, this, (MSG_FUNC)&COECore_Impl::OnPreRender);
	g_pOEMsgMgr->RegisterMessage(OMI_RENDER, this, (MSG_FUNC)&COECore_Impl::OnRender);
	g_pOEMsgMgr->RegisterMessage(OMI_POST_RENDER, this, (MSG_FUNC)&COECore_Impl::OnPostRender);
}

void COECore_Impl::UnregisterMessage()
{
	g_pOEMsgMgr->UnregisterMessage(OMI_START_PERFORM, this);
	g_pOEMsgMgr->UnregisterMessage(OMI_PRE_UPDATE, this);
	g_pOEMsgMgr->UnregisterMessage(OMI_UPDATE, this);
	g_pOEMsgMgr->UnregisterMessage(OMI_POST_UPDATE, this);
	g_pOEMsgMgr->UnregisterMessage(OMI_PRE_RENDER, this);
	g_pOEMsgMgr->UnregisterMessage(OMI_RENDER, this);
	g_pOEMsgMgr->UnregisterMessage(OMI_POST_RENDER, this);
}

void COECore_Impl::CalculateFPS()
{
	++m_nFPSCount;

	float fCurrTime = g_pOEDevice->GetCurrTime();
	float fDetailFPS = fCurrTime - m_fLastFPSTime;

	if (fDetailFPS > 1.0f)
	{
		m_fCurrFPS = m_nFPSCount/fDetailFPS;
		m_nFPSCount = 0;
		m_fLastFPSTime = fCurrTime;

		tstring strText;
		COEOS::strformat(strText, TS("%.2f FPS"), m_fCurrFPS);
		m_pStringFPS->SetText(strText);
	}
}

bool COECore_Impl::OnStartPerform(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_fLastFPSTime = g_pOEDevice->GetCurrTime();
	m_nFPSCount = 0;
	return true;
}

bool COECore_Impl::OnPreUpdate(uint nMsgID, COEDataBufferRead* pDBRead)
{
	// dispatch message
	g_pOEMsgMgr->DispatchMessage();
	return true;
}

bool COECore_Impl::OnUpdate(uint nMsgID, COEDataBufferRead* pDBRead)
{
	g_pOEApp->Update(g_pOEDevice->GetDetailTime());
	// calculate fps
	CalculateFPS();
	return true;
}

bool COECore_Impl::OnPostUpdate(uint nMsgID, COEDataBufferRead* pDBRead)
{
	// TODO: 
	return true;
}

bool COECore_Impl::OnPreRender(uint nMsgID, COEDataBufferRead* pDBRead)
{
	// TODO: 
	return true;
}

bool COECore_Impl::OnRender(uint nMsgID, COEDataBufferRead* pDBRead)
{
	g_pOEApp->Render(g_pOEDevice->GetDetailTime());
	// render fps
	m_pStringFPS->Render(OEUI_ZERO_POINT);
	return true;
}

bool COECore_Impl::OnPostRender(uint nMsgID, COEDataBufferRead* pDBRead)
{
	// TODO: 
	return true;
}
