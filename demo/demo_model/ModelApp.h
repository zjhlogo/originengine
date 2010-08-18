/*!
 * \file ModelApp.h
 * \date 1-3-2010 21:43:12
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MODELAPP_H__
#define __MODELAPP_H__

#include <libOEBase/IOEApp.h>
#include <libOEBase/OEMsgMouse.h>
#include <libOEBase/OEMsgKeyboard.h>
#include <libOEBase/OEMsgShaderParam.h>
#include <OECore/IOEModel.h>
#include <vector>

#include "../common/Camera.h"

class CModelApp : public IOEApp
{
public:
	enum CONST_DEFINE
	{
		KEY_COUNT = 256,
	};

public:
	CModelApp();
	~CModelApp();

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
	bool OnSetupShaderParam(COEMsgShaderParam& msg);

	bool UpdateMovement(float fDetailTime);
	bool UpdateRotation(float fDetailTime);

private:
	CCamera* m_pCamera;
	IOEModel* m_pModel;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};
#endif // __MODELAPP_H__
