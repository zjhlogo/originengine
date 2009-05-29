/*!
 * \file OED3DRenderer_Impl.h
 * \date 24-5-2009 18:26:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DRENDERER_IMPL_H__
#define __OED3DRENDERER_IMPL_H__

#include <IOERenderer.h>
#include "OED3DVertDecl_Impl.h"

class COED3DRenderer_Impl : public IOERenderer
{
public:
	COED3DRenderer_Impl();
	virtual ~COED3DRenderer_Impl();

	virtual void SetVertDecl(IOEVertDecl* pVertDecl);
	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);

	virtual void SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat);
	virtual void GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) const;

	virtual void EnableLight(bool bEnable);
	virtual void EnableZBuffer(bool bEnable);
	virtual void EnableFog(bool bEnable);

	virtual void SetCullMode(CULL_MODE_TYPE eMode);
	virtual void SetFillMode(FILL_MODE eFillMode);
	virtual void SetFogInfo(uint nColor, float fNear, float fFar);

private:
	void Init();
	void Destroy();

private:
	COED3DVertDecl_Impl* m_pVertDecl;

};

#endif // __OED3DRENDERER_IMPL_H__
