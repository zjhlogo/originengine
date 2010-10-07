/*!
 * \file Terrain.h
 * \date 10-6-2010 22:47:26
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "MapTile.h"
#include <OEBase/IOEFileMgr.h>
#include <OECore/IOERenderableObject.h>
#include <vector>

class CTerrain : public IOERenderableObject
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

	typedef std::vector<CMapTile*> TV_MAP_TILE;
	typedef std::vector<int> TV_TILE_INDEX;

public:
	RTTI_DEF(CTerrain, IOERenderableObject);

	CTerrain();
	virtual ~CTerrain();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);
	virtual IOERenderData* GetRenderData();

	bool LoadTerrain();

	void SetEyePos(const CVector3& vEyePos);
	const CVector3& GetEyePos();

private:
	bool Init();
	void Destroy();

	void ResetTile();
	CMapTile* ActiveSleepedTile(int nID);

	const ushort* GetMapTileField(int nIndex);
	void CalcMapTileMatrix(CMatrix4x4& matOut, int nIndex);

private:
	CMapTile* m_pMapTileBuff[MAX_CACHE_COUNT];

	TV_MAP_TILE m_vActivedTile;
	TV_MAP_TILE m_vSleepedTile;

	bool m_bFirstUpdate;
	int m_nLastTileX;
	int m_nLastTileZ;

	IOEFile* m_pMapFile;

	CVector3 m_vEyePos;

};
#endif // __TERRAIN_H__
