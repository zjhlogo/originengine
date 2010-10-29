/*!
 * \file IOERenderSystem.h
 * \date 24-5-2009 17:14:26
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDERSYSTEM_H__
#define __IOERENDERSYSTEM_H__

#include "OEBaseTypeEx.h"
#include "IOEMgr.h"
#include "IOEVertDecl.h"
#include "IOETexture.h"
#include "IOEShader.h"

class IOERenderSystem : public IOEMgr
{
public:
	RTTI_DEF(IOERenderSystem, IOEMgr);

	IOERenderSystem() {};
	virtual ~IOERenderSystem() {};

	virtual void SetShader(IOEShader* pShader) = 0;
	virtual IOEShader* GetShader() const = 0;

	virtual bool BeginRenderTarget(IOETexture* pTexture) = 0;
	virtual void EndRenderTarget() = 0;
	virtual bool CopyBackbuffer(IOETexture* pTexture) = 0;

	virtual bool SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat) = 0;
	virtual bool GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) = 0;

	virtual void DrawLineList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawLineStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

	virtual bool PushRenderState() = 0;
	virtual bool RestoreRenderState(const tstring& strCommon = EMPTY_STRING) = 0;
	virtual bool PopRenderState() = 0;

	virtual void EnableZBuffer(bool bEnable) = 0;
	virtual void EnableAlphaTest(bool bEnable) = 0;

	virtual void EnableFog(bool bEnable) = 0;
	virtual void SetFogInfo(uint nColor, float fNear, float fFar) = 0;

	virtual void EnableClipPlane(bool bEnable) = 0;
	virtual void SetClipPlane(const CVector4& vClipPlane) = 0;
	virtual const CVector4& GetClipPlane() = 0;

	virtual void SetCullMode(CULL_MODE_TYPE eCullMode) = 0;
	virtual void LockCullMode(const tstring& strReason) = 0;
	virtual void UnlockCullMode() = 0;

	virtual void SetFillMode(FILL_MODE eFillMode) = 0;

};

extern IOERenderSystem* g_pOERenderSystem;

#endif // __IOERENDERSYSTEM_H__
