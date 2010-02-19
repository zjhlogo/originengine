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
#include <OEInterfaces.h>
#include <OEMath/OEMath.h>

class CMapTile
{
public:
	enum CONST_DEFILE
	{
		TILE_SIZE	= 65,
		TILE_WIDTH	= 800,
		TILE_HEIGHT = 800,
	};

public:
	CMapTile();
	~CMapTile();

	bool LoadMap(const ushort* pHeightField, int nID);
	int GetID();

	void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	void Reset();
	void CalcMatrix(CMatrix4x4& matOut, int nIndex);

private:
	VERTEX_POLYT* m_pVerts;
	ushort* m_pIndis;
	int m_nID;

	IOETexture* m_pTexture;
	IOEShader* m_pShader;
};

#endif // __MAPTILE_H__
