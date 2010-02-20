/*!
 * \file TestApp.h
 * \date 24-5-2009 0:10:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __TESTAPP_H__
#define __TESTAPP_H__

#include <IOEApp.h>
#include <OEInterfaces.h>

#include "../common/Camera.h"
#include "TerrainMgr.h"

class CTestApp : public IOEApp
{
public:
	enum CONST_DEFINE
	{
		KEY_COUNT = 256,
	};

public:
	CTestApp();
	virtual ~CTestApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

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

#endif // __TESTAPP_H__
