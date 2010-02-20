/*!
 * \file BumpMapApp.cpp
 * \date 6-7-2009 9:23:27
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "BumpMapApp.h"
#include "../common/AppHelper.h"
#include <OERenderSystemUtil.h>
#include <OEMsgID.h>
#include <assert.h>

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
	m_pCamera = NULL;
	m_pSimpleShape = NULL;

	m_pShader = NULL;
	m_pTexBase = NULL;
	m_pTexNormal = NULL;
	m_pTexHeight = NULL;

	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
}

void CBumpMapApp::Destroy()
{
	// TODO: 
}

bool CBumpMapApp::Initialize()
{
	static const VERT_DECL_ELEMENT s_Decl[] =
	{
		VDT_FLOAT3, VDU_POSITION, 0,
		VDT_FLOAT2, VDU_TEXCOORD, 0,
		VDT_FLOAT3, VDU_TEXCOORD, 1,		// normal
		VDT_FLOAT3, VDU_TEXCOORD, 2,		// tangent
		VDT_UNKNOWN, VDU_UNKNOWN, 0,
	};

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("bumpmap.fx"));
	if (!m_pShader) return false;

	m_pTexBase = g_pOETextureMgr->CreateTextureFromFile(TS("rock.png"));
	if (!m_pTexBase) return false;

	m_pTexNormal = g_pOETextureMgr->CreateTextureFromFile(TS("rock_normal.png"));
	if (!m_pTexNormal) return false;

	m_pTexHeight = g_pOETextureMgr->CreateTextureFromFile(TS("rock_height.png"));
	if (!m_pTexHeight) return false;

	m_pCamera = new CCamera();

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape || !m_pSimpleShape->IsOK()) return false;

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_DOWN, this, (MSG_FUNC)&CBumpMapApp::OnLButtonDown);
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_UP, this, (MSG_FUNC)&CBumpMapApp::OnLButtonUp);
	g_pOEMsgMgr->RegisterMessage(OMI_MOUSE_MOVE, this, (MSG_FUNC)&CBumpMapApp::OnMouseMove);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_DOWN, this, (MSG_FUNC)&CBumpMapApp::OnKeyDown);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_UP, this, (MSG_FUNC)&CBumpMapApp::OnKeyUp);

	return true;
}

void CBumpMapApp::Terminate()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTexBase);
	SAFE_RELEASE(m_pTexNormal);
	SAFE_RELEASE(m_pTexHeight);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSimpleShape);
}

void CBumpMapApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderSystem->SetTransform(TT_VIEW, m_pCamera->GetViewMatrix());
}

void CBumpMapApp::Render3D(float fDetailTime)
{
	static float s_fTotalTime = 0.0f;

	static const VERTEX s_Verts[4] =
	{
		{-10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		{-10.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		{10.0f, 0.0f, 10.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		{10.0f, 0.0f, -10.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	};

	static const ushort s_Indis[6] = {0, 1, 3, 1, 2, 3};

	CDefaultRenderState DefaultState;

	CMatrix4x4 matWorldViewProj;
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_WORLD_VIEW_PROJ);
	m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);

	s_fTotalTime += fDetailTime;
	m_vLightPos.x = cos(s_fTotalTime)*10.0f;
	m_vLightPos.y = 5.0f;
	m_vLightPos.z = sin(s_fTotalTime)*10.0f;

	m_pShader->SetVector(TS("g_vLightPos"), m_vLightPos);
	m_pShader->SetVector(TS("g_vEyePos"), m_pCamera->GetEyePos());
	m_pShader->SetTexture(TS("g_texDiffuse"), m_pTexBase);
	m_pShader->SetTexture(TS("g_texNormal"), m_pTexNormal);
	m_pShader->SetTexture(TS("g_texHeightMap"), m_pTexHeight);
	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawTriList(s_Verts, 4, s_Indis, 6);

	m_pSimpleShape->DrawCube(m_vLightPos);
}

void CBumpMapApp::Render2D(float fDetailTime)
{
	// TODO: 
}

bool CBumpMapApp::OnLButtonDown(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_bLButtonDown = true;
	return true;
}

bool CBumpMapApp::OnLButtonUp(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_bLButtonDown = false;
	return true;
}

bool CBumpMapApp::OnMouseMove(uint nMsgID, COEDataBufferRead* pDBRead)
{
	if (!m_bLButtonDown) return true;
	COEMsg msg(pDBRead);
	msg.Read(m_nMouseDetailX);
	msg.Read(m_nMouseDetailY);
	return true;
}

bool CBumpMapApp::OnKeyUp(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	int nKeyCode = 0;
	msg.Read(nKeyCode);

	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = false;

	return true;
}

bool CBumpMapApp::OnKeyDown(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	int nKeyCode = 0;
	msg.Read(nKeyCode);

	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE

	if (m_KeyDown['1']) m_pShader->SetTechnique(TS("NormalMap"));
	if (m_KeyDown['2']) m_pShader->SetTechnique(TS("ParallaxMap"));
	if (m_KeyDown['3']) m_pShader->SetTechnique(TS("DiffuseTexture"));
	if (m_KeyDown['4']) m_pShader->SetTechnique(TS("NormalTexture"));
	if (m_KeyDown['5']) m_pShader->SetTechnique(TS("HeightMapTexture"));

	return true;
}

bool CBumpMapApp::UpdateMovement(float fDetailTime)
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

bool CBumpMapApp::UpdateRotation(float fDetailTime)
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
