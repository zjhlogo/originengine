/*!
 * \file WaterApp.h
 * \date 20-6-2009 9:57:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __WATERAPP_H__
#define __WATERAPP_H__

#include <libOEBase/IOEApp.h>
#include <libOEBase/OEMsgMouse.h>
#include <libOEBase/OEMsgKeyboard.h>
#include <OECore/IOEShaderMgr.h>

#include "../common/Camera.h"
#include "DlgWaveSetting.h"

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

	bool OnLButtonDown(COEMsgMouse& msg);
	bool OnLButtonUp(COEMsgMouse& msg);
	bool OnMouseMove(COEMsgMouse& msg);
	bool OnKeyUp(COEMsgKeyboard& msg);
	bool OnKeyDown(COEMsgKeyboard& msg);

	bool UpdateMovement(float fDetailTime);
	bool UpdateRotation(float fDetailTime);

private:
	VERTEX* m_pVerts;
	ushort* m_pIndis;

	uint m_nVerts;
	uint m_nIndis;

	IOEShader* m_pShader;
	CCamera* m_pCamera;
	CDlgWaveSetting* m_pDlgWaveSetting;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};

#endif // __WATERAPP_H__
