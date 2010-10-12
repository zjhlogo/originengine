/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"
#include "OERenderData/OESkinMeshRenderData_Impl.h"
#include <OEBase/IOEXmlMgr.h>
#include <OECore/IOEControlMgr.h>
#include <OECore/IOERenderMgr.h>

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

void COEModel_Impl::Render(float fDetailTime)
{
	for (TV_RENDER::iterator it = m_vRenders.begin(); it != m_vRenders.end(); ++it)
	{
		IOERender* pRender = (*it);
		pRender->Render(m_pRenderData);
	}
}

IOERenderData* COEModel_Impl::GetRenderData()
{
	return m_pRenderData;
}

bool COEModel_Impl::AddControl(const tstring& strControl)
{
	IOEControl* pControl = g_pOEControlMgr->GetControl(strControl);
	if (!pControl) return false;

	m_vControls.push_back(pControl);
	return true;
}

void COEModel_Impl::RemoveControl(const tstring& strControl)
{
	for (TV_CONTROL::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		IOEControl* pControl = (*it);
		if (pControl->GetRtti()->GetClassName() == strControl)
		{
			m_vControls.erase(it);
			break;
		}
	}
}

bool COEModel_Impl::AddRender(const tstring& strRender)
{
	IOERender* pRender = g_pOERenderMgr->GetRender(strRender);
	if (!pRender) return false;

	m_vRenders.push_back(pRender);
	return true;
}

void COEModel_Impl::RemoveRender(const tstring& strRender)
{
	for (TV_RENDER::iterator it = m_vRenders.begin(); it != m_vRenders.end(); ++it)
	{
		IOERender* pRender = (*it);
		if (pRender->GetRtti()->GetClassName() == strRender)
		{
			m_vRenders.erase(it);
			break;
		}
	}
}

bool COEModel_Impl::Create(const tstring& strFile)
{
	IOEXmlDocument* pXmlDocument = g_pOEXmlMgr->CreateDocument(strFile);
	if (!pXmlDocument) return false;

	// create render data
	IOEXmlNode* pXmlRoot = pXmlDocument->GetRootNode();
	bool bOK = CreateRenderData(pXmlRoot);
	SAFE_RELEASE(pXmlDocument);
	if (!bOK) return false;

	// create control
	AddControl(TS("COESkinMeshControl_Impl"));

	// for draw skin
	AddRender(TS("COESkinMeshRender_Impl"));
	// for draw skelecton
	AddRender(TS("COESkeletonRender_Impl"));
	// for draw bounding box
	AddRender(TS("COEBoundingBoxRender_Impl"));

	return true;
}

bool COEModel_Impl::CreateRenderData(IOEXmlNode* pXmlRoot)
{
	if (!pXmlRoot) return false;

	// create render data
	m_pRenderData = new COESkinMeshRenderData_Impl(this);
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
