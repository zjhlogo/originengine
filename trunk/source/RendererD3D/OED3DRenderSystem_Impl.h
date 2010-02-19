/*!
 * \file OED3DRenderSystem_Impl.h
 * \date 24-5-2009 18:26:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DRENDERSYSTEM_IMPL_H__
#define __OED3DRENDERSYSTEM_IMPL_H__

#include <IOERenderSystem.h>
#include "OED3DVertDecl_Impl.h"
#include "OED3DTexture_Impl.h"
#include "OED3DShader_Impl.h"

class COED3DRenderSystem_Impl : public IOERenderSystem
{
public:
	COED3DRenderSystem_Impl();
	virtual ~COED3DRenderSystem_Impl();

	virtual void SetShader(IOEShader* pShader);
	virtual IOEShader* GetShader() const;

	virtual bool SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat);
	virtual bool GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) const;

	virtual void DrawLineList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawLineStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);

	virtual void EnableLight(bool bEnable);
	virtual void EnableZBuffer(bool bEnable);
	virtual void EnableFog(bool bEnable);

	virtual void SetCullMode(CULL_MODE_TYPE eMode);
	virtual void SetFillMode(FILL_MODE eFillMode);
	virtual void SetFogInfo(uint nColor, float fNear, float fFar);
	virtual void SetSampleFilter(OE_SAMPLE_FILTER eSampleFilter);

private:
	bool Init();
	void Destroy();

	void DrawPrimitive(D3DPRIMITIVETYPE eType, const void* pVerts, uint nVerts, const ushort* pIndis, uint nPrimCount);

private:
	COED3DShader_Impl* m_pShader;

	CMatrix4x4 m_matWorld;
	CMatrix4x4 m_matView;
	CMatrix4x4 m_matProjection;

};

#endif // __OED3DRENDERSYSTEM_IMPL_H__
