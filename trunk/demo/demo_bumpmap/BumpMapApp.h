/*!
 * \file BumpMapApp.h
 * \date 6-7-2009 9:23:04
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __BUMPMAPAPP_H__
#define __BUMPMAPAPP_H__

#include <libOEBase/IOEApp.h>
#include <libOEBase/OEMsgMouse.h>
#include <libOEBase/OEMsgKeyboard.h>
#include <libOEBase/OEMsgShaderParam.h>
#include <OECore/IOEVertDecl.h>
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOETextureMgr.h>

#include "../common/Camera.h"
#include "../common/SimpleShape.h"

class CBumpMapApp : public IOEApp
{
public:
	enum CONST_DEFINE
	{
		KEY_COUNT = 256,
	};

	typedef struct VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		float tx, ty, tz;
	} VERTEX;

public:
	CBumpMapApp();
	virtual ~CBumpMapApp();

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
	CSimpleShape* m_pSimpleShape;

	IOEShader* m_pShader;
	IOETexture* m_pTexBase;
	IOETexture* m_pTexNormal;
	IOETexture* m_pTexHeight;
	CVector3 m_vLightPos;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};

#endif // __BUMPMAPAPP_H__
