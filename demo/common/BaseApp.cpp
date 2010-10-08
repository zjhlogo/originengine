/*!
 * \file BaseApp.cpp
 * \date 8-18-2010 10:50:41
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "BaseApp.h"
#include <libOEMsg/OEMsgList.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/IOECore.h>
#include <OEUI/IOEUIRenderSystem.h>
#include <assert.h>

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
	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
	m_bFirstTimeUpdate = true;
}

void CBaseApp::Destroy()
{
	// TODO: 
}

bool CBaseApp::Initialize()
{
	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	m_pFPS = new CFPSWindow(g_pOEUIRenderSystem->GetScreen());
	if (!m_pFPS) return false;

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_DOWN, this, (MSG_FUNC)&CBaseApp::OnLButtonDown);
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_UP, this, (MSG_FUNC)&CBaseApp::OnLButtonUp);
	g_pOEMsgMgr->RegisterMessage(OMI_MOUSE_MOVE, this, (MSG_FUNC)&CBaseApp::OnMouseMove);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_DOWN, this, (MSG_FUNC)&CBaseApp::OnKeyDown);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_UP, this, (MSG_FUNC)&CBaseApp::OnKeyUp);

	return true;
}

void CBaseApp::Terminate()
{
	SAFE_DELETE(m_pCamera);
}

void CBaseApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov || m_bFirstTimeUpdate) g_pOERenderSystem->SetTransform(TT_VIEW, m_pCamera->GetViewMatrix());
	m_bFirstTimeUpdate = false;
}

bool CBaseApp::OnLButtonDown(COEMsgMouse& msg)
{
	m_bLButtonDown = true;
	return true;
}

bool CBaseApp::OnLButtonUp(COEMsgMouse& msg)
{
	m_bLButtonDown = false;
	return true;
}

bool CBaseApp::OnMouseMove(COEMsgMouse& msg)
{
	if (!m_bLButtonDown) return true;
	m_nMouseDetailX = msg.GetPosX();
	m_nMouseDetailY = msg.GetPosY();
	return true;
}

bool CBaseApp::OnKeyUp(COEMsgKeyboard& msg)
{
	uint nKeyCode = msg.GetKeyCode();
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);

	m_KeyDown[nKeyCode] = false;
	return true;
}

bool CBaseApp::OnKeyDown(COEMsgKeyboard& msg)
{
	uint nKeyCode = msg.GetKeyCode();
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);

	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE

	return true;
}

bool CBaseApp::UpdateMovement(float fDetailTime)
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

bool CBaseApp::UpdateRotation(float fDetailTime)
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
