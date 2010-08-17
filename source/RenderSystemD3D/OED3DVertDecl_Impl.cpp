/*!
 * \file OED3DVertDecl_Impl.cpp
 * \date 24-5-2009 17:52:05
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DVertDecl_Impl.h"
#include "OED3DUtil.h"

#include <OEBase/IOELogFileMgr.h>
#include <assert.h>

extern IDirect3DDevice9* g_pd3dDevice;

COED3DVertDecl_Impl::COED3DVertDecl_Impl(const VERT_DECL_ELEMENT* pElement)
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
	memset(m_Element, 0, sizeof(m_Element));
	m_nElementCount = 0;
	m_nStride = 0;
	m_pDecl = NULL;
}

void COED3DVertDecl_Impl::Destroy()
{
	SAFE_RELEASE(m_pDecl);
}

int COED3DVertDecl_Impl::GetStrideSize() const
{
	return m_nStride;
}

const VERT_DECL_ELEMENT* COED3DVertDecl_Impl::GetElement() const
{
	return m_Element;
}

bool COED3DVertDecl_Impl::Compare(const VERT_DECL_ELEMENT* pElement) const
{
	int nElementCount = 0;
	while (pElement[nElementCount].eType != VDT_UNKNOWN) ++nElementCount;

	if (m_nElementCount != nElementCount) return false;

	for (uint i = 0; i < m_nElementCount; ++i)
	{
		if (m_Element[i].eType != pElement[i].eType
			|| m_Element[i].eUsage != pElement[i].eUsage
			|| m_Element[i].nIndex != pElement[i].nIndex) return false;
	}

	return true;
}

IDirect3DVertexDeclaration9* COED3DVertDecl_Impl::GetD3DVertDecl()
{
	return m_pDecl;
}

bool COED3DVertDecl_Impl::Create(const VERT_DECL_ELEMENT* pElement)
{
	static D3DVERTEXELEMENT9 s_D3DDecl[MAX_ELEMENT_COUNT];
	static const D3DVERTEXELEMENT9 END_ELEMENT = D3DDECL_END();

	// fill the element
	m_nElementCount = 0;
	while (pElement[m_nElementCount].eType != VDT_UNKNOWN)
	{
		assert(m_nElementCount < MAX_ELEMENT_COUNT);
		m_Element[m_nElementCount] = pElement[m_nElementCount];
		++m_nElementCount;
	}

	assert(m_nElementCount < MAX_ELEMENT_COUNT);
	m_Element[m_nElementCount] = pElement[m_nElementCount];
	s_D3DDecl[m_nElementCount] = END_ELEMENT;
	m_nStride = 0;

	// fill d3d declare
	for (uint i = 0; i < m_nElementCount; ++i)
	{
		s_D3DDecl[i].Stream = 0;													// Stream index
		s_D3DDecl[i].Offset = m_nStride;											// Offset in the stream in bytes
		s_D3DDecl[i].Type = COED3DUtil::ToD3DVertType(m_Element[i].eType);			// Data type
		s_D3DDecl[i].Method = D3DDECLMETHOD_DEFAULT;								// Processing method
		s_D3DDecl[i].Usage = COED3DUtil::ToD3DVertUsage(m_Element[i].eUsage);		// Semantics
		s_D3DDecl[i].UsageIndex = m_Element[i].nIndex;								// Semantic index

		m_nStride += COED3DUtil::GetVertTypeSize(m_Element[i].eType);
	}

	HRESULT hr = g_pd3dDevice->CreateVertexDeclaration(s_D3DDecl, &m_pDecl);
	if (FAILED(hr))
	{
		LOGOUT(TS("COED3DVertDecl_Impl::Create Failed"));
		return false;
	}

	return true;
}
