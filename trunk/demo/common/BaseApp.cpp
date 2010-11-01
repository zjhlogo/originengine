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
	m_pCamera->SetTargetNode(pNode);

	if (!UserDataInit()) return false;

	m_pFPS = new CFPSWindow(g_pOEUIRenderSystem->GetScreen());
	if (!m_pFPS) return false;

	return true;
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
	IOEMesh* pMesh = pModel->GetRenderData()->GetMesh(TS("MainMesh"));
	m_pCamera->InitFromBBox(pMesh->GetBoundingBoxMin(), pMesh->GetBoundingBoxMax());
}

void CBaseApp::ResetCameraPosRot(const CVector3& vPos, float fRotY, float fRotX)
{
	m_pCamera->MoveTo(vPos);
	m_pCamera->RotateTo(fRotY, fRotX);
}

void CBaseApp::ResetCameraPosRot(const CVector3& vPos, const CQuaternion& qRot)
{
	m_pCamera->MoveTo(vPos);

	float x, y, z;
	COEMath::GetEulerAngle(x, y, z, qRot);
	m_pCamera->RotateTo(y, x);
}

void CBaseApp::SetMovementSpeed(float fSpeed)
{
	m_pCamera->SetMovementSpeed(fSpeed);
}

float CBaseApp::GetMovementSpeed()
{
	return m_pCamera->GetMovementSpeed();
}

void CBaseApp::SetRotationSpeed(float fSpeed)
{
	m_pCamera->SetRotationSpeed(fSpeed);
}

float CBaseApp::GetRotationSpeed()
{
	return m_pCamera->GetRotationSpeed();
}

void CBaseApp::ShowFPS(bool bShow)
{
	m_pFPS->ShowFPS(bShow);
}

void CBaseApp::ShowCameraPosRot(bool bShow)
{
	m_pFPS->ShowCameraPosRot(bShow);
}
