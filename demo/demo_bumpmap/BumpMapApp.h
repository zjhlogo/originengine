/*!
 * \file BumpMapApp.h
 * \date 6-7-2009 9:23:04
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __BUMPMAPAPP_H__
#define __BUMPMAPAPP_H__

#include <IOEApp.h>
#include <IOEVertDecl.h>
#include <OEInterfaces.h>
#include <IOEShader.h>

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

	virtual void OnLButtonDown(int x, int y);
	virtual void OnLButtonUp(int x, int y);
	virtual void OnMouseMove(int dx, int dy);
	virtual void OnKeyUp(int nKeyCode);
	virtual void OnKeyDown(int nKeyCode);

private:
	void Init();
	void Destroy();

	bool UpdateMovement(float fDetailTime);
	bool UpdateRotation(float fDetailTime);

private:
	CCamera* m_pCamera;
	CSimpleShape* m_pSimpleShape;

	IOEShader* m_pShader;
	IOETexture* m_pTexBase;
	IOETexture* m_pTexNormal;
	CVector4 m_vLightPos;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};

#endif // __BUMPMAPAPP_H__
