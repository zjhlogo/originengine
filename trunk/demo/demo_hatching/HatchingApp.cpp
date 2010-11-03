/*!
 * \file HatchingApp.cpp
 * \date 11-2-2010 20:11:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "HatchingApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/IOEResMgr.h>
#include <libOEMsg/OEMsgList.h>

IMPLEMENT_OEAPP(CHatchingApp);

CHatchingApp::CHatchingApp()
{
	Init();
}

CHatchingApp::~CHatchingApp()
{
	Destroy();
}

void CHatchingApp::Init()
{
	m_pModel = NULL;
}

void CHatchingApp::Destroy()
{
	// TODO: 
}

bool CHatchingApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("demo_hatching.xml"));
	if (!m_pModel) return false;

	IOENode* pRootNode = g_pOECore->GetRootNode();
	if (!pRootNode) return false;

	IOENode* pNodeLight = pRootNode->GetChildNode(TS("Light"));
	pNodeLight->SetPosition(CVector3(300.0f, 0.0f, -300.0f));

	pRootNode->AttachObject(m_pModel);

	m_pModel->RegisterEvent(OMI_SETUP_SHADER_PARAM, this, (MSG_FUNC)&CHatchingApp::OnSetupShaderParam);
	ResetCameraPosRot(m_pModel);
	return true;
}

void CHatchingApp::UserDataTerm()
{
	SAFE_RELEASE(m_pModel);
}

void CHatchingApp::Update(float fDetailTime)
{
	// TODO: 
}

bool CHatchingApp::OnSetupShaderParam(COEMsgShaderParam& msg)
{
	IOEShader* pShader = msg.GetShader();
	IOEMaterial* pMaterial = msg.GetMaterial();

	pShader->SetTexture(TS("g_texHatchLevel"), pMaterial->GetTexture(MTT_TEXTURE2));
	return true;
}
