/*!
 * \file BumpMap.cpp
 * \date 10-6-2010 21:40:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "BumpMap.h"
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOETextureMgr.h>
#include <OECore/OERenderSystemUtil.h>

CBumpMap::CBumpMap()
{
	m_pShader = NULL;
	m_pTexDiffuse = NULL;
	m_pTexNormalHeight = NULL;

	m_bOK = Init();
}

CBumpMap::~CBumpMap()
{
	Destroy();
}

bool CBumpMap::Init()
{
	static const VERT_DECL_ELEMENT s_Decl[] =
	{
		VDT_FLOAT3, VDU_POSITION, 0,
		VDT_FLOAT2, VDU_TEXCOORD, 0,
		VDT_FLOAT3, VDU_TEXCOORD, 1,		// normal
		VDT_FLOAT3, VDU_TEXCOORD, 2,		// tangent
		VDT_UNKNOWN, VDU_UNKNOWN, 0,
	};

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("demo_bumpmap.fx"));
	if (!m_pShader) return false;

	m_pTexDiffuse = g_pOETextureMgr->CreateTextureFromFile(TS("rock_diffuse.png"));
	if (!m_pTexDiffuse) return false;

	m_pTexNormalHeight = g_pOETextureMgr->CreateTextureFromFile(TS("rock_normal_height.png"));
	if (!m_pTexNormalHeight) return false;

	return true;
}

void CBumpMap::Destroy()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTexDiffuse);
	SAFE_RELEASE(m_pTexNormalHeight);
}

void CBumpMap::Update(float fDetailTime)
{
	// TODO: 
}

void CBumpMap::Render(float fDetailTime)
{
	static const VERTEX s_Verts[4] =
	{
		{-10.0f, -10.0f, 30.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{-10.0f, 10.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{10.0f, 10.0f, 30.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f},
		{10.0f, -10.0f, 30.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f},
	};

	static const ushort s_Indis[6] = {0, 1, 3, 1, 2, 3};

	CDefaultRenderState DefaultState;

	CMatrix4x4 matWorldViewProj;
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_WORLD_VIEW_PROJ);
	m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);

	m_pShader->SetVector(TS("g_vLightPos"), m_vLightPos);
	m_pShader->SetVector(TS("g_vEyePos"), m_vEyePos);
	m_pShader->SetTexture(TS("g_texDiffuse"), m_pTexDiffuse);
	m_pShader->SetTexture(TS("g_texNormalHeight"), m_pTexNormalHeight);
	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawTriList(s_Verts, 4, s_Indis, 6);
}

IOERenderData* CBumpMap::GetRenderData()
{
	return NULL;
}

void CBumpMap::SetEyePos(const CVector3& vEyePos)
{
	m_vEyePos = vEyePos;
}

const CVector3& CBumpMap::GetEyePos()
{
	return m_vEyePos;
}

void CBumpMap::SetLightPos(const CVector3& vLightPos)
{
	m_vLightPos = vLightPos;
}

const CVector3& CBumpMap::GetLightPos()
{
	return m_vLightPos;
}

void CBumpMap::SetTechnique(const tstring& strTechnique)
{
	m_pShader->SetTechnique(strTechnique);
}
