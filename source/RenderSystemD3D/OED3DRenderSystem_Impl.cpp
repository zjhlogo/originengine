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
	return true;
}

void COED3DRenderSystem_Impl::Destroy()
{
	// TODO: 
}

bool COED3DRenderSystem_Impl::Initialize()
{
	m_pD3DBackBufferSurface = NULL;
	if (FAILED(g_pd3dDevice->GetRenderTarget(0, &m_pD3DBackBufferSurface))) return false;

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

bool COED3DRenderSystem_Impl::SetRenderTarget(IOETexture* pTexture)
{
	if (!pTexture) return false;

	if (pTexture->GetRtti()->GetTypeName() != TS("COED3DRenderTargetTexture_Impl")) return false;
	COED3DRenderTargetTexture_Impl* pRenderTarget = (COED3DRenderTargetTexture_Impl*)pTexture;

	IDirect3DTexture9* pD3DRenderTarget = pRenderTarget->GetTexture();
	IDirect3DSurface9* pD3DSurface = NULL;
	if (FAILED(pD3DRenderTarget->GetSurfaceLevel(0, &pD3DSurface))) return false;

	if (FAILED(g_pd3dDevice->SetRenderTarget(0, pD3DSurface))) return false;

	return true;
}

void COED3DRenderSystem_Impl::RestoreRenderTarget()
{
	g_pd3dDevice->SetRenderTarget(0, m_pD3DBackBufferSurface);
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
	}

	return true;
}

bool COED3DRenderSystem_Impl::GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) const
{
	bool bOK = false;

	if (eType & TT_WORLD)
	{
		matOut *= m_matWorld;
		bOK = true;
	}

	if (eType & TT_VIEW)
	{
		matOut *= m_matView;
		bOK = true;
	}

	if (eType & TT_PROJECTION)
	{
		matOut *= m_matProjection;
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

bool COED3DRenderSystem_Impl::RestoreRenderState()
{
	static const COED3DRenderState s_DefaultRenderState;
	m_CurrRenderState = s_DefaultRenderState;
	return true;
}

bool COED3DRenderSystem_Impl::PopRenderState()
{
	if (m_kRenderState.empty()) return false;

	m_CurrRenderState = m_kRenderState.top();
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

void COED3DRenderSystem_Impl::SetCullMode(CULL_MODE_TYPE eCullMode)
{
	m_CurrRenderState.m_eCullMode = eCullMode;
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
