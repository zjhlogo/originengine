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
}

void CModelApp::Destroy()
{
	// TODO: 
}

bool CModelApp::Initialize()
{
	if (!CBaseApp::Initialize()) return false;

	m_pModel = g_pOEResMgr->CreateModel(TS("Model.xml"));
	if (!m_pModel) return false;

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_SETUP_SHADER_PARAM, this, (MSG_FUNC)&CModelApp::OnSetupShaderParam);

	return true;
}

void CModelApp::Terminate()
{
	SAFE_RELEASE(m_pModel);
	CBaseApp::Terminate();
}

void CModelApp::Update(float fDetailTime)
{
	CBaseApp::Update(fDetailTime);
	m_pModel->Update(fDetailTime);
}

void CModelApp::Render(float fDetailTime)
{
	m_pModel->Render();
}

bool CModelApp::OnSetupShaderParam(COEMsgShaderParam& msg)
{
	IOEShader* pShader = msg.GetShader();

	pShader->SetVector(TS("g_vLightPos"), CVector3(0.0f, 0.0f, -300.0f));
	pShader->SetVector(TS("g_vEyePos"), m_pCamera->GetEyePos());

	return true;
}
