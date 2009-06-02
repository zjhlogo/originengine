/*!
 * \file MapTile.cpp
 * \date 29-5-2009 9:04:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MapTile.h"
#include <OEInterfaces.h>

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
}

void CMapTile::Destroy()
{
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
}

//bool CMapTile::LoadMap(const tchar* pstrFileName, int nID)
//{
//	Reset();
//
//	IOEFile* pFile = g_pOEFileMgr->OpenFile(pstrFileName);
//	if (!pFile) return false;
//
//	ushort* pHeightField = new ushort[TILE_SIZE*TILE_SIZE];
//	pFile->Read(pHeightField, sizeof(ushort)*TILE_SIZE*TILE_SIZE);
//	SAFE_RELEASE(pFile);
//
//	if (!m_pVerts)
//	{
//		m_pVerts = new TILE_VERTEX[TILE_SIZE*TILE_SIZE];
//	}
//
//	float fDetailWidth = (float)TILE_WIDTH/(TILE_SIZE-1);
//	float fDetailHeight = (float)TILE_HEIGHT/(TILE_SIZE-1);
//	for (int z = 0; z < TILE_SIZE; ++z)
//	{
//		for (int x = 0; x < TILE_SIZE; ++x)
//		{
//			int nIndex = z*TILE_SIZE+x;
//			int nHeightIndex = (TILE_SIZE-z-1)*TILE_SIZE+x;
//			m_pVerts[nIndex].x = x*fDetailWidth;
//			m_pVerts[nIndex].y = pHeightField[nHeightIndex]*0.004f-100.0f;
//			m_pVerts[nIndex].z = z*fDetailHeight;
//			m_pVerts[nIndex].nColor = 0xFFFFFFFF;
//			m_pVerts[nIndex].u = m_pVerts[nIndex].x/TILE_WIDTH;
//			m_pVerts[nIndex].v = 1.0f - m_pVerts[nIndex].z/TILE_HEIGHT;
//		}
//	}
//
//	if (!m_pIndis)
//	{
//		m_pIndis = new ushort[(TILE_SIZE-1)*(TILE_SIZE-1)*6];
//	}
//
//	int nIndisIndex = 0;
//	for (int z = 0; z < TILE_SIZE-1; ++z)
//	{
//		for (int x = 0; x < TILE_SIZE-1; ++x)
//		{
//			int nBaseIndex = z*TILE_SIZE+x;
//			m_pIndis[nIndisIndex++] = nBaseIndex;
//			m_pIndis[nIndisIndex++] = nBaseIndex+TILE_SIZE;
//			m_pIndis[nIndisIndex++] = nBaseIndex+1;
//			m_pIndis[nIndisIndex++] = nBaseIndex+TILE_SIZE;
//			m_pIndis[nIndisIndex++] = nBaseIndex+TILE_SIZE+1;
//			m_pIndis[nIndisIndex++] = nBaseIndex+1;
//		}
//	}
//
//	SAFE_DELETE_ARRAY(pHeightField);
//
//	m_nID = nID;
//	return true;
//}

bool CMapTile::LoadMap(const ushort* pHeightField, int nID)
{
	if (!pHeightField) return false;

	Reset();
	
	if (!m_pVerts)
	{
		m_pVerts = new TILE_VERTEX[TILE_SIZE*TILE_SIZE];
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
			m_pVerts[nIndex].y = pHeightField[nHeightIndex]*0.004f-100.0f;
			m_pVerts[nIndex].z = z*fDetailHeight;
			m_pVerts[nIndex].nColor = 0xFFFFFFFF;
			m_pVerts[nIndex].u = m_pVerts[nIndex].x/TILE_WIDTH;
			m_pVerts[nIndex].v = 1.0f - m_pVerts[nIndex].z/TILE_HEIGHT;
		}
	}

	if (!m_pIndis)
	{
		m_pIndis = new ushort[(TILE_SIZE-1)*(TILE_SIZE-1)*6];
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

	SAFE_DELETE_ARRAY(pHeightField);

	m_nID = nID;
	return true;
}

int CMapTile::GetID()
{
	return m_nID;
}

void CMapTile::Render(float fDetailTime)
{
	g_pOERenderer->DrawTriList(m_pVerts, TILE_SIZE*TILE_SIZE, m_pIndis, (TILE_SIZE-1)*(TILE_SIZE-1)*6);
}

void CMapTile::Reset()
{
	m_nID = -1;
}
