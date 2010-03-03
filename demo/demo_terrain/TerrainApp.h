/*!
 * \file TerrainApp.h
 * \date 24-5-2009 0:10:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __TERRAINAPP_H__
#define __TERRAINAPP_H__

#include <IOEApp.h>
#include "../common/Camera.h"
#include "TerrainMgr.h"

class CTerrainApp : public IOEApp
{
public:
	enum CONST_DEFINE
	{
		KEY_COUNT = 256,
	};

public:
	CTerrainApp();
	virtual ~CTerrainApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);
	virtual void Render3D(float fDetailTime);
	virtual void Render2D(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnLButtonDown(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnLButtonUp(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnMouseMove(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnKeyUp(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnKeyDown(uint nMsgID, COEDataBufferRead* pDBRead);

	bool UpdateMovement(float fDetailTime);
	bool UpdateRotation(float fDetailTime);

private:
	CCamera* m_pCamera;
	CTerrainMgr* m_pTerrainMgr;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};
#endif // __TERRAINAPP_H__
