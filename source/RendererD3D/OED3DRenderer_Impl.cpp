/*!
 * \file OED3DRenderer_Impl.cpp
 * \date 24-5-2009 18:26:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DRenderer_Impl.h"
#include "OED3DUtil.h"
#include <assert.h>

extern IDirect3DDevice9* g_pd3dDevice;

COED3DRenderer_Impl::COED3DRenderer_Impl()
{
	g_pOERenderer = this;
	Init();
}

COED3DRenderer_Impl::~COED3DRenderer_Impl()
{
	Destroy();
	g_pOERenderer = NULL;
}

void COED3DRenderer_Impl::Init()
{
	m_pVertDecl = NULL;
	m_pTexture = NULL;
}

void COED3DRenderer_Impl::Destroy()
{
	// TODO: 
}

void COED3DRenderer_Impl::SetVertDecl(IOEVertDecl* pVertDecl)
{
	m_pVertDecl = (COED3DVertDecl_Impl*)pVertDecl;
	g_pd3dDevice->SetVertexDeclaration(m_pVertDecl->GetD3DVertDecl());
}

void COED3DRenderer_Impl::DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	assert(m_pVertDecl);
	HRESULT hRet = g_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, nVerts, nIndis/3, pIndis, D3DFMT_INDEX16, pVerts, m_pVertDecl->GetStride());
	if (FAILED(hRet))
	{
		// TODO: 
	}
}

void COED3DRenderer_Impl::DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	assert(m_pVertDecl);
	g_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, nVerts, nIndis-2, pIndis, D3DFMT_INDEX16, pVerts, m_pVertDecl->GetStride());
}

void COED3DRenderer_Impl::DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	assert(m_pVertDecl);
	g_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLEFAN, 0, nVerts, nIndis-2, pIndis, D3DFMT_INDEX16, pVerts, m_pVertDecl->GetStride());
}

bool COED3DRenderer_Impl::SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat)
{
	D3DTRANSFORMSTATETYPE d3dType = COED3DUtil::ToD3DTransformType(eType);
	if (d3dType == D3DTS_FORCE_DWORD) return false;

	D3DXMATRIX matD3D;
	COED3DUtil::ToD3DXMatrix(matD3D, mat);
	if (D3D_OK != g_pd3dDevice->SetTransform(d3dType, &matD3D)) return false;

	switch (eType)
	{
	case IOERenderer::TT_WORLD:
		m_matWorld = mat;
		break;
	case IOERenderer::TT_VIEW:
		m_matView = mat;
		break;
	case IOERenderer::TT_PROJECTION:
		m_matProjection = mat;
		break;
	}

	return true;
}

bool COED3DRenderer_Impl::GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) const
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

void COED3DRenderer_Impl::SetTexture(IOETexture* pTexture)
{
	IDirect3DTexture9* pD3DTexture = NULL;
	m_pTexture = (COED3DTexture_Impl*)pTexture;
	if (m_pTexture) pD3DTexture = m_pTexture->GetTexture();

	g_pd3dDevice->SetTexture(0, pD3DTexture);
}

IOETexture* COED3DRenderer_Impl::GetTexture() const
{
	return m_pTexture;
}

void COED3DRenderer_Impl::EnableLight(bool bEnable)
{
	if (bEnable) g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	else g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void COED3DRenderer_Impl::EnableZBuffer(bool bEnable)
{
	if (bEnable) g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	else g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
}

void COED3DRenderer_Impl::EnableFog(bool bEnable)
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

void COED3DRenderer_Impl::SetCullMode(CULL_MODE_TYPE eMode)
{
	D3DCULL eD3DCull = COED3DUtil::ToD3DCullMode(eMode);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, eD3DCull);
}

void COED3DRenderer_Impl::SetFillMode(FILL_MODE eFillMode)
{
	D3DFILLMODE eD3DFill = COED3DUtil::ToD3DFillMode(eFillMode);
	g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, eD3DFill);
}

void COED3DRenderer_Impl::SetFogInfo(uint nColor, float fNear, float fFar)
{
	g_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, nColor);
	g_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&fNear);
	g_pd3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)&fFar);
}
