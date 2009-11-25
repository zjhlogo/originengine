/*!
 * \file WaterApp.h
 * \date 20-6-2009 9:57:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __WATERAPP_H__
#define __WATERAPP_H__

#include <OEInterfaces.h>
#include "../common/Camera.h"
#include "DlgWaveParam.h"

class CWaterApp : public IOEApp
{
public:
	enum CONST_DEFINE
	{
		NUM_X = 80,
		NUM_Z = 80,
		KEY_COUNT = 256,
	};

	typedef struct VERTEX_tag
	{
		float x, y, z;
		//float nx, ny, nz;
		//float hx, hy, hz;
		float u, v;
		//float tx, ty, tz;
	} VERTEX;

public:
	CWaterApp();
	virtual ~CWaterApp();

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
	VERTEX* m_pVerts;
	ushort* m_pIndis;

	uint m_nVerts;
	uint m_nIndis;

	IOEShader* m_pShader;
	CCamera* m_pCamera;
	CDlgWaveParam* m_pDlgWaveParam;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};

#endif // __WATERAPP_H__
