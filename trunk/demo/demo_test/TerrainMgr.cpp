/*!
 * \file TerrainMgr.cpp
 * \date 29-5-2009 9:08:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "TerrainMgr.h"
#include <OEInterfaces.h>
#include <assert.h>

CTerrainMgr::CTerrainMgr()
{
	Init();
}

CTerrainMgr::~CTerrainMgr()
{
	Destroy();
}

void CTerrainMgr::Init()
{
	m_pDecl = NULL;
	memset(m_pMapTileBuff, 0, sizeof(m_pMapTileBuff));
	m_nLastTileX = 0;
	m_nLastTileZ = 0;
	m_bFirstUpdate = true;

	m_pMapFile = _tfopen("maptile.raw", _T("rb")); 
}

void CTerrainMgr::Destroy()
{
	SAFE_RELEASE(m_pDecl);
	for (int i = 0; i < MAX_TILE_COUNT; ++i)
	{
		SAFE_DELETE(m_pMapTileBuff[i]);
	}

	fclose(m_pMapFile);
	m_pMapFile = NULL;
}

bool CTerrainMgr::LoadTerrain()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
		IOEVertDecl::T_COLOR, IOEVertDecl::U_COLOR, 0,
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	if (!m_pDecl) return false;

	for (int i = 0; i < MAX_TILE_COUNT; ++i)
	{
		m_pMapTileBuff[i] = new CMapTile();
	}

	return true;
}

void CTerrainMgr::UpdateTerrain(const CVector3& vEyePos)
{
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

	VTILE_INDEX vTileNotFound;
	vTileNotFound.clear();

	for (int z = -1; z <= 1; ++z)
	{
		for (int x = -1; x <= 1; ++x)
		{
			int nTileX = x+nX;
			int nTileZ = z+nZ;
			if (nTileX < 0 || nTileX > TILE_COUNT_X) continue;
			if (nTileZ < 0 || nTileZ > TILE_COUNT_Z) continue;

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

	for (VTILE_INDEX::iterator it = vTileNotFound.begin(); it != vTileNotFound.end(); ++it)
	{
		int nIndex = (*it);
		if (m_vSleepedTile.empty())
		{
			assert(false);
			break;
		}

		CMapTile* pTile = (*m_vSleepedTile.begin());

		//tstring strTileName = GetMapTileFile(nIndex);
		//if (pTile->LoadMap(strTileName.c_str(), nIndex))

		const ushort* pField = GetMapTileField(nIndex);
		if (pTile->LoadMap(pField, nIndex))
		{
			m_vActivedTile.push_back(pTile);
			m_vSleepedTile.erase(m_vSleepedTile.begin());
		}
	}
}

void CTerrainMgr::Render(float fDetailTime)
{
	g_pOERenderer->SetVertDecl(m_pDecl);

	CMatrix4x4 matWorldBackup;
	g_pOERenderer->GetTransform(matWorldBackup, IOERenderer::TT_WORLD);

	for (VMAP_TILE::iterator it = m_vActivedTile.begin(); it != m_vActivedTile.end(); ++it)
	{
		CMapTile* pTile = (*it);

		CMatrix4x4 matTile;
		CalcMapTileMatrix(matTile, pTile->GetID());
		g_pOERenderer->SetTransform(IOERenderer::TT_WORLD, matTile);
		pTile->Render(fDetailTime);
	}

	g_pOERenderer->SetTransform(IOERenderer::TT_WORLD, matWorldBackup);
}

void CTerrainMgr::ResetTile()
{
	m_vActivedTile.clear();

	m_vSleepedTile.clear();
	for (int i = 0; i < MAX_TILE_COUNT; ++i)
	{
		m_vSleepedTile.push_back(m_pMapTileBuff[i]);
	}
}

CMapTile* CTerrainMgr::ActiveSleepedTile(int nID)
{
	for (VMAP_TILE::iterator it = m_vSleepedTile.begin(); it != m_vSleepedTile.end(); ++it)
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

tstring CTerrainMgr::GetMapTileFile(int nIndex)
{
	// TODO: 根据索引取得分块地图文件名
	return _T("maptile.raw");
}

const ushort* CTerrainMgr::GetMapTileField(int nIndex)
{
	if (!m_pMapFile)
		return NULL;

	long nPos   = 0;
	long nLen   = 0;
	int  nCount = -1;

	for (int z = 0; z < TILE_COUNT_Z; z++)
	{
		nLen = CMapTile::TILE_SIZE*CMapTile::TILE_SIZE*sizeof(ushort)*TILE_COUNT_X * z;

		for (int x = 0; x < TILE_COUNT_X; x++)
		{
			nCount++;

			if (nCount != nIndex)
				continue;

			ushort* pHeightField = new ushort[CMapTile::TILE_SIZE*CMapTile::TILE_SIZE];
			memset(pHeightField, 0, sizeof(ushort) * CMapTile::TILE_SIZE*CMapTile::TILE_SIZE);

			nPos = sizeof(ushort) * x * CMapTile::TILE_SIZE + nLen; 
			fseek(m_pMapFile, nPos, SEEK_SET);

			for (int i = 0; i < CMapTile::TILE_SIZE; i++)
			{
				fread(&pHeightField[i * CMapTile::TILE_SIZE], sizeof(ushort), CMapTile::TILE_SIZE - 1, m_pMapFile);

				nPos  += (sizeof(ushort) * CMapTile::TILE_SIZE) * TILE_COUNT_X;
				fseek(m_pMapFile, nPos, SEEK_SET);
			}

			return pHeightField;
		}
	}

	return NULL;
}

void CTerrainMgr::CalcMapTileMatrix(CMatrix4x4& matOut, int nIndex)
{
	int nZ = nIndex/TILE_COUNT_X;
	int nX = nIndex%TILE_COUNT_X;

	matOut.Identity();
	matOut.m[12] = (float)(nX*CMapTile::TILE_WIDTH);
	matOut.m[13] = 0.0f;
	matOut.m[14] = (float)(nZ*CMapTile::TILE_HEIGHT);
}
