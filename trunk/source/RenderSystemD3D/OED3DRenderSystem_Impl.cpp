/*!
 * \file OED3DRenderSystem_Impl.cpp
 * \date 24-5-2009 18:26:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DRenderSystem_Impl.h"
#include "OED3DRenderTargetTexture_Impl.h"
#include "OED3DUtil.h"

#include <OEBase/IOEMsgMgr.h>
#include <OECore/IOEDevice.h>
#include <libOEMsg/OEMsgList.h>
#include <libOEMsg/OEMsgRenderState.h>
#include <libOEMsg/OEMsgTransform.h>

#include <assert.h>

extern IDirect3DDevice9* g_pd3dDevice;

COED3DRenderSystem_Impl::COED3DRenderSystem_Impl()
{
	assert(!g_pOERenderSystem);
	g_pOERenderSystem = this;
	m_bOK = Init();
}

COED3DRenderSystem_Impl::~COED3DRenderSystem_Impl()
{
	Destroy();
	g_pOERenderSystem = NULL;
}

bool COED3DRenderSystem_Impl::Init()
{
	m_pShader = NULL;
	m_pRenderTarget = NULL;
	m_pD3DBackBufferSurface = NULL;
	m_pD3DBackBufferDepthStencilSurface = NULL;
	m_bLockCullMode = false;
	return true;
}

void COED3DRenderSystem_Impl::Destroy()
{
	// TODO: 
}

bool COED3DRenderSystem_Impl::Initialize()
{
	if (FAILED(g_pd3dDevice->GetRenderTarget(0, &m_pD3DBackBufferSurface))) return false;
	if (FAILED(g_pd3dDevice->GetDepthStencilSurface(&m_pD3DBackBufferDepthStencilSurface))) return false;

	m_LastRenderState.m_bZBuffer = true;
	m_LastRenderState.m_bAlphaTest = false;
	m_LastRenderState.m_bFog = false;
	m_LastRenderState.m_nFogColor;
	m_LastRenderState.m_fFogNear;
	m_LastRenderState.m_fFogFar;
	m_LastRenderState.m_eFillMode = FM_SOLID;
	m_LastRenderState.m_eCullMode = CMT_CCW;
	m_LastRenderState.m_bEnableClipPlane = false;
	m_LastRenderState.m_vClipPlane.Init(0.0f, 0.0f, 0.0f, 0.0f);

	g_pOEDevice->RegisterEvent(OMI_PRE_RENDER, this, (MSG_FUNC)&COED3DRenderSystem_Impl::OnPreRender3D);
	g_pOEDevice->RegisterEvent(OMI_POST_RENDER, this, (MSG_FUNC)&COED3DRenderSystem_Impl::OnPostRender3D);

	return true;
}

void COED3DRenderSystem_Impl::Terminate()
{
	// TODO: 
}

void COED3DRenderSystem_Impl::SetShader(IOEShader* pShader)
{
	m_pShader = (COED3DShader_Impl*)pShader;
}

IOEShader* COED3DRenderSystem_Impl::GetShader() const
{
	return m_pShader;
}

bool COED3DRenderSystem_Impl::BeginRenderTarget(IOETexture* pTexture)
{
	if (m_pRenderTarget) return false;

	if (!pTexture) return false;
	if (pTexture->GetRtti()->GetTypeName() != TS("COED3DRenderTargetTexture_Impl")) return false;
	COED3DRenderTargetTexture_Impl* pRenderTarget = (COED3DRenderTargetTexture_Impl*)pTexture;

	if (FAILED(g_pd3dDevice->SetRenderTarget(0, pRenderTarget->GetSurface()))) return false;
	if (FAILED(g_pd3dDevice->SetDepthStencilSurface(pRenderTarget->GetDepthStencilSurface()))) return false;

	m_pRenderTarget = pTexture;
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	return true;
}

void COED3DRenderSystem_Impl::EndRenderTarget()
{
	g_pd3dDevice->SetDepthStencilSurface(m_pD3DBackBufferDepthStencilSurface);
	g_pd3dDevice->SetRenderTarget(0, m_pD3DBackBufferSurface);
	m_pRenderTarget = NULL;
}

bool COED3DRenderSystem_Impl::CopyBackbuffer(IOETexture* pTexture)
{
	if (!pTexture) return false;

	if (pTexture->GetRtti()->GetTypeName() != TS("COED3DRenderTargetTexture_Impl")) return false;
	COED3DRenderTargetTexture_Impl* pRenderTarget = (COED3DRenderTargetTexture_Impl*)pTexture;

	HRESULT hRet = g_pd3dDevice->StretchRect(m_pD3DBackBufferSurface, 0, pRenderTarget->GetSurface(), 0, D3DTEXF_NONE);
	if (FAILED(hRet)) return false;

	return true;
}

bool COED3DRenderSystem_Impl::SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat)
{
	D3DTRANSFORMSTATETYPE d3dType = COED3DUtil::ToD3DTransformType(eType);
	if (d3dType == D3DTS_FORCE_DWORD) return false;

	D3DXMATRIX matD3D;
	COED3DUtil::ToD3DXMatrix(matD3D, mat);
	if (D3D_OK != g_pd3dDevice->SetTransform(d3dType, &matD3D)) return false;

	switch (eType)
	{
	case TT_WORLD:
		m_matWorld = mat;
		break;
	case TT_VIEW:
		m_matView = mat;
		break;
	case TT_PROJECTION:
		m_matProjection = mat;
		break;
	default:
		return false;
		break;
	}

	return true;
}

bool COED3DRenderSystem_Impl::GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType)
{
	bool bOK = false;

	if (eType & TT_WORLD)
	{
		COEMsgTransform msg(OMI_GET_TRANSFORM, m_matWorld, TT_WORLD);
		CallEvent(msg);
		matOut *= msg.GetMatrix();
		bOK = true;
	}

	if (eType & TT_VIEW)
	{
		COEMsgTransform msg(OMI_GET_TRANSFORM, m_matView, TT_VIEW);
		CallEvent(msg);
		matOut *= msg.GetMatrix();
		bOK = true;
	}

	if (eType & TT_PROJECTION)
	{
		COEMsgTransform msg(OMI_GET_TRANSFORM, m_matProjection, TT_PROJECTION);
		CallEvent(msg);
		matOut *= msg.GetMatrix();
		bOK = true;
	}

	return bOK;
}

void COED3DRenderSystem_Impl::DrawLineList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	DrawPrimitive(D3DPT_LINELIST, pVerts, nVerts, pIndis, nIndis/2);
}

void COED3DRenderSystem_Impl::DrawLineStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	DrawPrimitive(D3DPT_LINESTRIP, pVerts, nVerts, pIndis, nIndis-1);
}

void COED3DRenderSystem_Impl::DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	DrawPrimitive(D3DPT_TRIANGLELIST, pVerts, nVerts, pIndis, nIndis/3);
}

void COED3DRenderSystem_Impl::DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	DrawPrimitive(D3DPT_TRIANGLESTRIP, pVerts, nVerts, pIndis, nIndis-2);
}

void COED3DRenderSystem_Impl::DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	DrawPrimitive(D3DPT_TRIANGLEFAN, pVerts, nVerts, pIndis, nIndis-2);
}

bool COED3DRenderSystem_Impl::PushRenderState()
{
	m_kRenderState.push(m_CurrRenderState);
	return true;
}

bool COED3DRenderSystem_Impl::RestoreRenderState(const tstring& strCommon /* = EMPTY_STRING */)
{
	static const COERenderState s_DefaultRenderState;

	COEMsgRenderState msg(s_DefaultRenderState, strCommon);
	CallEvent(msg);

	m_CurrRenderState = msg.GetRenderState();

	return true;
}

bool COED3DRenderSystem_Impl::PopRenderState()
{
	if (m_kRenderState.empty()) return false;

	CULL_MODE_TYPE eCullMode = m_CurrRenderState.m_eCullMode;
	m_CurrRenderState = m_kRenderState.top();
	if (m_bLockCullMode) m_CurrRenderState.m_eCullMode = eCullMode;

	m_kRenderState.pop();
	return true;
}

void COED3DRenderSystem_Impl::EnableZBuffer(bool bEnable)
{
	m_CurrRenderState.m_bZBuffer = bEnable;
}

void COED3DRenderSystem_Impl::EnableAlphaTest(bool bEnable)
{
	m_CurrRenderState.m_bAlphaTest = bEnable;
}

void COED3DRenderSystem_Impl::EnableFog(bool bEnable)
{
	m_CurrRenderState.m_bFog = bEnable;
}

void COED3DRenderSystem_Impl::SetFogInfo(uint nColor, float fNear, float fFar)
{
	m_CurrRenderState.m_nFogColor = nColor;
	m_CurrRenderState.m_fFogNear = fNear;
	m_CurrRenderState.m_fFogFar = fFar;
}

void COED3DRenderSystem_Impl::EnableClipPlane(bool bEnable)
{
	m_CurrRenderState.m_bEnableClipPlane = bEnable;
}

void COED3DRenderSystem_Impl::SetClipPlane(const CVector4& vClipPlane)
{
	m_CurrRenderState.m_vClipPlane = vClipPlane;
}

const CVector4& COED3DRenderSystem_Impl::GetClipPlane()
{
	return m_CurrRenderState.m_vClipPlane;
}

void COED3DRenderSystem_Impl::SetCullMode(CULL_MODE_TYPE eCullMode)
{
	if (!m_bLockCullMode) m_CurrRenderState.m_eCullMode = eCullMode;
}

void COED3DRenderSystem_Impl::LockCullMode(const tstring& strReason)
{
	m_bLockCullMode = true;
}

void COED3DRenderSystem_Impl::UnlockCullMode()
{
	m_bLockCullMode = false;
}

void COED3DRenderSystem_Impl::SetFillMode(FILL_MODE eFillMode)
{
	m_CurrRenderState.m_eFillMode = eFillMode;
}

bool COED3DRenderSystem_Impl::ApplyRenderState()
{
	// apply z-buffer
	if (m_LastRenderState.m_bZBuffer != m_CurrRenderState.m_bZBuffer)
	{
		if (m_CurrRenderState.m_bZBuffer)
		{
			g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		}
		else
		{
			g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		}
	}

	// apply alpha test
	if (m_LastRenderState.m_bAlphaTest != m_CurrRenderState.m_bAlphaTest)
	{
		if (m_CurrRenderState.m_bAlphaTest)
		{
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		else
		{
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}
	}

	// apply fog
	if (m_LastRenderState.m_bFog != m_CurrRenderState.m_bFog)
	{
		if (m_CurrRenderState.m_bFog)
		{
			g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
			// 这里不要用 D3DRS_FOGVERTEXMODE, 否则用 Shader 渲染时, 什么都看不见
			g_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		}
		else
		{
			g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
	}

	// apply fog color
	if (m_LastRenderState.m_nFogColor != m_CurrRenderState.m_nFogColor)
	{
		g_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, m_CurrRenderState.m_nFogColor);
	}

	// apply fog near
	if (fabsf(m_LastRenderState.m_fFogNear - m_CurrRenderState.m_fFogNear) > 0.1f)
	{
		g_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&m_CurrRenderState.m_fFogNear);
	}

	// apply fog far
	if (fabsf(m_LastRenderState.m_fFogFar - m_CurrRenderState.m_fFogFar) > 0.1f)
	{
		g_pd3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)&m_CurrRenderState.m_fFogFar);
	}

	// apply cull mode
	if (m_LastRenderState.m_eCullMode != m_CurrRenderState.m_eCullMode)
	{
		D3DCULL eD3DCull = COED3DUtil::ToD3DCullMode(m_CurrRenderState.m_eCullMode);
		g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, eD3DCull);
	}

	// apply fill mode
	if (m_LastRenderState.m_eFillMode != m_CurrRenderState.m_eFillMode)
	{
		D3DFILLMODE eD3DFill = COED3DUtil::ToD3DFillMode(m_CurrRenderState.m_eFillMode);
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, eD3DFill);
	}

	// apply clip plane
	if (m_LastRenderState.m_bEnableClipPlane != m_CurrRenderState.m_bEnableClipPlane)
	{
		if (m_CurrRenderState.m_bEnableClipPlane)
		{
			g_pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, TRUE);
		}
		else
		{
			g_pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
		}
	}

	// apply clip plane data
	if (fabsf(m_LastRenderState.m_vClipPlane.x-m_CurrRenderState.m_vClipPlane.x) > COEMath::TOL
		|| fabsf(m_LastRenderState.m_vClipPlane.y-m_CurrRenderState.m_vClipPlane.y) > COEMath::TOL
		|| fabsf(m_LastRenderState.m_vClipPlane.z-m_CurrRenderState.m_vClipPlane.z) > COEMath::TOL
		|| fabsf(m_LastRenderState.m_vClipPlane.w-m_CurrRenderState.m_vClipPlane.w) > COEMath::TOL)
	{
		float pClipSpacePlane[4];
		pClipSpacePlane[0] = m_CurrRenderState.m_vClipPlane.x;
		pClipSpacePlane[1] = m_CurrRenderState.m_vClipPlane.y;
		pClipSpacePlane[2] = m_CurrRenderState.m_vClipPlane.z;
		pClipSpacePlane[3] = m_CurrRenderState.m_vClipPlane.w;
		g_pd3dDevice->SetClipPlane(0, pClipSpacePlane);
	}

	m_LastRenderState = m_CurrRenderState;
	return true;
}

bool COED3DRenderSystem_Impl::DrawPrimitive(D3DPRIMITIVETYPE eType, const void* pVerts, uint nVerts, const ushort* pIndis, uint nPrimCount)
{
	if (!m_pShader) return false;

	ID3DXEffect* pEffect = m_pShader->GetEffect();
	if (!pEffect) return false;

	COED3DVertDecl_Impl* pVertDecl = (COED3DVertDecl_Impl*)m_pShader->GetVertDecl();
	if (!pVertDecl) return false;

	if (!ApplyRenderState()) return false;

	uint nPass = 0;
	pEffect->Begin(&nPass, 0);
	for (uint i = 0; i < nPass; ++i)
	{
		pEffect->BeginPass(i);
		pEffect->CommitChanges();

		g_pd3dDevice->SetVertexDeclaration(pVertDecl->GetD3DVertDecl());
		HRESULT hRet = g_pd3dDevice->DrawIndexedPrimitiveUP(eType, 0, nVerts, nPrimCount, pIndis, D3DFMT_INDEX16, pVerts, pVertDecl->GetStrideSize());
		pEffect->EndPass();
	}
	pEffect->End();

	return true;
}

bool COED3DRenderSystem_Impl::OnPreRender3D(COEMsgCommand& msg)
{
	this->SetShader(NULL);
	return true;
}

bool COED3DRenderSystem_Impl::OnPostRender3D(COEMsgCommand& msg)
{
	this->SetShader(NULL);
	return true;
}
