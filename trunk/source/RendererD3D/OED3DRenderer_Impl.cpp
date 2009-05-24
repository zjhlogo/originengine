/*!
 * \file OED3DRenderer_Impl.cpp
 * \date 24-5-2009 18:26:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DRenderer_Impl.h"
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
