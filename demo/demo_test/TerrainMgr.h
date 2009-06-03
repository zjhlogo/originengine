/*!
 * \file TerrainMgr.h
 * \date 29-5-2009 9:08:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __TERRAINMGR_H__
#define __TERRAINMGR_H__

#include "MapTile.h"
#include <OEInterfaces.h>
#include <vector>

class CTerrainMgr
{
public:
	enum CONST_DEFINE
	{
		MAX_CACHE_COUNT = 9,

		TILE_COUNT_X = 16,
		TILE_COUNT_Z = 16,

		MAP_SIZE_X = TILE_COUNT_X*(CMapTile::TILE_SIZE-1)+1,
		MAP_SIZE_Z = TILE_COUNT_Z*(CMapTile::TILE_SIZE-1)+1,
	};

	typedef std::vector<CMapTile*> VMAP_TILE;
	typedef std::vector<int> VTILE_INDEX;

public:
	CTerrainMgr();
	~CTerrainMgr();

	bool LoadTerrain();
	void UpdateTerrain(const CVector3& vEyePos);

	void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	void ResetTile();
	CMapTile* ActiveSleepedTile(int nID);
	tstring GetMapTileFile(int nIndex);
	const uchar* GetMapTileField(int nIndex);
	void CalcMapTileMatrix(CMatrix4x4& matOut, int nIndex);

private:
	IOEVertDecl* m_pDecl;
	IOETexture* m_pTexture;

	CMapTile* m_pMapTileBuff[MAX_CACHE_COUNT];

	VMAP_TILE m_vActivedTile;
	VMAP_TILE m_vSleepedTile;

	bool m_bFirstUpdate;
	int m_nLastTileX;
	int m_nLastTileZ;

	IOEFile* m_pMapFile;
};

#endif // __TERRAINMGR_H__
