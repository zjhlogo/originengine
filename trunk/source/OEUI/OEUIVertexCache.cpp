/*!
 * \file OEUIVertexCache.cpp
 * \date 28-7-2009 11:21:31
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIVertexCache.h"
#include <OECore/IOERenderSystem.h>
#include <OECore/OERenderSystemUtil.h>
#include <assert.h>

COEUIVertexCache::COEUIVertexCache(uint nVertsCacheSize, uint nIndisCacheCount)
{
	Init();
	m_bOK = Create(nVertsCacheSize, nIndisCacheCount);
}

COEUIVertexCache::~COEUIVertexCache()
{
	Destroy();
}

void COEUIVertexCache::Init()
{
	m_nVertsCacheSize = 0;
	m_nMaxVertsCount = 0;
	m_nMaxIndisCount = 0;
	m_nVertsCount = 0;
	m_nIndisCount = 0;
	m_nStrideSize = 0;

	m_pVertsCache = NULL;
	m_pIndisCache = NULL;

	m_pTexture = NULL;
	m_pShader = NULL;

	m_bOK = false;
}

void COEUIVertexCache::Destroy()
{
	SAFE_DELETE_ARRAY(m_pVertsCache);
	SAFE_DELETE_ARRAY(m_pIndisCache);

	m_nVertsCacheSize = 0;
	m_nMaxVertsCount = 0;
	m_nMaxIndisCount = 0;
	m_nVertsCount = 0;
	m_nIndisCount = 0;
	m_nStrideSize = 0;
}

void COEUIVertexCache::SetTexture(IOETexture* pTexture)
{
	m_pTexture = pTexture;
}

IOETexture* COEUIVertexCache::GetTexture() const
{
	return m_pTexture;
}

void COEUIVertexCache::SetShader(IOEShader* pShader)
{
	assert(m_nVertsCount == 0 && m_nIndisCount == 0);

	m_pShader = pShader;

	m_nStrideSize = 0;
	m_nMaxVertsCount = 0;

	if (m_pShader)
	{
		m_nStrideSize = m_pShader->GetVertDecl()->GetStrideSize();
		m_nMaxVertsCount = m_nVertsCacheSize/m_nStrideSize;
	}
}

IOEShader* COEUIVertexCache::GetShader() const
{
	return m_pShader;
}

bool COEUIVertexCache::AddVerts(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	if (m_nVertsCount + nVerts > m_nMaxVertsCount) return false;
	if (m_nIndisCount + nIndis > m_nMaxIndisCount) return false;

	uchar* pVertsCache = (uchar*)m_pVertsCache + m_nVertsCount*m_nStrideSize;
	memcpy(pVertsCache, pVerts, nVerts*m_nStrideSize);

	for (uint i = 0; i < nIndis; ++i)
	{
		m_pIndisCache[m_nIndisCount+i] = m_nVertsCount+pIndis[i];
	}

	m_nVertsCount += nVerts;
	m_nIndisCount += nIndis;

	return true;
}

void COEUIVertexCache::Flush()
{
	if (m_pShader)
	{
		CDefaultRenderState DefaultState;
		g_pOERenderSystem->EnableAlphaTest(true);

		m_pShader->SetTexture(TS("g_texDiffuse"), m_pTexture);
		g_pOERenderSystem->SetShader(m_pShader);
		g_pOERenderSystem->DrawTriList(m_pVertsCache, m_nVertsCount, m_pIndisCache, m_nIndisCount);
	}

	m_nVertsCount = 0;
	m_nIndisCount = 0;
}

bool COEUIVertexCache::Compare(IOETexture* pTexture, IOEShader* pShader)
{
	if (m_pTexture != pTexture) return false;
	if (m_pShader != pShader) return false;
	return true;
}

int COEUIVertexCache::GetVertsCount()
{
	return m_nVertsCount;
}

int COEUIVertexCache::GetIndisCount()
{
	return m_nIndisCount;
}

bool COEUIVertexCache::IsOK()
{
	return m_bOK;
}

bool COEUIVertexCache::Create(uint nVertsCacheSize, uint nIndisCacheCount)
{
	Destroy();

	m_pVertsCache = new uchar[nVertsCacheSize];
	if (!m_pVertsCache) return false;

	m_pIndisCache = new ushort[nIndisCacheCount];
	if (!m_pIndisCache) return false;

	m_nVertsCacheSize = nVertsCacheSize;
	m_nMaxVertsCount = 0;
	m_nMaxIndisCount = nIndisCacheCount;
	m_nVertsCount = 0;
	m_nIndisCount = 0;
	m_nStrideSize = 0;

	return true;
}
