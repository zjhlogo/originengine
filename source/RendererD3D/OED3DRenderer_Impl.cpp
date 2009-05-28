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
	g_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, nVerts, nIndis/3, pIndis, D3DFMT_INDEX16, pVerts, m_pVertDecl->GetStride());
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

void COED3DRenderer_Impl::SetTransform(TRANSFORM_TYPE eType, const CMatrix4x4& mat)
{
	D3DTRANSFORMSTATETYPE d3dType = COED3DUtil::ToD3DTransformType(eType);
	D3DXMATRIX matD3D;
	COED3DUtil::ToD3DXMatrix(matD3D, mat);
	g_pd3dDevice->SetTransform(d3dType, &matD3D);
}

void COED3DRenderer_Impl::GetTransform(CMatrix4x4& matOut, TRANSFORM_TYPE eType) const
{
	D3DTRANSFORMSTATETYPE d3dType = COED3DUtil::ToD3DTransformType(eType);
	D3DXMATRIX matD3D;
	g_pd3dDevice->GetTransform(d3dType, &matD3D);
	COED3DUtil::ToOEMatrix(matOut, matD3D);
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

void COED3DRenderer_Impl::SetCullMode(CULL_MODE_TYPE eMode)
{
	D3DCULL eD3DCull = COED3DUtil::ToD3DCullMode(eMode);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, eD3DCull);
}
