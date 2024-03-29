/*!
 * \file Terrain.cpp
 * \date 10-6-2010 22:47:32
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Terrain.h"
#include <OECore/IOECore.h>
#include <assert.h>

CTerrain::CTerrain()
{
	m_bOK = Init();
}

CTerrain::~CTerrain()
{
	Destroy();
}

bool CTerrain::Init()
{
	memset(m_pMapTileBuff, 0, sizeof(m_pMapTileBuff));
	m_nLastTileX = 0;
	m_nLastTileZ = 0;
	m_bFirstUpdate = true;

	m_pMapFile = NULL;

	return true;
}

void CTerrain::Destroy()
{
	SAFE_RELEASE(m_pMapFile);

	for (int i = 0; i < MAX_CACHE_COUNT; ++i)
	{
		SAFE_DELETE(m_pMapTileBuff[i]);
	}
}

void CTerrain::Update(float fDetailTime)
{
	IOENode* pCameraNode = g_pOECore->GetRootNode()->GetChildNode(TS("Camera"));
	if (!pCameraNode) return;

	const CVector3 vEyePos = pCameraNode->GetPosition();

	float fX = vEyePos.x;
	float fZ = vEyePos.z;

	int nX = (int)(fX/CMapTile::TILE_WIDTH);
	int nZ = (int)(fZ/CMapTile::TILE_HEIGHT);

	// if at the same tile, no need update
	if (!m_bFirstUpdate
		&& m_nLastTileX == nX
		&& m_nLastTileZ == nZ) return;

	m_bFirstUpdate = false;

	m_nLastTileX = nX;
	m_nLastTileZ = nZ;

	// start update terrain tile
	ResetTile();

	TV_TILE_INDEX vTileNotFound;
	vTileNotFound.clear();

	for (int z = -1; z <= 1; ++z)
	{
		for (int x = -1; x <= 1; ++x)
		{
			int nTileX = x+nX;
			int nTileZ = z+nZ;
			if (nTileX < 0 || nTileX >= TILE_COUNT_X) continue;
			if (nTileZ < 0 || nTileZ >= TILE_COUNT_Z) continue;

			int nIndex = nTileZ*TILE_COUNT_X+nTileX;

			CMapTile* pMapTile = ActiveSleepedTile(nIndex);
			if (!pMapTile)
			{
				vTileNotFound.push_back(nIndex);
			}
			else
			{
				m_vActivedTile.push_back(pMapTile);
			}
		}
	}

	for (TV_TILE_INDEX::iterator it = vTileNotFound.begin(); it != vTileNotFound.end(); ++it)
	{
		int nIndex = (*it);
		if (m_vSleepedTile.empty())
		{
			assert(false);
			break;
		}

		CMapTile* pTile = (*m_vSleepedTile.begin());

		const ushort* pField = GetMapTileField(nIndex);
		if (pTile->LoadMap(pField, nIndex))
		{
			m_vActivedTile.push_back(pTile);
			m_vSleepedTile.erase(m_vSleepedTile.begin());
		}
	}
}

void CTerrain::Render(float fDetailTime)
{
	for (TV_MAP_TILE::iterator it = m_vActivedTile.begin(); it != m_vActivedTile.end(); ++it)
	{
		CMapTile* pTile = (*it);
		pTile->Render(fDetailTime);
	}
}

IOERenderData* CTerrain::GetRenderData()
{
	return NULL;
}

bool CTerrain::LoadTerrain()
{
	for (int i = 0; i < MAX_CACHE_COUNT; ++i)
	{
		SAFE_DELETE(m_pMapTileBuff[i]);
		m_pMapTileBuff[i] = new CMapTile();
		assert(m_pMapTileBuff[i]);
	}

	m_pMapFile = g_pOEFileMgr->OpenFile(TS("media\\terrain.raw"));
	if (!m_pMapFile) return false;

	return true;
}

void CTerrain::ResetTile()
{
	m_vActivedTile.clear();
	m_vSleepedTile.clear();

	for (int i = 0; i < MAX_CACHE_COUNT; ++i)
	{
		m_vSleepedTile.push_back(m_pMapTileBuff[i]);
	}
}

CMapTile* CTerrain::ActiveSleepedTile(int nID)
{
	for (TV_MAP_TILE::iterator it = m_vSleepedTile.begin(); it != m_vSleepedTile.end(); ++it)
	{
		CMapTile* pTile = (*it);
		if (pTile->GetID() == nID)
		{
			m_vSleepedTile.erase(it);
			return pTile;
		}
	}

	return NULL;
}

const ushort* CTerrain::GetMapTileField(int nIndex)
{
	static ushort s_HeightField[CMapTile::TILE_SIZE*CMapTile::TILE_SIZE];

	uint nX = nIndex%TILE_COUNT_X;
	uint nZ = TILE_COUNT_Z - nIndex/TILE_COUNT_X - 1;

	assert(nX < TILE_COUNT_X && nZ < TILE_COUNT_Z);

	uint nBasePos = (nZ*(CMapTile::TILE_SIZE-1)*MAP_SIZE_X+nX*(CMapTile::TILE_SIZE-1))*sizeof(ushort);

	for (uint z = 0; z < CMapTile::TILE_SIZE; ++z)
	{
		m_pMapFile->Seek(nBasePos+z*MAP_SIZE_X*sizeof(ushort));
		m_pMapFile->Read(&s_HeightField[z*CMapTile::TILE_SIZE], CMapTile::TILE_SIZE*sizeof(ushort));
	}

	return s_HeightField;
}
