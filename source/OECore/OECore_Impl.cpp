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
#include <OEUI/IOEUIResMgr.h>
#include <OEUI/IOEUIRendererMgr.h>

#include <libOEBase/IOEApp.h>
#include <libOEMsg/OEMsgList.h>

#include "OENode_Impl.h"

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
	m_pRootNode = NULL;
	m_bRendering = false;
	m_bRenderingSceneToTexture = false;
	return true;
}

void COECore_Impl::Destroy()
{
	// TODO: 
}

bool COECore_Impl::Initialize()
{
	// we registe message first to make sure the message comes to here first
	RegisterMessage();

	if (!InitializeInterfaces())
	{
		LOGOUT(TS("IOECore::Initialize Failed"));
		return false;
	}

	m_pRootNode = new COENode_Impl(TS("RootNode"));
	if (!m_pRootNode) return false;
	m_pRootNode->NewChildNode(TS("Camera"));
	m_pRootNode->NewChildNode(TS("Light"));

	// Ӧ�ó���ӿ���
	if (!g_pOEApp->Initialize()) return false;

	LOGOUT(TS("IOECore::Initialize OK"));
	return true;
}

void COECore_Impl::Terminate()
{
	End();

	// Ӧ�ó���ӿ���
	g_pOEApp->Terminate();

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

IOENode* COECore_Impl::GetRootNode()
{
	return m_pRootNode;
}

bool COECore_Impl::RenderSceneToTexture(IOETexture* pTexture)
{
	if (!m_bRendering) return false;
	if (m_bRenderingSceneToTexture) return false;

	if (!g_pOERenderSystem->BeginRenderTarget(pTexture)) return false;

	m_bRenderingSceneToTexture = true;
	RenderObjects(g_pOEDevice->GetDetailTime());
	m_bRenderingSceneToTexture = false;

	g_pOERenderSystem->EndRenderTarget();
	return true;
}

bool COECore_Impl::IsRenderingSceneToTexture()
{
	return m_bRenderingSceneToTexture;
}

bool COECore_Impl::InitializeInterfaces()
{
	// �ļ�������
	if (!g_pOEFileMgr->Initialize()) return false;

	// ��־������
	if (!g_pOELogFileMgr->Initialize()) return false;

	// Xml ������
	if (!g_pOEXmlMgr->Initialize()) return false;

	// ��Ϣ������
	if (!g_pOEMsgMgr->Initialize()) return false;
	if (!g_pOEMsgMgr->AddMsgDB(g_OEMsgListDB)) return false;

	// �����ļ�������
	if (!g_pOEConfigFileMgr->Initialize()) return false;

	// ��Ⱦ�豸������
	if (!g_pOEDevice->Initialize()) return false;

	// ��Ⱦ�ӿڹ�����
	if (!g_pOERenderSystem->Initialize()) return false;

	// ���������
	if (!g_pOETextureMgr->Initialize()) return false;

	// Shader ������
	if (!g_pOEShaderMgr->Initialize()) return false;

	// ������Դ����
	if (!g_pOEResMgr->Initialize()) return false;

	// ģ�Ϳ�����������
	if (!g_pOEControlMgr->Initialize()) return false;

	// ģ����Ⱦ��������
	if (!g_pOERenderMgr->Initialize()) return false;

	// UI ��Ⱦ������
	if (!g_pOEUIRenderSystem->Initialize()) return false;

	// UI ���������
	if (!g_pOEUIResMgr->Initialize()) return false;

	// UI �ַ���������
	if (!g_pOEUIRendererMgr->Initialize()) return false;

	return true;
}

void COECore_Impl::TerminateInterfaces()
{
	// UI �ַ���������
	g_pOEUIRendererMgr->Terminate();

	// UI ���������
	g_pOEUIResMgr->Terminate();

	// UI ��Ⱦ������
	g_pOEUIRenderSystem->Terminate();

	// ģ����Ⱦ��������
	g_pOERenderMgr->Terminate();

	// ģ�Ϳ�����������
	g_pOEControlMgr->Terminate();

	// ������Դ����
	g_pOEResMgr->Terminate();

	// Shader ������
	g_pOEShaderMgr->Terminate();

	// ���������
	g_pOETextureMgr->Terminate();

	// ��Ⱦ�ӿڹ�����
	g_pOERenderSystem->Terminate();

	// ��Ⱦ�豸������
	g_pOEDevice->Terminate();

	// �����ļ�������
	g_pOEConfigFileMgr->Terminate();

	// ��Ϣ������
	g_pOEMsgMgr->Terminate();

	// Xml ������
	g_pOEXmlMgr->Terminate();

	// ��־������
	g_pOELogFileMgr->Terminate();

	// �ļ�������
	g_pOEFileMgr->Terminate();
}

void COECore_Impl::RegisterMessage()
{
	g_pOEDevice->RegisterEvent(OMI_START_PERFORM, this, (MSG_FUNC)&COECore_Impl::OnStartPerform);
	g_pOEDevice->RegisterEvent(OMI_PRE_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnPreUpdate);
	g_pOEDevice->RegisterEvent(OMI_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnUpdate);
	g_pOEDevice->RegisterEvent(OMI_POST_UPDATE, this, (MSG_FUNC)&COECore_Impl::OnPostUpdate);
	g_pOEDevice->RegisterEvent(OMI_PRE_RENDER, this, (MSG_FUNC)&COECore_Impl::OnPreRender);
	g_pOEDevice->RegisterEvent(OMI_RENDER, this, (MSG_FUNC)&COECore_Impl::OnRender);
	g_pOEDevice->RegisterEvent(OMI_POST_RENDER, this, (MSG_FUNC)&COECore_Impl::OnPostRender);
}

void COECore_Impl::UpdateNodes(IOENode* pNode, bool bParentDirty, const CMatrix4x4& matParent)
{
	if (!pNode) return;

	if (bParentDirty || pNode->IsDirty())
	{
		pNode->Update(matParent);
		pNode->ClearDirty();
		bParentDirty = true;
	}

	const CMatrix4x4& matFinal = pNode->GetFinalMatrix();

	int nNumChildNodes = pNode->GetNumChildNodes();
	for (int i = 0; i < nNumChildNodes; ++i)
	{
		UpdateNodes(pNode->GetChildNode(i), bParentDirty, matFinal);
	}
}

void COECore_Impl::ClearObjectList()
{
	m_sRenderableObjects.clear();
	m_vRenderObjectInfo.clear();
}

void COECore_Impl::QueryObjects(IOENode* pNode)
{
	if (!pNode) return;

	int nNumAttachedObjects = pNode->GetNumAttachedObjects();
	for (int i = 0; i < nNumAttachedObjects; ++i)
	{
		IOEObject* pObject = pNode->GetAttachedObject(i);
		if (!pObject) continue;

		const COERtti* pRtti = pObject->GetRtti();
		if (!pRtti) continue;

		if (pRtti->IsDerived(TS("IOERenderableObject")))
		{
			IOERenderableObject* pRenderableObject = (IOERenderableObject*)pObject;
			m_sRenderableObjects.insert(pRenderableObject);

			RENDER_OBJECT_INFO ObjInfo = {pRenderableObject, pNode};
			m_vRenderObjectInfo.push_back(ObjInfo);
		}
		//else if (pRtti->IsDerived(TS("IOECamera")))
		//{
		//	IOECamera* pCamera = (IOECamera*)pObject;
		//	m_sCamera.insert(pCamera);
		//}
	}

	int nNumChildNodes = pNode->GetNumChildNodes();
	for (int i = 0; i < nNumChildNodes; ++i)
	{
		QueryObjects(pNode->GetChildNode(i));
	}
}

void COECore_Impl::UpdateObjects(float fDetailTime)
{
	// update renderable object
	for (TS_RENDERABLE_OBJECT::iterator it = m_sRenderableObjects.begin(); it != m_sRenderableObjects.end(); ++it)
	{
		(*it)->Update(fDetailTime);
	}
}

void COECore_Impl::RenderObjects(float fDetailTime)
{
	for (TV_RENDER_OBJECT_INFO::iterator it = m_vRenderObjectInfo.begin(); it != m_vRenderObjectInfo.end(); ++it)
	{
		RENDER_OBJECT_INFO& ObjInfo = (*it);
		if (!ObjInfo.pObject->IsVisible()) continue;
		IOERenderData* pRenderData = ObjInfo.pObject->GetRenderData();
		if (pRenderData)
		{
			pRenderData->SetObject(TS("AttachedNode"), ObjInfo.pNode);
		}
		ObjInfo.pObject->Render(fDetailTime);
	}
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

	g_pOEApp->PreUpdate(g_pOEDevice->GetDetailTime());
	return true;
}

bool COECore_Impl::OnUpdate(COEMsgCommand& msg)
{
	g_pOEApp->Update(g_pOEDevice->GetDetailTime());

	return true;
}

bool COECore_Impl::OnPostUpdate(COEMsgCommand& msg)
{
	float fDetailTime = g_pOEDevice->GetDetailTime();

	g_pOEApp->PostUpdate(fDetailTime);

	CMatrix4x4 matParent;
	UpdateNodes(m_pRootNode, false, matParent);

	ClearObjectList();
	QueryObjects(m_pRootNode);
	UpdateObjects(fDetailTime);

	return true;
}

bool COECore_Impl::OnPreRender(COEMsgCommand& msg)
{
	m_bRendering = true;
	return true;
}

bool COECore_Impl::OnRender(COEMsgCommand& msg)
{
	RenderObjects(g_pOEDevice->GetDetailTime());

	return true;
}

bool COECore_Impl::OnPostRender(COEMsgCommand& msg)
{
	m_bRendering = false;
	return true;
}
