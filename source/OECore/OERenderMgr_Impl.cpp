/*!
 * \file OERenderMgr_Impl.cpp
 * \date 13-2-2010 13:06:51
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OERenderMgr_Impl.h"
#include "OERender/OESkinMeshRender_Impl.h"
#include "OERender/OESkeletonRender_Impl.h"
#include "OERender/OEBoundingBoxRender_Impl.h"

#include <assert.h>

COERenderMgr_Impl::COERenderMgr_Impl()
{
	assert(!g_pOERenderMgr);
	g_pOERenderMgr = this;
	m_bOK = true;
}

COERenderMgr_Impl::~COERenderMgr_Impl()
{
	g_pOERenderMgr = NULL;
}

bool COERenderMgr_Impl::Initialize()
{
	AddRenderHandle(new COESkinMeshRender_Impl());
	AddRenderHandle(new COESkeletonRender_Impl());
	AddRenderHandle(new COEBoundingBoxRender_Impl());

	return true;
}

void COERenderMgr_Impl::Terminate()
{
	for (TM_RENDER::iterator it = m_RenderMap.begin(); it != m_RenderMap.end(); ++it)
	{
		IOERender* pRender = it->second;
		SAFE_RELEASE(pRender);
	}
	m_RenderMap.clear();
}

bool COERenderMgr_Impl::AddRenderHandle(IOERender* pRender)
{
	if (GetRender(pRender->GetRtti()->GetClassName())) return false;

	m_RenderMap.insert(std::make_pair(pRender->GetRtti()->GetClassName(), pRender));
	return true;
}

IOERender* COERenderMgr_Impl::GetRender(const tstring& strClassName)
{
	TM_RENDER::iterator itfound = m_RenderMap.find(strClassName);
	if (itfound == m_RenderMap.end()) return NULL;

	return itfound->second;
}
