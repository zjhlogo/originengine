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
#include <IOEVertDecl.h>
#include <OEInterfaces.h>

#include "Camera.h"
#include "TerrainMgr.h"

class CTestApp : public IOEApp
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
	} VERTEX;

public:
	CTestApp();
	virtual ~CTestApp();

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
	//IOEVertDecl* m_pDecl;
	CCamera* m_pCamera;
	CTerrainMgr* m_pTerrainMgr;

	IOETexture* m_pTexture;
	IOEShader* m_pShader;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};

#endif // __TESTAPP_H__
