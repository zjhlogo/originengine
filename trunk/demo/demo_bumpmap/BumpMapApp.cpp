/*!
 * \file BumpMapApp.cpp
 * \date 6-7-2009 9:23:27
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "BumpMapApp.h"
#include "../common/AppHelper.h"

#include <OECore/IOEShaderMgr.h>
#include <OECore/IOETextureMgr.h>
#include <OECore/OERenderSystemUtil.h>

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
	m_pSimpleShape = NULL;

	m_pShader = NULL;
	m_pTexBase = NULL;
	m_pTexNormal = NULL;
	m_pTexHeight = NULL;
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

	if (!CBaseApp::Initialize()) return false;

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("demo_bumpmap.fx"));
	if (!m_pShader) return false;

	m_pTexBase = g_pOETextureMgr->CreateTextureFromFile(TS("rock_diffuse.png"));
	if (!m_pTexBase) return false;

	m_pTexNormal = g_pOETextureMgr->CreateTextureFromFile(TS("rock_normal.png"));
	if (!m_pTexNormal) return false;

	m_pTexHeight = g_pOETextureMgr->CreateTextureFromFile(TS("rock_height.png"));
	if (!m_pTexHeight) return false;

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape || !m_pSimpleShape->IsOK()) return false;

	return true;
}

void CBumpMapApp::Terminate()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTexBase);
	SAFE_RELEASE(m_pTexNormal);
	SAFE_RELEASE(m_pTexHeight);
	SAFE_DELETE(m_pSimpleShape);
	CBaseApp::Terminate();
}

void CBumpMapApp::Render(float fDetailTime)
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

bool CBumpMapApp::OnKeyDown(COEMsgKeyboard& msg)
{
	CBaseApp::OnKeyDown(msg);

	if (m_KeyDown['1']) m_pShader->SetTechnique(TS("NormalMap"));
	if (m_KeyDown['2']) m_pShader->SetTechnique(TS("ParallaxMap"));
	if (m_KeyDown['3']) m_pShader->SetTechnique(TS("DiffuseTexture"));
	if (m_KeyDown['4']) m_pShader->SetTechnique(TS("NormalTexture"));
	if (m_KeyDown['5']) m_pShader->SetTechnique(TS("HeightMapTexture"));

	return true;
}
