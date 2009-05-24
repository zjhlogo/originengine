/*!
 * \file OED3DVertDecl_Impl.cpp
 * \date 24-5-2009 17:52:05
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DVertDecl_Impl.h"
#include "OED3DUtil.h"

extern IDirect3DDevice9* g_pd3dDevice;

COED3DVertDecl_Impl::COED3DVertDecl_Impl(const ELEMENT* pElement)
{
	Init();
	m_bOK = Create(pElement);
}

COED3DVertDecl_Impl::~COED3DVertDecl_Impl()
{
	Destroy();
}

void COED3DVertDecl_Impl::Init()
{
	m_nStride = 0;
	m_pDecl = NULL;
}

void COED3DVertDecl_Impl::Destroy()
{
	SAFE_RELEASE(m_pDecl);
}

bool COED3DVertDecl_Impl::Create(const ELEMENT* pElement)
{
	static D3DVERTEXELEMENT9 s_D3DDecl[IOEVertDecl::MAX_ELEMENT_COUNT];
	static const D3DVERTEXELEMENT9 END_ELEMENT = D3DDECL_END();

	int nElementCount = 0;
	while (pElement[nElementCount].eType != IOEVertDecl::T_UNKNOWN) ++nElementCount;
	if (nElementCount+1 >= IOEVertDecl::MAX_ELEMENT_COUNT)
	{
		// TODO: logout failed
		return false;
	}

	s_D3DDecl[nElementCount] = END_ELEMENT;
	int nOffset = 0;

	for (int i = 0; i < nElementCount; ++i)
	{
		s_D3DDecl[i].Stream = 0;													// Stream index
		s_D3DDecl[i].Offset = nOffset;												// Offset in the stream in bytes
		s_D3DDecl[i].Type = COED3DUtil::ToD3DVertType(pElement[i].eType);			// Data type
		s_D3DDecl[i].Method = D3DDECLMETHOD_DEFAULT;								// Processing method
		s_D3DDecl[i].Usage = COED3DUtil::ToD3DVertUsage(pElement[i].eUsage);		// Semantics
		s_D3DDecl[i].UsageIndex = pElement[i].nIndex;								// Semantic index

		nOffset += COED3DUtil::GetVertTypeSize(pElement[i].eType);
	}

	HRESULT hr = S_OK;

	hr = g_pd3dDevice->CreateVertexDeclaration(s_D3DDecl, &m_pDecl);
	if (FAILED(hr))
	{
		// TODO: logout
		return false;
	}

	m_nStride = nOffset;

	return true;
}

int COED3DVertDecl_Impl::GetStride()
{
	return m_nStride;
}

IDirect3DVertexDeclaration9* COED3DVertDecl_Impl::GetD3DVertDecl()
{
	return m_pDecl;
}
