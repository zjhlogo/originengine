/*!
 * \file OEUIRenderer_Impl.cpp
 * \date 27-7-2009 18:06:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIRenderer_Impl.h"
#include <OEInterfaces.h>
#include <assert.h>

COEUIRenderer_Impl::COEUIRenderer_Impl()
{
	g_pOEUIRenderer = this;
	Init();
}

COEUIRenderer_Impl::~COEUIRenderer_Impl()
{
	Destroy();
	g_pOEUIRenderer = NULL;
}

void COEUIRenderer_Impl::Init()
{
	m_pDecl = NULL;
	m_pTexture = NULL;

	memset(m_pVertsCache, 0, sizeof(m_pVertsCache));
}

void COEUIRenderer_Impl::Destroy()
{
	SAFE_RELEASE(m_pDecl);
	for (int i = 0; i < VERTEX_CACHE_COUNT; ++i)
	{
		SAFE_DELETE(m_pVertsCache[i]);
	}
}

void COEUIRenderer_Impl::SetTexture(IOETexture* pTexture)
{
	m_pTexture = pTexture;
}

IOETexture* COEUIRenderer_Impl::GetTexture() const
{
	return m_pTexture;
}

void COEUIRenderer_Impl::DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	if (!m_pDecl) Create();

	COEUIVertexCache* pEmptyCache = NULL;
	COEUIVertexCache* pMatchCache = NULL;

	for (int i = 0; i < VERTEX_CACHE_COUNT; ++i)
	{
		if (m_pVertsCache[i]->Compare(m_pDecl, m_pTexture))
		{
			pMatchCache = m_pVertsCache[i];
			break;
		}

		if (!pEmptyCache && m_pVertsCache[i]->GetVertsCount() == 0)
		{
			pEmptyCache = m_pVertsCache[i];
		}
	}

	if (pMatchCache)
	{
		if (!pMatchCache->AddVerts(pVerts, nVerts, pIndis, nIndis))
		{
			pMatchCache->Flush();
			bool bOK = pMatchCache->AddVerts(pVerts, nVerts, pIndis, nIndis);
			assert(bOK);
		}
	}
	else if (pEmptyCache)
	{
		pEmptyCache->SetVertDecl(m_pDecl);
		pEmptyCache->SetTexture(m_pTexture);
		bool bOK = pEmptyCache->AddVerts(pVerts, nVerts, pIndis, nIndis);
		assert(bOK);
	}
	else
	{
		assert(false);
	}
}

void COEUIRenderer_Impl::FlushAll()
{
	for (int i = 0; i < VERTEX_CACHE_COUNT; ++i)
	{
		m_pVertsCache[i]->Flush();
		m_pVertsCache[i]->SetTexture(NULL);
		m_pVertsCache[i]->SetVertDecl(NULL);
	}
}

bool COEUIRenderer_Impl::Create()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT4, IOEVertDecl::U_POSITIONT, 0,
		IOEVertDecl::T_COLOR, IOEVertDecl::U_COLOR, 0,
		IOEVertDecl::T_FLOAT2, IOEVertDecl::U_TEXCOORD, 0,		// normal
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	if (!m_pDecl) return false;

	for (int i = 0; i < VERTEX_CACHE_COUNT; ++i)
	{
		m_pVertsCache[i] = new COEUIVertexCache(VERTEX_CACHE_SIZE, INDEX_CACHE_COUNT);
		if (!m_pVertsCache[i] || !m_pVertsCache[i]->IsOK()) return false;
	}

	return true;
}