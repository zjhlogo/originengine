/*!
 * \file TerrainApp.h
 * \date 24-5-2009 0:10:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __TERRAINAPP_H__
#define __TERRAINAPP_H__

#include <libOEBase/IOEApp.h>
#include <libOEBase/OEMsgMouse.h>
#include <libOEBase/OEMsgKeyboard.h>
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
	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnLButtonDown(COEMsgMouse& msg);
	bool OnLButtonUp(COEMsgMouse& msg);
	bool OnMouseMove(COEMsgMouse& msg);
	bool OnKeyUp(COEMsgKeyboard& msg);
	bool OnKeyDown(COEMsgKeyboard& msg);

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
