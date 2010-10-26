/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"
#include "OERenderData_Impl.h"
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
	CreateRenderData(pXmlRenderData);

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
	// create render data
	m_pRenderData = new COERenderData_Impl(this, pXmlRenderData);
	if (!m_pRenderData || !m_pRenderData->IsOK()) return false;

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
