/*!
 * \file BumpMapApp.cpp
 * \date 6-7-2009 9:23:27
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "BumpMapApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>
#include <OECore/IOEDevice.h>
#include <OECore/IOEResMgr.h>
#include <libOEMsg/OEMsgList.h>

IMPLEMENT_OEAPP(CBumpMapApp);

CBumpMapApp::CBumpMapApp()
{
	Init();
}

CBumpMapApp::~CBumpMapApp()
{
	Destroy();
}

void CBumpMapApp::Init()
{
	m_pSimpleShape = NULL;
	m_pModel = NULL;
	m_bTimeStop = true;
}

void CBumpMapApp::Destroy()
{
	// TODO: 
}

bool CBumpMapApp::UserDataInit()
{
	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape || !m_pSimpleShape->IsOK()) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pSimpleShape);

	m_pModel = g_pOEResMgr->CreateModel(TS("demo_bumpmap.xml"));
	if (!m_pModel) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pModel);

	ResetCameraPosRot(m_pModel);

	g_pOEDevice->RegisterEvent(OMI_KEY_DOWN, this, (MSG_FUNC)&CBumpMapApp::OnKeyDown);
	m_pModel->RegisterEvent(OMI_SETUP_SHADER_PARAM, this, (MSG_FUNC)&CBumpMapApp::OnSetupShaderParam);

	return true;
}

void CBumpMapApp::UserDataTerm()
{
	SAFE_DELETE(m_pModel);
	SAFE_DELETE(m_pSimpleShape);
}

void CBumpMapApp::Update(float fDetailTime)
{
	static float s_fTotalTime = 11.0f;

	if (!m_bTimeStop) s_fTotalTime += fDetailTime*0.3f;
	m_vLightPos.x = cos(s_fTotalTime)*40.0f;
	m_vLightPos.z = sin(s_fTotalTime)*40.0f;
	m_vLightPos.y = cos(s_fTotalTime)*sin(s_fTotalTime)*40.0f;

	g_pOECore->GetRootNode()->GetChildNode(TS("Light"))->SetPosition(m_vLightPos);
	m_pSimpleShape->SetPosition(m_vLightPos);
}

bool CBumpMapApp::OnKeyDown(COEMsgKeyboard& msg)
{
	IOERenderData* pRenderData = m_pModel->GetRenderData();
	IOEMaterialsList* pMaterialsList = pRenderData->GetMaterialsList(TS("MainMaterialsList"));
	IOEMaterial* pMaterial = pMaterialsList->GetMaterial(0);
	IOEShader* pShader = pMaterial->GetShader();

	switch (msg.GetKeyCode())
	{
	case '1':
		pShader->SetTechnique(TS("NormalMap"));
		break;
	case '2':
		pShader->SetTechnique(TS("ParallaxMap"));
		break;
	case '3':
		pShader->SetTechnique(TS("ConeMap"));
		break;
	case '4':
		pShader->SetTechnique(TS("DiffuseTexture"));
		break;
	case '5':
		pShader->SetTechnique(TS("NormalTexture"));
		break;
	case '6':
		pShader->SetTechnique(TS("HeightMapTexture"));
		break;
	case ' ':
		m_bTimeStop = !m_bTimeStop;
		break;
	}

	return true;
}

bool CBumpMapApp::OnSetupShaderParam(COEMsgShaderParam& msg)
{
	IOEShader* pShader = msg.GetShader();
	IOEMaterial* pMaterial = msg.GetMaterial();

	IOENode* pCameraNode = g_pOECore->GetRootNode()->GetChildNode(TS("Camera"));
	if (!pCameraNode) return false;

	pShader->SetVector(TS("g_vEyePos"), pCameraNode->GetPosition());
	pShader->SetTexture(TS("g_texConeMap"), pMaterial->GetTexture(MTT_TEXTURE2));

	return true;
}
