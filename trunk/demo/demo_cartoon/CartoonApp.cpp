/*!
 * \file CartoonApp.cpp
 * \date 11-2-2010 22:02:32
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "CartoonApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/IOEResMgr.h>
#include <libOEMsg/OEMsgList.h>

IMPLEMENT_OEAPP(CCartoonApp);

CCartoonApp::CCartoonApp()
{
	Init();
}

CCartoonApp::~CCartoonApp()
{
	Destroy();
}

void CCartoonApp::Init()
{
	m_pModel = NULL;
}

void CCartoonApp::Destroy()
{
	// TODO: 
}

bool CCartoonApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("demo_cartoon.xml"));
	if (!m_pModel) return false;

	IOENode* pRootNode = g_pOECore->GetRootNode();
	if (!pRootNode) return false;

	IOENode* pNodeLight = pRootNode->GetChildNode(TS("Light"));
	pNodeLight->SetPosition(CVector3(300.0f, 0.0f, -300.0f));

	pRootNode->AttachObject(m_pModel);

	ResetCameraPosRot(m_pModel);
	return true;
}

void CCartoonApp::UserDataTerm()
{
	SAFE_RELEASE(m_pModel);
}

void CCartoonApp::Update(float fDetailTime)
{
	// TODO: 
}
