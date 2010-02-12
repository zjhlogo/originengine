/*!
 * \file TestApp.cpp
 * \date 24-5-2009 0:11:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "TestApp.h"
#include "../common/AppHelper.h"
#include <OEMsgID.h>
#include <assert.h>

IMPLEMENT_OEAPP(CTestApp);

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

void CTestApp::Destroy()
{
	// TODO: 
}

bool CTestApp::Initialize()
{
	//static const IOEVertDecl::ELEMENT s_Decl[] =
	//{
	//	IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
	//	IOEVertDecl::T_FLOAT2, IOEVertDecl::U_TEXCOORD, 0,
	//	IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	//};

	//m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	//if (!m_pDecl) return false;

	m_pCamera = new CCamera();
	m_pTerrainMgr = new CTerrainMgr();
	if (!m_pTerrainMgr->LoadTerrain()) return false;

	//g_pOERenderSystem->EnableFog(true);
	//g_pOERenderSystem->SetFogInfo(0xFF000000, 600.0f, 800.0f);
	////g_pOERenderSystem->SetFillMode(IOERenderSystem::FM_WIREFRAME);

	//m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, _T("terrain.fx"));
	//if (!m_pShader) return false;

	//m_pTexture = g_pOETextureMgr->CreateTextureFromFile(_T("grass.png"));
	//if (!m_pTexture) return false;

	//// 这里用来测试引用机制
	//IOETexture* pTexture1 = g_pOETextureMgr->CreateTextureFromFile(_T("GRASS.PNG"));
	//IOETexture* pTexture2 = g_pOETextureMgr->CreateTextureFromFile(_T("GrAsS.pNg"));
	//IOETexture* pTexture3 = g_pOETextureMgr->CreateTextureFromFile(_T("grass.png"));
	//SAFE_RELEASE(pTexture1);
	//SAFE_RELEASE(pTexture2);
	//SAFE_RELEASE(pTexture3);

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_DOWN, this, (MSG_FUNC)&CTestApp::OnLButtonDown);
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_UP, this, (MSG_FUNC)&CTestApp::OnLButtonUp);
	g_pOEMsgMgr->RegisterMessage(OMI_MOUSE_MOVE, this, (MSG_FUNC)&CTestApp::OnMouseMove);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_DOWN, this, (MSG_FUNC)&CTestApp::OnKeyDown);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_UP, this, (MSG_FUNC)&CTestApp::OnKeyUp);

	return true;
}

void CTestApp::Terminate()
{
	//SAFE_RELEASE(m_pDecl);
	//SAFE_RELEASE(m_pShader);
	//SAFE_RELEASE(m_pTexture);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pTerrainMgr);
}

void CTestApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderSystem->SetTransform(IOERenderSystem::TT_VIEW, m_pCamera->GetViewMatrix());
	m_pTerrainMgr->UpdateTerrain(m_pCamera->GetEyePos());
}

void CTestApp::Render(float fDetailTime)
{
	//static const VERTEX s_Verts[4] =
	//{
	//	{-10.0f, 0.0f, -10.0f, 0.0f, 1.0f},
	//	{-10.0f, 0.0f, 10.0f, 0.0f, 0.0f},
	//	{10.0f, 0.0f, 10.0f, 1.0f, 0.0f},
	//	{10.0f, 0.0f, -10.0f, 1.0f, 1.0f},
	//};

	//static const ushort s_Indis[6] = {0, 1, 3, 1, 2, 3};

	//CMatrix4x4 matView;
	//CMatrix4x4 matProj;

	//g_pOERenderSystem->GetTransform(matView, IOERenderSystem::TT_VIEW);
	//g_pOERenderSystem->GetTransform(matProj, IOERenderSystem::TT_PROJECTION);

	//CMatrix4x4 matViewProj = matView*matProj;
	//m_pShader->SetMatrix(_T("g_matViewProj"), matViewProj);
	//m_pShader->SetTexture(_T("g_texBase"), m_pTexture);
	//m_pShader->DrawTriList(s_Verts, 4, s_Indis, 6);

	//g_pOERenderSystem->SetFillMode(IOERenderSystem::FM_SOLID);
	//g_pOERenderSystem->SetVertDecl(m_pDecl);
	//g_pOERenderSystem->DrawTriList(s_Verts, 4, s_Indis, 6);

	m_pTerrainMgr->Render(fDetailTime);
}

bool CTestApp::OnLButtonDown(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_bLButtonDown = true;
	return true;
}

bool CTestApp::OnLButtonUp(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_bLButtonDown = false;
	return true;
}

bool CTestApp::OnMouseMove(uint nMsgID, COEDataBufferRead* pDBRead)
{
	if (!m_bLButtonDown) return true;
	COEMsg msg(pDBRead);
	msg.Read(m_nMouseDetailX);
	msg.Read(m_nMouseDetailY);
	return true;
}

bool CTestApp::OnKeyUp(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	int nKeyCode = 0;
	msg.Read(nKeyCode);

	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = false;

	return true;
}

bool CTestApp::OnKeyDown(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	int nKeyCode = 0;
	msg.Read(nKeyCode);

	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE

	return true;
}

bool CTestApp::UpdateMovement(float fDetailTime)
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
