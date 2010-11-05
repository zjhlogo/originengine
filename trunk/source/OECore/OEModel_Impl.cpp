/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"
#include <OEBase/IOEXmlMgr.h>
#include <OECore/IOEControlMgr.h>
#include <OECore/IOERenderMgr.h>
#include <OECore/IOEResMgr.h>

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
	DestroyMesh();
	DestroyAnimData();
	DestroyMaterialsList();
	SAFE_RELEASE(m_pRenderData);
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
		if (pControl->GetRtti()->GetTypeName() == strControl)
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
		if (pRender->GetRtti()->GetTypeName() == strRender)
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

	IOEXmlNode* pXmlRoot = pXmlDocument->GetRootNode();

	// create render data
	IOEXmlNode* pXmlRenderData = pXmlRoot->FirstChild(TS("RenderData"));
	if (!CreateRenderData(pXmlRenderData)) return false;

	// create controls
	IOEXmlNode* pXmlControls = pXmlRoot->FirstChild(TS("Controls"));
	CreateControls(pXmlControls);

	// create renderer
	IOEXmlNode* pXmlRenderers = pXmlRoot->FirstChild(TS("Renderers"));
	CreateRenderers(pXmlRenderers);

	return true;
}

bool COEModel_Impl::CreateRenderData(IOEXmlNode* pXmlRenderData)
{
	if (!pXmlRenderData) return false;

	// create render data
	m_pRenderData = g_pOEResMgr->CreateRenderData();
	if (!m_pRenderData) return false;
	m_pRenderData->SetObject(TS("Holder"), this);

	// create mesh
	IOEXmlNode* pXmlMesh = pXmlRenderData->FirstChild(TS("Mesh"));
	if (pXmlMesh)
	{
		tstring strMeshFile;
		if (!pXmlMesh->GetText(strMeshFile)) return false;
		if (!LoadMesh(strMeshFile)) return false;
	}

	// create skeleton
	IOEXmlNode* pXmlSkeleton = pXmlRenderData->FirstChild(TS("Skeleton"));
	if (pXmlSkeleton)
	{
		tstring strSkeletonFile;
		if (!pXmlSkeleton->GetText(strSkeletonFile)) return false;
		if (!LoadAnimData(strSkeletonFile)) return false;
	}

	// create materials
	IOEXmlNode* pXmlMaterials = pXmlRenderData->FirstChild(TS("Materials"));
	if (pXmlMaterials)
	{
		if (!LoadMaterialsList(pXmlMaterials)) return false;
	}

	return true;
}

bool COEModel_Impl::CreateControls(IOEXmlNode* pXmlControls)
{
	if (!pXmlControls) return false;

	IOEXmlNode* pXmlControl = pXmlControls->FirstChild(TS("Control"));
	while (pXmlControl)
	{
		tstring strControlName;
		pXmlControl->GetText(strControlName);
		AddControl(strControlName);
		pXmlControl = pXmlControl->NextSibling(TS("Control"));
	}

	return true;
}

bool COEModel_Impl::CreateRenderers(IOEXmlNode* pXmlRenderers)
{
	if (!pXmlRenderers) return false;

	IOEXmlNode* pXmlRender = pXmlRenderers->FirstChild(TS("Renderer"));
	while (pXmlRender)
	{
		tstring strControlName;
		pXmlRender->GetText(strControlName);
		AddRender(strControlName);
		pXmlRender = pXmlRender->NextSibling(TS("Renderer"));
	}

	return true;
}

bool COEModel_Impl::LoadMesh(const tstring& strFile)
{
	DestroyMesh();
	return CreateMesh(strFile);
}

bool COEModel_Impl::LoadAnimData(const tstring& strFile)
{
	DestroyAnimData();
	return CreateAnimData(strFile);
}

bool COEModel_Impl::LoadMaterialsList(IOEXmlNode* pXmlMaterials)
{
	DestroyMaterialsList();
	return CreateMaterialsList(pXmlMaterials);
}

bool COEModel_Impl::CreateMesh(const tstring& strFile)
{
	DestroyMesh();

	IOEMesh* pMesh = g_pOEResMgr->CreateMesh(strFile);
	if (!pMesh) return false;

	m_pRenderData->SetMesh(TS("MainMesh"), pMesh);
	return true;
}

bool COEModel_Impl::CreateAnimData(const tstring& strFile)
{
	DestroyAnimData();

	IOEAnimData* pAnimData = g_pOEResMgr->CreateAnimData(strFile);
	if (!pAnimData) return false;

	m_pRenderData->SetAnimData(TS("MainAnimData"), pAnimData);
	return true;
}

bool COEModel_Impl::CreateMaterialsList(IOEXmlNode* pXmlMaterials)
{
	DestroyMaterialsList();

	IOEMaterialsList* pMaterialsList = g_pOEResMgr->CreateMaterialsList(pXmlMaterials);
	if (!pMaterialsList) return false;

	m_pRenderData->SetMaterialsList(TS("MainMaterialsList"), pMaterialsList);
	return true;
}

void COEModel_Impl::DestroyMesh()
{
	if (!m_pRenderData) return;

	IOEMesh* pMesh = m_pRenderData->GetMesh(TS("MainMesh"));
	SAFE_RELEASE(pMesh);
	m_pRenderData->RemoveMesh(TS("MainMesh"));
}

void COEModel_Impl::DestroyAnimData()
{
	if (!m_pRenderData) return;

	IOEAnimData* pAnimData = m_pRenderData->GetAnimData(TS("MainAnimData"));
	SAFE_RELEASE(pAnimData);

	m_pRenderData->RemoveAnimData(TS("MainAnimData"));
}

void COEModel_Impl::DestroyMaterialsList()
{
	if (!m_pRenderData) return;

	IOEMaterialsList* pMaterialsList = m_pRenderData->GetMaterialsList(TS("MainMaterialsList"));
	SAFE_RELEASE(pMaterialsList);

	m_pRenderData->RemoveMaterialsList(TS("MainMaterialsList"));
}
