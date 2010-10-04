/*!
 * \file MeshApp.cpp
 * \date 31-7-2009 20:46:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshApp.h"
#include "../common/AppHelper.h"

#include <OEBase/IOEMsgMgr.h>
#include <libOEMsg/OEMsgList.h>
#include <OECore/IOETextureMgr.h>
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOEResMgr.h>
#include <OECore/OERenderSystemUtil.h>

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
	m_pTexture = NULL;
	m_pShader = NULL;
	m_pMesh = NULL;
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
		{VDT_FLOAT3, VDU_NORMAL, 0},
		{VDT_FLOAT3, VDU_TEXCOORD, 1},
		{VDT_UBYTE4, VDU_BLENDINDICES, 0},
		{VDT_FLOAT4, VDU_BLENDWEIGHT, 0},
		{VDT_UNKNOWN, VDU_UNKNOWN, 0},
	};

	if (!CBaseApp::Initialize()) return false;

	m_pTexture = g_pOETextureMgr->CreateTextureFromFile(TS("casual03_diff_spec.png"));
	if (!m_pTexture) return false;

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("demo_mesh.fx"));
	if (!m_pShader) return false;

	m_pMesh = g_pOEResMgr->CreateMesh(TS("casual03_mesh.mesh"));
	if (!m_pMesh) return false;

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_SETUP_SHADER_PARAM, this, (MSG_FUNC)&CMeshApp::OnSetupShaderParam);

	return true;
}

void CMeshApp::Terminate()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTexture);
	CBaseApp::Terminate();
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

bool CMeshApp::OnSetupShaderParam(COEMsgShaderParam& msg)
{
	IOEShader* pShader = msg.GetShader();

	pShader->SetVector(TS("g_vLightPos"), CVector3(0.0f, 0.0f, -300.0f));
	pShader->SetVector(TS("g_vEyePos"), m_pCamera->GetEyePos());

	return true;
}
