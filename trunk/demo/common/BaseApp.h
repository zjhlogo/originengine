/*!
 * \file BaseApp.h
 * \date 8-18-2010 10:50:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BASEAPP_H__
#define __BASEAPP_H__

#include <libOEBase/IOEApp.h>
#include <libOEMsg/OEMsgMouse.h>
#include <libOEMsg/OEMsgKeyboard.h>
#include <libOEMsg/OEMsgShaderParam.h>
#include "Camera.h"
#include "FPSWindow.h"

class CBaseApp : public IOEApp
{
public:
	enum CONST_DEFINE
	{
		KEY_COUNT = 256,
	};

public:
	CBaseApp();
	virtual ~CBaseApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);

protected:
	virtual bool OnLButtonDown(COEMsgMouse& msg);
	virtual bool OnLButtonUp(COEMsgMouse& msg);
	virtual bool OnMouseMove(COEMsgMouse& msg);
	virtual bool OnKeyUp(COEMsgKeyboard& msg);
	virtual bool OnKeyDown(COEMsgKeyboard& msg);

	virtual bool UpdateMovement(float fDetailTime);
	virtual bool UpdateRotation(float fDetailTime);

private:
	void Init();
	void Destroy();

protected:
	CCamera* m_pCamera;
	CFPSWindow* m_pFPS;
	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];
	bool m_bFirstTimeUpdate;

};

#endif // __BASEAPP_H__
