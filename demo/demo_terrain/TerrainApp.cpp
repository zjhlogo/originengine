/*!
 * \file TerrainApp.cpp
 * \date 24-5-2009 0:11:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "TerrainApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOERenderSystem.h>
#include <OECore/IOECore.h>
#include <OEBase/IOEMsgMgr.h>
#include <libOEBase/OEMsgID.h>
#include <assert.h>

IMPLEMENT_OEAPP(CTerrainApp);

CTerrainApp::CTerrainApp()
{
	Init();
}

CTerrainApp::~CTerrainApp()
{
	Destroy();
}

void CTerrainApp::Init()
{
	//m_pDecl = NULL;
	m_pCamera = NULL;
	m_pTerrainMgr = NULL;
	//m_pShader = NULL;
	//m_pTexture = NULL;

	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
}

void CTerrainApp::Destroy()
{
	// TODO: 
}

bool CTerrainApp::Initialize()
{
	m_pCamera = new CCamera();
	m_pTerrainMgr = new CTerrainMgr();
	if (!m_pTerrainMgr->LoadTerrain()) return false;

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_DOWN, this, (MSG_FUNC)&CTerrainApp::OnLButtonDown);
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_UP, this, (MSG_FUNC)&CTerrainApp::OnLButtonUp);
	g_pOEMsgMgr->RegisterMessage(OMI_MOUSE_MOVE, this, (MSG_FUNC)&CTerrainApp::OnMouseMove);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_DOWN, this, (MSG_FUNC)&CTerrainApp::OnKeyDown);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_UP, this, (MSG_FUNC)&CTerrainApp::OnKeyUp);

	return true;
}

void CTerrainApp::Terminate()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pTerrainMgr);
}

void CTerrainApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderSystem->SetTransform(TT_VIEW, m_pCamera->GetViewMatrix());
	m_pTerrainMgr->UpdateTerrain(m_pCamera->GetEyePos());
}

void CTerrainApp::Render(float fDetailTime)
{
	m_pTerrainMgr->Render(fDetailTime);
}

bool CTerrainApp::OnLButtonDown(COEMsgMouse& msg)
{
	m_bLButtonDown = true;
	return true;
}

bool CTerrainApp::OnLButtonUp(COEMsgMouse& msg)
{
	m_bLButtonDown = false;
	return true;
}

bool CTerrainApp::OnMouseMove(COEMsgMouse& msg)
{
	if (!m_bLButtonDown) return true;
	m_nMouseDetailX = msg.GetPosX();
	m_nMouseDetailY = msg.GetPosY();
	return true;
}

bool CTerrainApp::OnKeyUp(COEMsgKeyboard& msg)
{
	uint nKeyCode = msg.GetKeyCode();
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);

	m_KeyDown[nKeyCode] = false;
	return true;
}

bool CTerrainApp::OnKeyDown(COEMsgKeyboard& msg)
{
	uint nKeyCode = msg.GetKeyCode();
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);

	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE

	return true;
}

bool CTerrainApp::UpdateMovement(float fDetailTime)
{
	static const float MOVE_DIST = 400.0f;

	bool bUpdateMovement = m_KeyDown['W'] || m_KeyDown['S'] || m_KeyDown['A'] || m_KeyDown['D'];
	if (!bUpdateMovement) return false;

	if (m_KeyDown['W']) m_pCamera->Move(m_pCamera->GetVectorForword(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['S']) m_pCamera->Move(m_pCamera->GetVectorForword(), -MOVE_DIST*fDetailTime);
	if (m_KeyDown['D']) m_pCamera->Move(m_pCamera->GetVectorRight(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['A']) m_pCamera->Move(m_pCamera->GetVectorRight(), -MOVE_DIST*fDetailTime);

	return true;
}

bool CTerrainApp::UpdateRotation(float fDetailTime)
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
