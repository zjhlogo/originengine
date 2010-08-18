/*!
 * \file WaterApp.h
 * \date 20-6-2009 9:57:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __WATERAPP_H__
#define __WATERAPP_H__

#include "../common/BaseApp.h"
#include "DlgWaveSetting.h"

#include <OECore/IOEShader.h>

class CWaterApp : public CBaseApp
{
public:
	enum CONST_DEFINE
	{
		NUM_X = 80,
		NUM_Z = 80,
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

	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	virtual bool OnKeyDown(COEMsgKeyboard& msg);

private:
	VERTEX* m_pVerts;
	ushort* m_pIndis;

	uint m_nVerts;
	uint m_nIndis;

	IOEShader* m_pShader;
	CDlgWaveSetting* m_pDlgWaveSetting;

};

#endif // __WATERAPP_H__
