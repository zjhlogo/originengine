/*!
 * \file OESkinMeshRenderData_Impl.cpp
 * \date 13-2-2010 19:57:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkinMeshRenderData_Impl.h"
#include "../OEBone_Impl.h"

COESkinMeshRenderData_Impl::COESkinMeshRenderData_Impl()
{
	m_bOK = Init();
}

COESkinMeshRenderData_Impl::~COESkinMeshRenderData_Impl()
{
	Destroy();
}

bool COESkinMeshRenderData_Impl::Init()
{
	m_pMesh = NULL;
	m_pSkeleton = NULL;
	m_fAnimLength = 0.0f;
	m_fTotalTime = 0.0f;
	return true;
}

void COESkinMeshRenderData_Impl::Destroy()
{
	DestroyMesh();
	DestroySkeleton();
	DestroyMaterials();
}

IOEMesh* COESkinMeshRenderData_Impl::GetMesh()
{
	return m_pMesh;
}

bool COESkinMeshRenderData_Impl::LoadMesh(const tstring& strFile)
{
	DestroyMesh();
	return CreateMesh(strFile);
}

bool COESkinMeshRenderData_Impl::LoadSkeleton(const tstring& strFile)
{
	DestroySkeleton();
	return CreateSkeleton(strFile);
}

bool COESkinMeshRenderData_Impl::LoadMaterials(IOEXmlNode* pXmlMaterials)
{
	DestroyMaterials();
	return CreateMaterials(pXmlMaterials);
}

IOESkeleton* COESkinMeshRenderData_Impl::GetSkeleton()
{
	return m_pSkeleton;
}

TV_MATRIX4X4& COESkinMeshRenderData_Impl::GetSkinMatrix()
{
	return m_vmatSkin;
}

IOEMaterial* COESkinMeshRenderData_Impl::GetMaterial(int nIndex)
{
	TM_MATERIAL::iterator itfound = m_MaterialMap.find(nIndex);
	if (itfound == m_MaterialMap.end()) return NULL;
	return itfound->second;
}

void COESkinMeshRenderData_Impl::SetAnimLength(float fAnimLength)
{
	m_fAnimLength = fAnimLength;
}

float COESkinMeshRenderData_Impl::GetAnimLength() const
{
	return m_fAnimLength;
}

void COESkinMeshRenderData_Impl::SetTotalTime(float fTotalTime)
{
	m_fTotalTime = fTotalTime;
}

float COESkinMeshRenderData_Impl::GetTotalTime() const
{
	return m_fTotalTime;
}

bool COESkinMeshRenderData_Impl::CreateMesh(const tstring& strFile)
{
	DestroyMesh();

	m_pMesh = g_pOEResMgr->CreateMesh(strFile);
	if (!m_pMesh) return false;

	return true;
}

bool COESkinMeshRenderData_Impl::CreateSkeleton(const tstring& strFile)
{
	DestroySkeleton();

	m_pSkeleton = g_pOEResMgr->CreateSkeleton(strFile);
	if (!m_pSkeleton) return false;

	int nBonesCount = m_pSkeleton->GetBonesCount();
	for (int i = 0; i < nBonesCount; ++i)
	{
		IOEBone* pBone = m_pSkeleton->GetBone(i);
		if (m_fAnimLength < pBone->GetTimeLength()) m_fAnimLength = pBone->GetTimeLength();
	}

	m_vmatSkin.resize(nBonesCount);
	return true;
}

bool COESkinMeshRenderData_Impl::CreateMaterials(IOEXmlNode* pXmlMaterials)
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
			SAFE_RELEASE(pMaterial);
			return false;
		}

		m_MaterialMap.insert(std::make_pair(pMaterial->GetID(), pMaterial));

		pXmlMaterial = pXmlMaterial->NextSibling(TS("Material"));
	}

	return true;
}

void COESkinMeshRenderData_Impl::DestroyMesh()
{
	SAFE_RELEASE(m_pMesh);
}

void COESkinMeshRenderData_Impl::DestroySkeleton()
{
	SAFE_RELEASE(m_pSkeleton);
	m_fAnimLength = 0.0f;
}

void COESkinMeshRenderData_Impl::DestroyMaterials()
{
	for (TM_MATERIAL::iterator it = m_MaterialMap.begin(); it != m_MaterialMap.end(); ++it)
	{
		IOEMaterial* pMaterial = it->second;
		SAFE_RELEASE(pMaterial);
	}
	m_MaterialMap.clear();
}
