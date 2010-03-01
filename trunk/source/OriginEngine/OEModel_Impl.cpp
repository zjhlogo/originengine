/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"
#include "OESkinMeshRenderData_Impl.h"
#include <IOEXmlMgr.h>
#include <IOEControlMgr.h>
#include <IOERenderMgr.h>

COEModel_Impl::COEModel_Impl(const tstring& strFile)
{
	Init();
	m_bOK = Create(strFile);
}

COEModel_Impl::~COEModel_Impl()
{
	Destroy();
}

void COEModel_Impl::Init()
{
	m_pRenderData = NULL;
}

void COEModel_Impl::Destroy()
{
	// release data
	SAFE_RELEASE(m_pRenderData);
	m_vControls.clear();
	m_vRenders.clear();
}

void COEModel_Impl::Update(float fDetailTime)
{
	for (TV_CONTROL::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		IOEControl* pControl = (*it);
		pControl->Update(m_pRenderData, fDetailTime);
	}
}

void COEModel_Impl::Render()
{
	for (TV_RENDER::iterator it = m_vRenders.begin(); it != m_vRenders.end(); ++it)
	{
		IOERender* pRender = (*it);
		pRender->Render(m_pRenderData);
	}
}

bool COEModel_Impl::Create(const tstring& strFile)
{
	IOEXmlDocument* pXmlDocument = g_pOEXmlMgr->CreateDocument(strFile);
	if (!pXmlDocument) return false;

	// create mesh
	IOEXmlNode* pXmlRoot = pXmlDocument->GetRootNode();
	bool bOK = CreateRenderData(pXmlRoot);
	SAFE_RELEASE(pXmlDocument);
	if (!bOK) return false;

	// create control
	IOEControl* pControl = g_pOEControlMgr->GetControl(OECT_SKINMESH);
	if (pControl) m_vControls.push_back(pControl);

	// for draw skin
	IOERender* pRender = g_pOERenderMgr->GetRender(OERT_SKINMESH);
	if (pRender) m_vRenders.push_back(pRender);

	// for draw skelecton
	pRender = g_pOERenderMgr->GetRender(OERT_SKELETON);
	if (pRender) m_vRenders.push_back(pRender);

	return true;
}

bool COEModel_Impl::CreateRenderData(IOEXmlNode* pXmlRoot)
{
	if (!pXmlRoot) return false;

	// create render data
	m_pRenderData = new COESkinMeshRenderData_Impl();
	if (!m_pRenderData) return false;

	// create mesh
	IOEXmlNode* pXmlMesh = pXmlRoot->FirstChild(TS("Mesh"));
	if (!pXmlMesh) return false;

	tstring strMeshFile;
	if (!pXmlMesh->GetText(strMeshFile)) return false;
	if (!m_pRenderData->LoadMesh(strMeshFile)) return false;

	// create skeleton
	IOEXmlNode* pXmlSkeleton = pXmlRoot->FirstChild(TS("Skeleton"));
	if (!pXmlSkeleton) return false;

	tstring strSkeletonFile;
	if (!pXmlSkeleton->GetText(strSkeletonFile)) return false;
	if (!m_pRenderData->LoadSkeleton(strSkeletonFile)) return false;

	// create materials
	IOEXmlNode* pXmlMaterials = pXmlRoot->FirstChild(TS("Materials"));
	if (!m_pRenderData->LoadMaterials(pXmlMaterials)) return false;

	return true;
}
