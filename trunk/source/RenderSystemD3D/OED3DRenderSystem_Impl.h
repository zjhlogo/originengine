/*!
 * \file OED3DRenderSystem_Impl.h
 * \date 24-5-2009 18:26:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DRENDERSYSTEM_IMPL_H__
#define __OED3DRENDERSYSTEM_IMPL_H__

#include <OECore/IOERenderSystem.h>
#include <libOEMsg/OEMsgCommand.h>
#include "OED3DVertDecl_Impl.h"
#include "OED3DTexture_Impl.h"
#include "OED3DShader_Impl.h"
#include "OED3DRenderState.h"
#include <d3d9.h>

#include <stack>

class COED3DRenderSystem_Impl : public IOERenderSystem
{
public:
	typedef std::stack<COED3DRenderState> TK_RENDER_STATE;

public:
	RTTI_DEF(COED3DRenderSystem_Impl, IOERenderSystem);

	COED3DRenderSystem_Impl();
	virtual ~COED3DRenderSystem_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void SetShader(IOEShader* pShader);
	virtual IOEShader* GetShader() const;

	virtual bool BeginRenderTarget(IOETexture* pTexture);
	virtual void EndRenderTarget();
	virtual bool CopyBackbuffer(IOETexture* pTexture);

	virtual bool SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat);
	virtual bool GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) const;

	virtual void DrawLineList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawLineStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);

	virtual bool PushRenderState();
	virtual bool RestoreRenderState();
	virtual bool PopRenderState();

	virtual void EnableZBuffer(bool bEnable);
	virtual void EnableAlphaTest(bool bEnable);

	virtual void EnableFog(bool bEnable);
	virtual void SetFogInfo(uint nColor, float fNear, float fFar);

	virtual void SetCullMode(CULL_MODE_TYPE eCullMode);
	virtual void SetFillMode(FILL_MODE eFillMode);

private:
	bool Init();
	void Destroy();

	bool ApplyRenderState();
	bool DrawPrimitive(D3DPRIMITIVETYPE eType, const void* pVerts, uint nVerts, const ushort* pIndis, uint nPrimCount);

	bool OnPreRender3D(COEMsgCommand& msg);
	bool OnPostRender3D(COEMsgCommand& msg);

private:
	COED3DShader_Impl* m_pShader;
	IOETexture* m_pRenderTarget;
	IDirect3DSurface9* m_pD3DBackBufferSurface;

	TK_RENDER_STATE m_kRenderState;
	COED3DRenderState m_CurrRenderState;
	COED3DRenderState m_LastRenderState;

	CMatrix4x4 m_matWorld;
	CMatrix4x4 m_matView;
	CMatrix4x4 m_matProjection;

};

#endif // __OED3DRENDERSYSTEM_IMPL_H__
