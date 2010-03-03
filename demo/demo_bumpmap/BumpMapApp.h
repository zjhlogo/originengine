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
#include <IOEShaderMgr.h>
#include <IOETextureMgr.h>

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
	virtual void Render3D(float fDetailTime);
	virtual void Render2D(float fDetailTime);

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
