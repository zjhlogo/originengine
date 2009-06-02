/*!
 * \file MapTile.h
 * \date 29-5-2009 9:04:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MAPTILE_H__
#define __MAPTILE_H__

#include <OEBasicType.h>
#include <OEMath/OEMath.h>

class CMapTile
{
public:
	enum CONST_DEFILE
	{
		TILE_SIZE	= 65,
		TILE_WIDTH	= 400,
		TILE_HEIGHT = 400,
	};

	typedef struct TILE_VERTEX_tag
	{
		float x, y, z;
		uint nColor;
		float u, v;
	} TILE_VERTEX;

public:
	CMapTile();
	~CMapTile();

	//bool LoadMap(const tchar* pstrFileName, int nID);
	bool LoadMap(const ushort* pHeightField, int nID);
	int GetID();

	void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	void Reset();

private:
	TILE_VERTEX* m_pVerts;
	ushort* m_pIndis;
	int m_nID;

};

#endif // __MAPTILE_H__
