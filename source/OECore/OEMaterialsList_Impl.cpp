/*!
 * \file OEMaterialsList_Impl.cpp
 * \date 10-29-2010 19:17:51
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEMaterialsList_Impl.h"
#include <OECore/IOEResMgr.h>
#include <OEBase/IOELogFileMgr.h>

COEMaterialsList_Impl::COEMaterialsList_Impl(IOEXmlNode* pXmlMaterials)
{
	Init();
	m_bOK = Create(pXmlMaterials);
}

COEMaterialsList_Impl::~COEMaterialsList_Impl()
{
	Destroy();
}

void COEMaterialsList_Impl::Init()
{
	// TODO: 
}

void COEMaterialsList_Impl::Destroy()
{
	for (TM_MATERIAL::iterator it = m_mapMaterials.begin(); it != m_mapMaterials.end(); ++it)
	{
		IOEMaterial* pMaterial = it->second;
		SAFE_RELEASE(pMaterial);
	}
	m_mapMaterials.clear();
}

IOEMaterial* COEMaterialsList_Impl::GetMaterial(int nID)
{
	TM_MATERIAL::iterator itfound = m_mapMaterials.find(nID);
	if (itfound == m_mapMaterials.end()) return NULL;

	return itfound->second;
}

int COEMaterialsList_Impl::GetNumMaterial()
{
	return (int)m_mapMaterials.size();
}

bool COEMaterialsList_Impl::Create(IOEXmlNode* pXmlMaterials)
{
	if (!pXmlMaterials) return false;

	int nMaterialCount = 0;
	if (!pXmlMaterials->GetAttribute(nMaterialCount, TS("count"))) return false;

	IOEXmlNode* pXmlMaterial = pXmlMaterials->FirstChild(TS("Material"));
	for (int i = 0; i < nMaterialCount; ++i)
	{
		IOEMaterial* pMaterial = g_pOEResMgr->CreateMaterial(pXmlMaterial);
		if (!pMaterial) return false;

		if (GetMaterial(pMaterial->GetID()))
		{
			LOGOUT(TS("Duplicate Material ID %d"), pMaterial->GetID());
			SAFE_RELEASE(pMaterial);
			return false;
		}

		m_mapMaterials.insert(std::make_pair(pMaterial->GetID(), pMaterial));

		pXmlMaterial = pXmlMaterial->NextSibling(TS("Material"));
	}

	return true;
}
