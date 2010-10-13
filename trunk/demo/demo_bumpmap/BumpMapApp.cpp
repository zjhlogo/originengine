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
}

void CBumpMapApp::Destroy()
{
	// TODO: 
}

bool CBumpMapApp::Initialize()
{
	if (!CBaseApp::Initialize()) return false;

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape || !m_pSimpleShape->IsOK()) return false;

	m_pModel = g_pOEResMgr->CreateModel(TS("demo_bumpmap.xml"));
	if (!m_pModel) return false;

	IOEMesh* pMesh = m_pModel->GetRenderData()->GetMesh();
	m_pCamera->InitFromBBox(pMesh->GetBoundingBoxMin(), pMesh->GetBoundingBoxMax());

	m_pModel->RegisterEvent(OMI_SETUP_SHADER_PARAM, this, (MSG_FUNC)&CBumpMapApp::OnSetupShaderParam);

	g_pOECore->GetRootNode()->AttachObject(m_pSimpleShape);
	g_pOECore->GetRootNode()->AttachObject(m_pModel);

	return true;
}

void CBumpMapApp::Terminate()
{
	SAFE_DELETE(m_pModel);
	SAFE_DELETE(m_pSimpleShape);
	CBaseApp::Terminate();
}

void CBumpMapApp::Update(float fDetailTime)
{
	static float s_fTotalTime = 0.0f;

	CBaseApp::Update(fDetailTime);

	s_fTotalTime += fDetailTime*0.3f;
	m_vLightPos.x = cos(s_fTotalTime)*40.0f;
	m_vLightPos.z = sin(s_fTotalTime)*40.0f;
	m_vLightPos.y = cos(s_fTotalTime)*sin(s_fTotalTime)*40.0f;

	m_pSimpleShape->SetPosition(m_vLightPos);
}

bool CBumpMapApp::OnKeyDown(COEMsgKeyboard& msg)
{
	CBaseApp::OnKeyDown(msg);

	IOEShader* pShader = m_pModel->GetRenderData()->GetMaterial(0)->GetShader();

	if (m_KeyDown['1']) pShader->SetTechnique(TS("NormalMap"));
	if (m_KeyDown['2']) pShader->SetTechnique(TS("ParallaxMap"));
	if (m_KeyDown['3']) pShader->SetTechnique(TS("DiffuseTexture"));
	if (m_KeyDown['4']) pShader->SetTechnique(TS("NormalTexture"));
	if (m_KeyDown['5']) pShader->SetTechnique(TS("HeightMapTexture"));

	return true;
}

bool CBumpMapApp::OnSetupShaderParam(COEMsgShaderParam& msg)
{
	IOEShader* pShader = msg.GetShader();

	pShader->SetVector(TS("g_vLightPos"), m_vLightPos);
	pShader->SetVector(TS("g_vEyePos"), m_pCamera->GetEyePos());

	IOETexture* pTexture = m_pModel->GetRenderData()->GetMaterial(0)->GetTexture(MTT_NORMAL);
	pShader->SetTexture(TS("g_texNormalHeight"), pTexture);

	return true;
}
