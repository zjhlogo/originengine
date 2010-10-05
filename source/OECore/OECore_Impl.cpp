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
#include <OEBase/IOEXmlMgr.h>
#include <OEBase/IOEMsgMgr.h>

#include <OECore/IOEConfigFileMgr.h>
#include <OECore/IOEResMgr.h>
#include <OECore/IOEControlMgr.h>
#include <OECore/IOERenderMgr.h>
#include <OECore/IOEDevice.h>
#include <OECore/IOETextureMgr.h>
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOERenderSystem.h>

#include <OEUI/IOEUIRenderSystem.h>

#include <libOEBase/IOEApp.h>
#include <libOEMsg/OEMsgList.h>

#include "OENode_Impl.h"
#include "OEFPSPrinter.h"

COECore_Impl::COECore_Impl()
:IOECore(TS("COECore_Impl"))
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
	m_pRootNode = NULL;
	m_pFPSPrinter = NULL;
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

	m_pRootNode = CreateNewNode();

	m_pFPSPrinter = new COEFPSPrinter();
	m_pRootNode->AttachObject(m_pFPSPrinter);

	// registe message
	RegisterMessage();

	// 应用程序接口类
	if (!g_pOEApp->Initialize()) return false;

	LOGOUT(TS("IOECore::Initialize OK"));
	return true;
}

void COECore_Impl::Terminate()
{
	End();

	// 应用程序接口类
	g_pOEApp->Terminate();

	m_pRootNode->DettachObject(m_pFPSPrinter);
	SAFE_RELEASE(m_pFPSPrinter);
	SAFE_RELEASE(m_pRootNode);

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

IOENode* COECore_Impl::CreateNewNode()
{
	COENode_Impl* pNode = new COENode_Impl();
	return pNode;
}

IOENode* COECore_Impl::GetRootNode()
{
	return m_pRootNode;
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
	if (!g_pOEMsgMgr->AddMsgDB(g_OEMsgListDB)) return false;

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

	return true;
}

void COECore_Impl::TerminateInterfaces()
{
	// UI 字符串管理类
	g_pOEUIStringMgr->Terminate();

	// UI 字体管理类
	g_pOEUIFontMgr->Terminate();

	// UI 渲染管理类
	g_pOEUIRenderSystem->Terminate();

	// 模型渲染器管理类
	g_pOERenderMgr->Terminate();

	// 模型控制器管理类
	g_pOEControlMgr->Terminate();

	// 核心资源理类
	g_pOEResMgr->Terminate();

	// Shader 管理类
	g_pOEShaderMgr->Terminate();

	// 纹理管理类
	g_pOETextureMgr->Terminate();

	// 渲染接口管理类
	g_pOERenderSystem->Terminate();

	// 渲染设备管理类
	g_pOEDevice->Terminate();

	// 配置文件管理类
	g_pOEConfigFileMgr->Terminate();

	// 消息管理类
	g_pOEMsgMgr->Terminate();

	// Xml 管理类
	g_pOEXmlMgr->Terminate();

	// 日志管理类
	g_pOELogFileMgr->Terminate();

	// 文件管理类
	g_pOEFileMgr->Terminate();
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

bool COECore_Impl::UpdateNodes(IOENode* pNode, float fDetailTime)
{
	if (!pNode) return false;

	int nNumAttachedObjects = pNode->GetNumAttachedObjects();
	for (int i = 0; i < nNumAttachedObjects; ++i)
	{
		IOEObject* pObject = pNode->GetAttachedObject(i);
		if (!pObject) return false;
		if (pObject->GetClassName() != TS("IOERenderableObject")) return false;

		IOERenderableObject* pRenderableObject = (IOERenderableObject*)pObject;
		pRenderableObject->Update(fDetailTime);
	}

	int nNumChildNodes = pNode->GetNumChildNodes();
	for (int i = 0; i < nNumChildNodes; ++i)
	{
		bool bOK = UpdateNodes(pNode->GetChildNode(i), fDetailTime);
		if (!bOK)
		{
			// TODO: logout
		}
	}

	return true;
}

bool COECore_Impl::RenderNodes(IOENode* pNode, float fDetailTime)
{
	if (!pNode) return false;

	int nNumAttachedObjects = pNode->GetNumAttachedObjects();
	for (int i = 0; i < nNumAttachedObjects; ++i)
	{
		IOEObject* pObject = pNode->GetAttachedObject(i);
		if (!pObject) return false;
		if (pObject->GetClassName() != TS("IOERenderableObject")) return false;

		IOERenderableObject* pRenderableObject = (IOERenderableObject*)pObject;
		pRenderableObject->Render(fDetailTime);
	}

	int nNumChildNodes = pNode->GetNumChildNodes();
	for (int i = 0; i < nNumChildNodes; ++i)
	{
		bool bOK = RenderNodes(pNode->GetChildNode(i), fDetailTime);
		if (!bOK)
		{
			// TODO: logout
		}
	}

	return true;
}

bool COECore_Impl::OnStartPerform(COEMsgCommand& msg)
{
	// TODO: 
	return true;
}

bool COECore_Impl::OnPreUpdate(COEMsgCommand& msg)
{
	// dispatch message
	g_pOEMsgMgr->ReceiveMessage();
	return true;
}

bool COECore_Impl::OnUpdate(COEMsgCommand& msg)
{
	UpdateNodes(m_pRootNode, g_pOEDevice->GetDetailTime());
	g_pOEApp->Update(g_pOEDevice->GetDetailTime());

	return true;
}

bool COECore_Impl::OnPostUpdate(COEMsgCommand& msg)
{
	// TODO: 
	return true;
}

bool COECore_Impl::OnPreRender(COEMsgCommand& msg)
{
	// TODO: 
	return true;
}

bool COECore_Impl::OnRender(COEMsgCommand& msg)
{
	RenderNodes(m_pRootNode, g_pOEDevice->GetDetailTime());
	g_pOEApp->Render(g_pOEDevice->GetDetailTime());

	return true;
}

bool COECore_Impl::OnPostRender(COEMsgCommand& msg)
{
	// TODO: 
	return true;
}
