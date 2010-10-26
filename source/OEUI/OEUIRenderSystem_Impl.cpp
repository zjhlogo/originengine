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

	memset(m_pVertsCache, 0, sizeof(m_pVertsCache));
	m_pScreen = NULL;

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

	for (int i = 0; i < VERTEX_CACHE_COUNT; ++i)
	{
		m_pVertsCache[i] = new COEUIVertexCache(VERTEX_CACHE_SIZE, INDEX_CACHE_COUNT);
		if (!m_pVertsCache[i] || !m_pVertsCache[i]->IsOK()) return false;
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
	for (int i = 0; i < VERTEX_CACHE_COUNT; ++i)
	{
		SAFE_DELETE(m_pVertsCache[i]);
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

void COEUIRenderSystem_Impl::DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	COEUIVertexCache* pEmptyCache = NULL;
	COEUIVertexCache* pMatchCache = NULL;

	for (int i = 0; i < VERTEX_CACHE_COUNT; ++i)
	{
		if (m_pVertsCache[i]->Compare(m_pTexture, m_pShader))
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

void COEUIRenderSystem_Impl::FlushAll()
{
	for (int i = 0; i < VERTEX_CACHE_COUNT; ++i)
	{
		m_pVertsCache[i]->Flush();
		m_pVertsCache[i]->SetTexture(NULL);
		m_pVertsCache[i]->SetShader(NULL);
	}
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
	return true;
}
