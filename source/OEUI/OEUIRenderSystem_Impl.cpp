/*!
 * \file OEUIRenderSystem_Impl.cpp
 * \date 19-2-2010 19:53:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIRenderSystem_Impl.h"
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOEDevice.h>
#include <OEBase/IOEMsgMgr.h>
#include <libOEMsg/OEMsgList.h>
#include <assert.h>

COEUIRenderSystem_Impl::COEUIRenderSystem_Impl()
{
	assert(!g_pOEUIRenderSystem);
	g_pOEUIRenderSystem = this;
	m_bOK = Init();
}

COEUIRenderSystem_Impl::~COEUIRenderSystem_Impl()
{
	Destroy();
	g_pOEUIRenderSystem = NULL;
}

bool COEUIRenderSystem_Impl::Init()
{
	m_pTexture = NULL;
	m_pShader = NULL;

	memset(m_pSolidVertsCache, 0, sizeof(m_pSolidVertsCache));
	memset(m_pTransparentVertsCache, 0, sizeof(m_pTransparentVertsCache));

	m_pScreen = NULL;

	m_fCurrDepth = 1.0f;
	return true;
}

void COEUIRenderSystem_Impl::Destroy()
{
	// TODO: 
}

bool COEUIRenderSystem_Impl::Initialize()
{
	m_pShader = g_pOEShaderMgr->CreateDefaultShader(DST_POLY_UI);
	if (!m_pShader) return false;

	for (int i = 0; i < NUM_SOLID_CACHE; ++i)
	{
		m_pSolidVertsCache[i] = new COEUIVertexCache(SOLID_VERTEX_CACHE, SOLID_INDEX_CACHE);
		if (!m_pSolidVertsCache[i] || !m_pSolidVertsCache[i]->IsOK()) return false;
	}

	for (int i = 0; i < NUM_TRANSPARENT_CACHE; ++i)
	{
		m_pTransparentVertsCache[i] = new COEUIVertexCache(TRANSPARENT_VERTEX_CACHE, TRANSPARENT_INDEX_CACHE);
		if (!m_pTransparentVertsCache[i] || !m_pTransparentVertsCache[i]->IsOK()) return false;
	}

	m_pScreen = new COEUIScreen();
	if (!m_pScreen || !m_pScreen->IsOK()) return false;

	g_pOEDevice->RegisterEvent(OMI_UI_UPDATE, this, (MSG_FUNC)&COEUIRenderSystem_Impl::OnUpdate);
	g_pOEDevice->RegisterEvent(OMI_UI_RENDER, this, (MSG_FUNC)&COEUIRenderSystem_Impl::OnRender);
	g_pOEDevice->RegisterEvent(OMI_UI_POST_RENDER, this, (MSG_FUNC)&COEUIRenderSystem_Impl::OnPostRender);

	return true;
}

void COEUIRenderSystem_Impl::Terminate()
{
	SAFE_RELEASE(m_pScreen);
	SAFE_RELEASE(m_pShader);

	for (int i = 0; i < NUM_TRANSPARENT_CACHE; ++i)
	{
		SAFE_DELETE(m_pTransparentVertsCache[i]);
	}

	for (int i = 0; i < NUM_SOLID_CACHE; ++i)
	{
		SAFE_DELETE(m_pSolidVertsCache[i]);
	}
}

COEUIScreen* COEUIRenderSystem_Impl::GetScreen()
{
	return m_pScreen;
}

void COEUIRenderSystem_Impl::SetTexture(IOETexture* pTexture)
{
	m_pTexture = pTexture;
}

IOETexture* COEUIRenderSystem_Impl::GetTexture() const
{
	return m_pTexture;
}

void COEUIRenderSystem_Impl::DrawSolidTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	COEUIVertexCache* pEmptyCache = NULL;
	COEUIVertexCache* pMatchCache = NULL;

	for (int i = 0; i < NUM_SOLID_CACHE; ++i)
	{
		if (m_pSolidVertsCache[i]->Compare(m_pTexture, m_pShader))
		{
			pMatchCache = m_pSolidVertsCache[i];
			break;
		}

		if (!pEmptyCache && m_pSolidVertsCache[i]->GetVertsCount() == 0)
		{
			pEmptyCache = m_pSolidVertsCache[i];
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
		pEmptyCache->SetTexture(m_pTexture);
		pEmptyCache->SetShader(m_pShader);
		bool bOK = pEmptyCache->AddVerts(pVerts, nVerts, pIndis, nIndis);
		assert(bOK);
	}
	else
	{
		assert(false);
	}
}

void COEUIRenderSystem_Impl::DrawTransparentTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	COEUIVertexCache* pEmptyCache = NULL;
	COEUIVertexCache* pMatchCache = NULL;

	for (int i = 0; i < NUM_TRANSPARENT_CACHE; ++i)
	{
		if (m_pTransparentVertsCache[i]->Compare(m_pTexture, m_pShader))
		{
			pMatchCache = m_pTransparentVertsCache[i];
			break;
		}

		if (!pEmptyCache && m_pTransparentVertsCache[i]->GetVertsCount() == 0)
		{
			pEmptyCache = m_pTransparentVertsCache[i];
		}
	}

	if (pMatchCache)
	{
		if (!pMatchCache->AddVerts(pVerts, nVerts, pIndis, nIndis))
		{
			FlushSolid();
			pMatchCache->Flush();
			bool bOK = pMatchCache->AddVerts(pVerts, nVerts, pIndis, nIndis);
			assert(bOK);
		}
	}
	else if (pEmptyCache)
	{
		pEmptyCache->SetTexture(m_pTexture);
		pEmptyCache->SetShader(m_pShader);
		bool bOK = pEmptyCache->AddVerts(pVerts, nVerts, pIndis, nIndis);
		assert(bOK);
	}
	else
	{
		assert(false);
	}
}

float COEUIRenderSystem_Impl::NextDepth()
{
	float fLastDepth = m_fCurrDepth;
	m_fCurrDepth -= COEMath::TOL;
	return fLastDepth;
}

void COEUIRenderSystem_Impl::FlushSolid()
{
	for (int i = 0; i < NUM_SOLID_CACHE; ++i)
	{
		m_pSolidVertsCache[i]->Flush();
		m_pSolidVertsCache[i]->SetTexture(NULL);
		m_pSolidVertsCache[i]->SetShader(NULL);
	}
}

void COEUIRenderSystem_Impl::FlushTransparent()
{
	for (int i = 0; i < NUM_TRANSPARENT_CACHE; ++i)
	{
		m_pTransparentVertsCache[i]->Flush();
		m_pTransparentVertsCache[i]->SetTexture(NULL);
		m_pTransparentVertsCache[i]->SetShader(NULL);
	}
}

void COEUIRenderSystem_Impl::FlushAll()
{
	FlushSolid();
	FlushTransparent();
}

bool COEUIRenderSystem_Impl::OnUpdate(COEMsgCommand& msg)
{
	float fDetailTime = g_pOEDevice->GetDetailTime();
	m_pScreen->Update(fDetailTime);
	m_pScreen->UpdateChildren(fDetailTime);
	return true;
}

bool COEUIRenderSystem_Impl::OnRender(COEMsgCommand& msg)
{
	float fDetailTime = g_pOEDevice->GetDetailTime();
	m_pScreen->Render(fDetailTime);
	m_pScreen->RenderChildren(fDetailTime);
	return true;
}

bool COEUIRenderSystem_Impl::OnPostRender(COEMsgCommand& msg)
{
	FlushAll();
	m_fCurrDepth = 1.0f;
	return true;
}
