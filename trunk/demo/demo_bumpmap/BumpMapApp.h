/*!
 * \file BumpMapApp.h
 * \date 6-7-2009 9:23:04
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __BUMPMAPAPP_H__
#define __BUMPMAPAPP_H__

#include "../common/BaseApp.h"
#include "../common/SimpleShape.h"
#include <OECore/IOEShader.h>
#include <OECore/IOETexture.h>

class CBumpMapApp : public CBaseApp
{
public:
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

	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	virtual bool OnKeyDown(COEMsgKeyboard& msg);

private:
	CSimpleShape* m_pSimpleShape;

	IOEShader* m_pShader;
	IOETexture* m_pTexBase;
	IOETexture* m_pTexNormal;
	IOETexture* m_pTexHeight;
	CVector3 m_vLightPos;

};

#endif // __BUMPMAPAPP_H__
