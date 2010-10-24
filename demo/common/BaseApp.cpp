/*!
 * \file BaseApp.cpp
 * \date 8-18-2010 10:50:41
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "BaseApp.h"
#include <OECore/IOECore.h>
#include <OEUI/IOEUIRenderSystem.h>

CBaseApp::CBaseApp()
{
	Init();
}

CBaseApp::~CBaseApp()
{
	Destroy();
}

void CBaseApp::Init()
{
	m_pCamera = NULL;
	m_pFPS = NULL;
}

void CBaseApp::Destroy()
{
	// TODO: 
}

bool CBaseApp::Initialize()
{
	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	IOENode* pNode = g_pOECore->GetRootNode()->NewChildNode(TS("Camera"));
	if (!pNode) return false;

	pNode->AttachObject(m_pCamera);
	m_pCamera->SetTargetNode(pNode);

	m_pFPS = new CFPSWindow(g_pOEUIRenderSystem->GetScreen());
	if (!m_pFPS) return false;

	return UserDataInit();
}

void CBaseApp::Terminate()
{
	UserDataTerm();
	SAFE_DELETE(m_pCamera);
}

void CBaseApp::PreUpdate(float fDetailTime)
{
	m_pCamera->Update(fDetailTime);
}

void CBaseApp::PostUpdate(float fDetailTime)
{
	// TODO: 
}

void CBaseApp::ResetCameraPosRot(IOEModel* pModel)
{
	IOEMesh* pMesh = pModel->GetRenderData()->GetMesh();
	m_pCamera->InitFromBBox(pMesh->GetBoundingBoxMin(), pMesh->GetBoundingBoxMax());
}

void CBaseApp::ResetCameraPosRot(const CVector3& vPos, float fRotY, float fRotX)
{
	m_pCamera->MoveTo(vPos);
	m_pCamera->RotateTo(fRotY, fRotX);
}
