/*!
 * \file OED3DRenderSystem_Impl.cpp
 * \date 24-5-2009 18:26:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DRenderSystem_Impl.h"
#include "OED3DUtil.h"
#include <assert.h>

extern IDirect3DDevice9* g_pd3dDevice;

COED3DRenderSystem_Impl::COED3DRenderSystem_Impl()
{
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
	return true;
}

void COED3DRenderSystem_Impl::Destroy()
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

	matOut.Identity();

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

void COED3DRenderSystem_Impl::EnableLight(bool bEnable)
{
	if (bEnable) g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	else g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void COED3DRenderSystem_Impl::EnableZBuffer(bool bEnable)
{
	if (bEnable) g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	else g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
}

void COED3DRenderSystem_Impl::EnableFog(bool bEnable)
{
	if (bEnable)
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

void COED3DRenderSystem_Impl::SetCullMode(CULL_MODE_TYPE eMode)
{
	D3DCULL eD3DCull = COED3DUtil::ToD3DCullMode(eMode);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, eD3DCull);
}

void COED3DRenderSystem_Impl::SetFillMode(FILL_MODE eFillMode)
{
	D3DFILLMODE eD3DFill = COED3DUtil::ToD3DFillMode(eFillMode);
	g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, eD3DFill);
}

void COED3DRenderSystem_Impl::SetFogInfo(uint nColor, float fNear, float fFar)
{
	g_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, nColor);
	g_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&fNear);
	g_pd3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)&fFar);
}

void COED3DRenderSystem_Impl::SetSampleFilter(OE_SAMPLE_FILTER eSampleFilter)
{
	D3DTEXTUREFILTERTYPE eD3DSampleFilter = COED3DUtil::ToD3DSampleFilter(eSampleFilter);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, eD3DSampleFilter);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, eD3DSampleFilter);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, eD3DSampleFilter);
}

void COED3DRenderSystem_Impl::DrawPrimitive(D3DPRIMITIVETYPE eType, const void* pVerts, uint nVerts, const ushort* pIndis, uint nPrimCount)
{
	ID3DXEffect* pEffect = m_pShader->GetEffect();
	COED3DVertDecl_Impl* pVertDecl = (COED3DVertDecl_Impl*)m_pShader->GetVertDecl();

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
}
