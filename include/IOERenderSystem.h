/*!
 * \file IOERenderSystem.h
 * \date 24-5-2009 17:14:26
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDERSYSTEM_H__
#define __IOERENDERSYSTEM_H__

#include "IOEObject.h"
#include "IOEVertDecl.h"
#include "IOETexture.h"
#include "IOEShader.h"

#include "OEBaseTypeEx.h"

class IOERenderSystem : public IOEObject
{
public:
	IOERenderSystem() {};
	virtual ~IOERenderSystem() {};

	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

	virtual void SetShader(IOEShader* pShader) = 0;
	virtual IOEShader* GetShader() const = 0;

	virtual bool SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat) = 0;
	virtual bool GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) const = 0;

	virtual void DrawLineList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawLineStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

	virtual bool PushRenderState() = 0;
	virtual bool RestoreRenderState() = 0;
	virtual bool PopRenderState() = 0;

	virtual void EnableZBuffer(bool bEnable) = 0;
	virtual void EnableAlphaTest(bool bEnable) = 0;

	virtual void EnableFog(bool bEnable) = 0;
	virtual void SetFogInfo(uint nColor, float fNear, float fFar) = 0;

	virtual void SetCullMode(CULL_MODE_TYPE eCullMode) = 0;
	virtual void SetFillMode(FILL_MODE eFillMode) = 0;

};

extern IOERenderSystem* g_pOERenderSystem;

#endif // __IOERENDERSYSTEM_H__
