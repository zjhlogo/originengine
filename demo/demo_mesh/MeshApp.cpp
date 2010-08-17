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
#include <OEBase/IOEMsgMgr.h>
#include <libOEBase/OEMsgID.h>
#include <OECore/OERenderSystemUtil.h>
#include <assert.h>

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
	m_pCamera = NULL;
	m_pTexture = NULL;
	m_pShader = NULL;
	m_pMesh = NULL;
	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
}

void CMeshApp::Destroy()
{
	// TODO: 
}

bool CMeshApp::Initialize()
{
	static const VERT_DECL_ELEMENT s_Decl[] =
	{
		{VDT_FLOAT3, VDU_POSITION, 0},
		{VDT_FLOAT2, VDU_TEXCOORD, 0},
		{VDT_UBYTE4, VDU_BLENDINDICES, 0},
		{VDT_FLOAT4, VDU_BLENDWEIGHT, 0},
		{VDT_UNKNOWN, VDU_UNKNOWN, 0},
	};

	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	m_pTexture = g_pOETextureMgr->CreateTextureFromFile(TS("felhound_diffuse.jpg"));
	if (!m_pTexture) return false;

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("demo_mesh.fx"));
	if (!m_pShader) return false;

	m_pMesh = g_pOEResMgr->CreateMesh(TS("felhound.mesh"));
	if (!m_pMesh) return false;

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_DOWN, this, (MSG_FUNC)&CMeshApp::OnLButtonDown);
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_UP, this, (MSG_FUNC)&CMeshApp::OnLButtonUp);
	g_pOEMsgMgr->RegisterMessage(OMI_MOUSE_MOVE, this, (MSG_FUNC)&CMeshApp::OnMouseMove);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_DOWN, this, (MSG_FUNC)&CMeshApp::OnKeyDown);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_UP, this, (MSG_FUNC)&CMeshApp::OnKeyUp);
	g_pOEMsgMgr->RegisterMessage(OMI_SETUP_SHADER_PARAM, this, (MSG_FUNC)&CMeshApp::OnSetupShaderParam);

	return true;
}

void CMeshApp::Terminate()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETE(m_pCamera);
}

void CMeshApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderSystem->SetTransform(TT_VIEW, m_pCamera->GetViewMatrix());
}

void CMeshApp::Render(float fDetailTime)
{
	int nNumPiece = m_pMesh->GetNumPieces();

	CMatrix4x4 matWorldViewProj;
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_WORLD_VIEW_PROJ);

	CDefaultRenderState DefaultState;

	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = m_pMesh->GetPiece(i);
		if (!pPiece) continue;

		m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
		m_pShader->SetTexture(TS("g_texDiffuse"), m_pTexture);

		g_pOERenderSystem->SetShader(m_pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}
}

bool CMeshApp::OnLButtonDown(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_bLButtonDown = true;
	return true;
}

bool CMeshApp::OnLButtonUp(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_bLButtonDown = false;
	return true;
}

bool CMeshApp::OnMouseMove(uint nMsgID, COEDataBufferRead* pDBRead)
{
	if (!m_bLButtonDown) return true;
	COEMsg msg(pDBRead);
	msg.Read(m_nMouseDetailX);
	msg.Read(m_nMouseDetailY);
	return true;
}

bool CMeshApp::OnKeyUp(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	int nKeyCode = 0;
	msg.Read(nKeyCode);

	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = false;

	return true;
}

bool CMeshApp::OnKeyDown(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	int nKeyCode = 0;
	msg.Read(nKeyCode);

	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE

	return true;
}

bool CMeshApp::OnSetupShaderParam(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	IOEShader* pShader = NULL;
	msg.Read(pShader);

	pShader->SetVector(TS("g_vLightPos"), CVector3(0.0f, 0.0f, -300.0f));
	pShader->SetVector(TS("g_vEyePos"), m_pCamera->GetEyePos());

	return true;
}

bool CMeshApp::UpdateMovement(float fDetailTime)
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

bool CMeshApp::UpdateRotation(float fDetailTime)
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
