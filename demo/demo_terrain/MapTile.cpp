/*!
 * \file MapTile.cpp
 * \date 29-5-2009 9:04:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MapTile.h"
#include "Terrain.h"
#include <OECore/OERenderSystemUtil.h>

CMapTile::CMapTile()
{
	Init();
}

CMapTile::~CMapTile()
{
	Destroy();
}

void CMapTile::Init()
{
	m_pVerts = NULL;
	m_pIndis = NULL;
	m_nID = -1;
	m_pTexture = NULL;
	m_pShader = NULL;
}

void CMapTile::Destroy()
{
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pShader);
}

bool CMapTile::LoadMap(const ushort* pHeightField, int nID)
{
	Reset();

	if (!m_pTexture)
	{
		m_pTexture = g_pOETextureMgr->CreateTextureFromFile(TS("grass_diffuse.png"));
		if (!m_pTexture) return false;
	}

	if (!m_pShader)
	{
		m_pShader = g_pOEShaderMgr->CreateDefaultShader(DST_POLYT);
		if (!m_pShader) return false;
	}

	if (!m_pVerts)
	{
		m_pVerts = new VERTEX_POLYT[TILE_SIZE*TILE_SIZE];
		if (!m_pVerts) return false;
	}

	if (!m_pIndis)
	{
		m_pIndis = new ushort[(TILE_SIZE-1)*(TILE_SIZE-1)*6];
		if (!m_pIndis) return false;
	}

	float fDetailWidth = (float)TILE_WIDTH/(TILE_SIZE-1);
	float fDetailHeight = (float)TILE_HEIGHT/(TILE_SIZE-1);
	for (int z = 0; z < TILE_SIZE; ++z)
	{
		for (int x = 0; x < TILE_SIZE; ++x)
		{
			int nIndex = z*TILE_SIZE+x;
			int nHeightIndex = (TILE_SIZE-z-1)*TILE_SIZE+x;
			m_pVerts[nIndex].x = x*fDetailWidth;
			m_pVerts[nIndex].y = pHeightField[nHeightIndex]*0.01f-400.0f;
			m_pVerts[nIndex].z = z*fDetailHeight;
			m_pVerts[nIndex].u = m_pVerts[nIndex].x/TILE_WIDTH;
			m_pVerts[nIndex].v = 1.0f - m_pVerts[nIndex].z/TILE_HEIGHT;
		}
	}

	int nIndisIndex = 0;
	for (int z = 0; z < TILE_SIZE-1; ++z)
	{
		for (int x = 0; x < TILE_SIZE-1; ++x)
		{
			int nBaseIndex = z*TILE_SIZE+x;
			m_pIndis[nIndisIndex++] = nBaseIndex;
			m_pIndis[nIndisIndex++] = nBaseIndex+TILE_SIZE;
			m_pIndis[nIndisIndex++] = nBaseIndex+1;
			m_pIndis[nIndisIndex++] = nBaseIndex+TILE_SIZE;
			m_pIndis[nIndisIndex++] = nBaseIndex+TILE_SIZE+1;
			m_pIndis[nIndisIndex++] = nBaseIndex+1;
		}
	}

	m_nID = nID;
	return true;
}

int CMapTile::GetID()
{
	return m_nID;
}

void CMapTile::Render(float fDetailTime)
{
	CDefaultRenderState DefaultState;

	CMatrix4x4 matWorld;
	CalcMatrix(matWorld, m_nID);

	CMatrix4x4 matViewProj;
	g_pOERenderSystem->GetTransform(matViewProj, TT_VIEW_PROJ);
	CMatrix4x4 matWorldViewProj = matWorld*matViewProj;

	m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
	m_pShader->SetTexture(TS("g_texDiffuse"), m_pTexture);
	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawTriList(m_pVerts, TILE_SIZE*TILE_SIZE, m_pIndis, (TILE_SIZE-1)*(TILE_SIZE-1)*6);
}

void CMapTile::Reset()
{
	m_nID = -1;
}

void CMapTile::CalcMatrix(CMatrix4x4& matOut, int nIndex)
{
	int nZ = nIndex/CTerrain::TILE_COUNT_X;
	int nX = nIndex%CTerrain::TILE_COUNT_X;

	matOut.Identity();
	matOut.m[12] = (float)(nX*TILE_WIDTH);
	matOut.m[13] = 0.0f;
	matOut.m[14] = (float)(nZ*TILE_HEIGHT);
}
