/*!
 * \file ModelApp.cpp
 * \date 1-3-2010 21:43:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ModelApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOEResMgr.h>
#include <OECore/IOECore.h>
#include <OEBase/IOEMsgMgr.h>
#include <libOEMsg/OEMsgList.h>

IMPLEMENT_OEAPP(CModelApp);

CModelApp::CModelApp()
{
	Init();
}

CModelApp::~CModelApp()
{
	Destroy();
}

void CModelApp::Init()
{
	m_pModel = NULL;
	m_pNodeRoot = NULL;
	m_pNode1 = NULL;
	m_pNode2 = NULL;
}

void CModelApp::Destroy()
{
	// TODO: 
}

bool CModelApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("casual03.xml"));
	if (!m_pModel) return false;

	m_pNodeRoot = g_pOECore->GetRootNode();

	m_pNode1 = m_pNodeRoot->NewChildNode(TS("Node1"));
	m_pNode1->SetPosition(CVector3(-100.0f, 0.0f, 0.0f));
	m_pNode1->AttachObject(m_pModel);

	m_pNode2 = m_pNodeRoot->NewChildNode(TS("Node2"));
	m_pNode2->SetPosition(CVector3(100.0f, 0.0f, 0.0f));
	m_pNode2->AttachObject(m_pModel);

	CQuaternion qRot(COEMath::VECTOR_UP, COEMath::PI_2);
	//pRootNode->SetRotation(qRot);

	ResetCameraPosRot(m_pModel);

	return true;
}

void CModelApp::UserDataTerm()
{
	SAFE_RELEASE(m_pModel);
}

void CModelApp::Update(float fDetailTime)
{
	static float s_fTotalTime = 0.0f;

	s_fTotalTime += (0.3f*fDetailTime);

	CQuaternion qRotRoot(COEMath::VECTOR_UP, s_fTotalTime);
	m_pNodeRoot->SetRotation(qRotRoot);

	CQuaternion qRotNode1(COEMath::VECTOR_UP, -2.0f*s_fTotalTime);
	m_pNode1->SetRotation(qRotNode1);

	m_pNode2->SetRotation(qRotRoot);
}
