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
#include <IOEVertDecl.h>
#include <vector>

class CTerrainMgr
{
public:
	enum CONST_DEFINE
	{
		MAX_TILE_COUNT = 9,
		TILE_COUNT_X = 16,
		TILE_COUNT_Z = 16,
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
	void CalcMapTileMatrix(CMatrix4x4& matOut, int nIndex);

private:
	IOEVertDecl* m_pDecl;
	CMapTile* m_pMapTileBuff[MAX_TILE_COUNT];

	VMAP_TILE m_vActivedTile;
	VMAP_TILE m_vSleepedTile;

	bool m_bFirstUpdate;
	int m_nLastTileX;
	int m_nLastTileZ;
};

#endif // __TERRAINMGR_H__
