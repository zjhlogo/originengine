/*!
 * \file TestApp.cpp
 * \date 24-5-2009 0:11:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "TestApp.h"

#include <OEInterfaces.h>
#include <assert.h>

CTestApp::CTestApp()
{
	Init();
}

CTestApp::~CTestApp()
{
	Destroy();
}

void CTestApp::Init()
{
	m_pDecl = NULL;
	m_pCamera = NULL;
	m_pTerrainMgr = NULL;
	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
}

void CTestApp::Destroy()
{
	// TODO: 
}

bool CTestApp::Initialize()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
		IOEVertDecl::T_COLOR, IOEVertDecl::U_COLOR, 0,
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	if (!m_pDecl) return false;

	m_pCamera = new CCamera();
	m_pTerrainMgr = new CTerrainMgr();
	if (!m_pTerrainMgr->LoadTerrain()) return false;

	g_pOERenderer->EnableFog(true);
	g_pOERenderer->SetFogInfo(0xFF000000, 200.0f, 400.0f);
	g_pOERenderer->SetFillMode(IOERenderer::FM_WIREFRAME);

	return true;
}

void CTestApp::Terminate()
{
	SAFE_RELEASE(m_pDecl);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pTerrainMgr);
}

void CTestApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderer->SetTransform(IOERenderer::TT_VIEW, m_pCamera->GetViewMatrix());
	m_pTerrainMgr->UpdateTerrain(m_pCamera->GetEyePos());
}

void CTestApp::Render(float fDetailTime)
{
	//static const VERTEX s_Verts[4] =
	//{
	//	{-10.0f, 0.0f, -10.0f, 0xFFFF0000},
	//	{-10.0f, 0.0f, 10.0f, 0xFF00FF00},
	//	{10.0f, 0.0f, 10.0f, 0xFF0000FF},
	//	{10.0f, 0.0f, -10.0f, 0xFFFFFFFF},
	//};

	//static const ushort s_Indis[6] = {0, 1, 3, 1, 2, 3};

	//g_pOERenderer->SetFillMode(IOERenderer::FM_SOLID);
	//g_pOERenderer->SetVertDecl(m_pDecl);
	//g_pOERenderer->DrawTriList(s_Verts, 4, s_Indis, 6);
	m_pTerrainMgr->Render(fDetailTime);
}

void CTestApp::OnLButtonDown(int x, int y)
{
	m_bLButtonDown = true;
}

void CTestApp::OnLButtonUp(int x, int y)
{
	m_bLButtonDown = false;
}

void CTestApp::OnMouseMove(int dx, int dy)
{
	if (!m_bLButtonDown) return;
	m_nMouseDetailX = dx;
	m_nMouseDetailY = dy;
}

void CTestApp::OnKeyUp(int nKeyCode)
{
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = false;
}

void CTestApp::OnKeyDown(int nKeyCode)
{
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE
}

bool CTestApp::UpdateMovement(float fDetailTime)
{
	static const float MOVE_DIST = 100.0f;

	bool bUpdateMovement = m_KeyDown['W'] || m_KeyDown['S'] || m_KeyDown['A'] || m_KeyDown['D'];
	if (!bUpdateMovement) return false;

	if (m_KeyDown['W']) m_pCamera->Move(m_pCamera->GetVectorForword(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['S']) m_pCamera->Move(m_pCamera->GetVectorForword(), -MOVE_DIST*fDetailTime);
	if (m_KeyDown['D']) m_pCamera->Move(m_pCamera->GetVectorRight(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['A']) m_pCamera->Move(m_pCamera->GetVectorRight(), -MOVE_DIST*fDetailTime);

	return true;
}

bool CTestApp::UpdateRotation(float fDetailTime)
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