/*!
 * \file ModelApp.cpp
 * \date 1-3-2010 21:43:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ModelApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOERenderSystem.h>
#include <OECore/IOECore.h>
#include <OECore/IOEResMgr.h>
#include <OEBase/IOEMsgMgr.h>
#include <libOEBase/OEMsgID.h>
#include <assert.h>

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
	m_pCamera = NULL;
	m_pModel = NULL;
	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
}

void CModelApp::Destroy()
{
	// TODO: 
}

bool CModelApp::Initialize()
{
	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	m_pModel = g_pOEResMgr->CreateModel(TS("Model.xml"));
	if (!m_pModel) return false;

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_DOWN, this, (MSG_FUNC)&CModelApp::OnLButtonDown);
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_UP, this, (MSG_FUNC)&CModelApp::OnLButtonUp);
	g_pOEMsgMgr->RegisterMessage(OMI_MOUSE_MOVE, this, (MSG_FUNC)&CModelApp::OnMouseMove);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_DOWN, this, (MSG_FUNC)&CModelApp::OnKeyDown);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_UP, this, (MSG_FUNC)&CModelApp::OnKeyUp);
	g_pOEMsgMgr->RegisterMessage(OMI_SETUP_SHADER_PARAM, this, (MSG_FUNC)&CModelApp::OnSetupShaderParam);

	return true;
}

void CModelApp::Terminate()
{
	SAFE_RELEASE(m_pModel);
	SAFE_DELETE(m_pCamera);
}

void CModelApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderSystem->SetTransform(TT_VIEW, m_pCamera->GetViewMatrix());

	m_pModel->Update(fDetailTime);
}

void CModelApp::Render(float fDetailTime)
{
	m_pModel->Render();
}

bool CModelApp::OnLButtonDown(COEMsgMouse& msg)
{
	m_bLButtonDown = true;
	return true;
}

bool CModelApp::OnLButtonUp(COEMsgMouse& msg)
{
	m_bLButtonDown = false;
	return true;
}

bool CModelApp::OnMouseMove(COEMsgMouse& msg)
{
	if (!m_bLButtonDown) return true;
	m_nMouseDetailX = msg.GetPosX();
	m_nMouseDetailY = msg.GetPosY();
	return true;
}

bool CModelApp::OnKeyUp(COEMsgKeyboard& msg)
{
	uint nKeyCode = msg.GetKeyCode();
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);

	m_KeyDown[nKeyCode] = false;
	return true;
}

bool CModelApp::OnKeyDown(COEMsgKeyboard& msg)
{
	uint nKeyCode = msg.GetKeyCode();
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);

	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE

	return true;
}

bool CModelApp::OnSetupShaderParam(COEMsgShaderParam& msg)
{
	IOEShader* pShader = msg.GetShader();

	pShader->SetVector(TS("g_vLightPos"), CVector3(0.0f, 0.0f, -300.0f));
	pShader->SetVector(TS("g_vEyePos"), m_pCamera->GetEyePos());

	return true;
}

bool CModelApp::UpdateMovement(float fDetailTime)
{
	static const float MOVE_DIST = 50.0f;

	bool bUpdateMovement = m_KeyDown['W'] || m_KeyDown['S'] || m_KeyDown['A'] || m_KeyDown['D'];
	if (!bUpdateMovement) return false;

	if (m_KeyDown['W']) m_pCamera->Move(m_pCamera->GetVectorForword(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['S']) m_pCamera->Move(m_pCamera->GetVectorForword(), -MOVE_DIST*fDetailTime);
	if (m_KeyDown['D']) m_pCamera->Move(m_pCamera->GetVectorRight(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['A']) m_pCamera->Move(m_pCamera->GetVectorRight(), -MOVE_DIST*fDetailTime);

	return true;
}

bool CModelApp::UpdateRotation(float fDetailTime)
{
	static const float ROTATE_ADJUST = 0.3f;

	if (!m_bLButtonDown) return false;
	if (m_nMouseDetailX == 0 && m_nMouseDetailY == 0) return false;

	if (m_nMouseDetailX) m_pCamera->Rotate(m_pCamera->GetVectorUp(), m_nMouseDetailX*fDetailTime*ROTATE_ADJUST);
	if (m_nMouseDetailY) m_pCamera->Rotate(m_pCamera->GetVectorRight(), m_nMouseDetailY*fDetailTime*ROTATE_ADJUST);

	m_nMouseDetailY = 0;
	m_nMouseDetailX = 0;

	return true;
}
