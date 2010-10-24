/*!
 * \file MeshApp.cpp
 * \date 31-7-2009 20:46:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/IOEResMgr.h>
#include <libOEMsg/OEMsgList.h>

IMPLEMENT_OEAPP(CMeshApp);

CMeshApp::CMeshApp()
{
	Init();
}

CMeshApp::~CMeshApp()
{
	Destroy();
}

void CMeshApp::Init()
{
	m_pModel = NULL;
}

void CMeshApp::Destroy()
{
	// TODO: 
}

bool CMeshApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("sky.xml"));
	if (!m_pModel) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pModel);
	m_pModel->RegisterEvent(OMI_SETUP_SHADER_PARAM, this, (MSG_FUNC)&CMeshApp::OnSetupShaderParam);

	ResetCameraPosRot(m_pModel);
	return true;
}

void CMeshApp::UserDataTerm()
{
	SAFE_DELETE(m_pModel);
}

void CMeshApp::Update(float fDetailTime)
{
	m_vOffset.x += fDetailTime*0.01f;
	m_vOffset.y += fDetailTime*0.01f;

	if (m_vOffset.x > 1.0f) m_vOffset.x -= 1.0f;
	if (m_vOffset.y > 1.0f) m_vOffset.y -= 1.0f;
}

bool CMeshApp::OnSetupShaderParam(COEMsgShaderParam& msg)
{
	IOEShader* pShader = msg.GetShader();

	IOENode* pCameraNode = g_pOECore->GetRootNode()->GetChildNode(TS("Camera"));
	if (!pCameraNode) return false;

	const CVector3& vEyePos = pCameraNode->GetPosition();

	CMatrix4x4 matWorld;
	COEMath::SetMatrixTranslation(matWorld, vEyePos);

	CMatrix4x4 matWorldToProject;
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_VIEW_PROJ);
	matWorldToProject = matWorld * matWorldToProject;
	pShader->SetMatrix(TS("g_matWorldToProject2"), matWorldToProject);

	pShader->SetVector(TS("g_texOffset"), m_vOffset);

	return true;
}
