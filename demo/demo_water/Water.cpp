/*!
 * \file Water.cpp
 * \date 10-6-2010 22:59:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Water.h"
#include <OECore/IOEShaderMgr.h>
#include <OECore/OERenderSystemUtil.h>

CWater::CWater()
{
	m_fTime = 0.0f;
	m_pVerts = NULL;
	m_pIndis = NULL;
	m_nVerts = 0;
	m_nIndis = 0;
	m_pShader = NULL;

	m_bOK = Init();
}

CWater::~CWater()
{
	Destroy();
}

bool CWater::Init()
{
	static const VERT_DECL_ELEMENT s_Decl[] =
	{
		VDT_FLOAT3, VDU_POSITION, 0,
		//VDT_FLOAT3, VDU_NORMAL, 0,
		//VDT_FLOAT3, VDU_COLOR, 0,
		VDT_FLOAT2, VDU_TEXCOORD, 0,
		//VDT_FLOAT3, VDU_TEXCOORD, 1,
		VDT_UNKNOWN, VDU_UNKNOWN, 0,
	};

	m_nVerts = (NUM_X+1)*(NUM_Z+1);
	m_pVerts = new VERTEX[m_nVerts];
	if (!m_pVerts) return false;

	m_nIndis = NUM_X * NUM_Z * 2 * 3;
	m_pIndis = new ushort[m_nIndis];
	if (!m_pIndis) return false;

	for (int z = 0; z <= NUM_Z; ++z)
	{
		for (int x = 0; x <= NUM_X; ++x)
		{
			int nIndex = z*(NUM_X+1)+x;
			m_pVerts[nIndex].x = x*10.0f;
			m_pVerts[nIndex].y = 0.0f;
			m_pVerts[nIndex].z = z*10.0f;
			m_pVerts[nIndex].u = x/(NUM_X*10.0f);
			m_pVerts[nIndex].v = z/(NUM_Z*10.0f);
		}
	}

	int nIndisIndex = 0;
	for (int z = 0; z < NUM_Z; ++z)
	{
		for (int x = 0; x < NUM_X; ++x)
		{
			int nBaseIndex = z*(NUM_X+1)+x;
			m_pIndis[nIndisIndex++] = nBaseIndex;
			m_pIndis[nIndisIndex++] = nBaseIndex+(NUM_X+1);
			m_pIndis[nIndisIndex++] = nBaseIndex+1;
			m_pIndis[nIndisIndex++] = nBaseIndex+(NUM_X+1);
			m_pIndis[nIndisIndex++] = nBaseIndex+(NUM_X+1)+1;
			m_pIndis[nIndisIndex++] = nBaseIndex+1;
		}
	}

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("demo_water.fx"));
	if (!m_pShader) return false;

	return true;
}

void CWater::Destroy()
{
	SAFE_RELEASE(m_pShader);
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
}

void CWater::Update(float fDetailTime)
{
	// TODO: 
}

void CWater::Render(float fDetailTime)
{
	CDefaultRenderState DefaultState;
	g_pOERenderSystem->SetFillMode(FM_WIREFRAME);

	CMatrix4x4 matWorldToProject;
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_WORLD_VIEW_PROJ);
	m_pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);

	m_pShader->SetFloat(TS("fTime"), m_fTime);
	m_pShader->SetVector(TS("vWaveFreq"), m_vVecFreq);
	m_pShader->SetVector(TS("vWaveSpeed"), m_vVecSpeed);
	m_pShader->SetVector(TS("vWaveDirX"), m_vVecDirX);
	m_pShader->SetVector(TS("vWaveDirY"), m_vVecDirY);
	m_pShader->SetVector(TS("vWaveHeight"), m_vVecHeight);
	m_pShader->SetVector(TS("g_vEyePos"), m_vEyePos);

	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawTriList(m_pVerts, m_nVerts, m_pIndis, m_nIndis);
}

IOERenderData* CWater::GetRenderData()
{
	return NULL;
}
