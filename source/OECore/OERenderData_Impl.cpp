/*!
 * \file OERenderData_Impl.cpp
 * \date 10-12-2010 17:14:57
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OERenderData_Impl.h"
#include "OEBone_Impl.h"

COERenderData_Impl::COERenderData_Impl(IOEObject* pHolder, IOEXmlNode* pXmlRenderData)
:IOERenderData(pHolder)
{
	Init();
	m_bOK = CreateRenderData(pXmlRenderData);
}

COERenderData_Impl::~COERenderData_Impl()
{
	Destroy();
}

void COERenderData_Impl::Init()
{
	m_pMesh = NULL;
	m_pSkeleton = NULL;
	m_fAnimLength = 0.0f;
	m_fTotalTime = 0.0f;
}

void COERenderData_Impl::Destroy()
{
	DestroyMesh();
	DestroySkeleton();
	DestroyMaterials();
}

IOEMesh* COERenderData_Impl::GetMesh()
{
	return m_pMesh;
}

IOESkeleton* COERenderData_Impl::GetSkeleton()
{
	return m_pSkeleton;
}

TV_MATRIX4X4& COERenderData_Impl::GetSkinMatrix()
{
	return m_vmatSkin;
}

IOEMaterial* COERenderData_Impl::GetMaterial(int nIndex)
{
	TM_MATERIAL::iterator itfound = m_MaterialMap.find(nIndex);
	if (itfound == m_MaterialMap.end()) return NULL;
	return itfound->second;
}

void COERenderData_Impl::SetAnimLength(float fAnimLength)
{
	m_fAnimLength = fAnimLength;
}

float COERenderData_Impl::GetAnimLength() const
{
	return m_fAnimLength;
}

void COERenderData_Impl::SetTotalTime(float fTotalTime)
{
	m_fTotalTime = fTotalTime;
}

float COERenderData_Impl::GetTotalTime() const
{
	return m_fTotalTime;
}

bool COERenderData_Impl::CreateRenderData(IOEXmlNode* pXmlRenderData)
{
	if (!pXmlRenderData) return false;

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
		if (!LoadSkeleton(strSkeletonFile)) return false;
	}

	// create materials
	IOEXmlNode* pXmlMaterials = pXmlRenderData->FirstChild(TS("Materials"));
	if (pXmlMaterials)
	{
		if (!LoadMaterials(pXmlMaterials)) return false;
	}

	return true;
}

bool COERenderData_Impl::LoadMesh(const tstring& strFile)
{
	DestroyMesh();
	return CreateMesh(strFile);
}

bool COERenderData_Impl::LoadSkeleton(const tstring& strFile)
{
	DestroySkeleton();
	return CreateSkeleton(strFile);
}

bool COERenderData_Impl::LoadMaterials(IOEXmlNode* pXmlMaterials)
{
	DestroyMaterials();
	return CreateMaterials(pXmlMaterials);
}

bool COERenderData_Impl::CreateMesh(const tstring& strFile)
{
	DestroyMesh();

	m_pMesh = g_pOEResMgr->CreateMesh(strFile);
	if (!m_pMesh) return false;

	return true;
}

bool COERenderData_Impl::CreateSkeleton(const tstring& strFile)
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

bool COERenderData_Impl::CreateMaterials(IOEXmlNode* pXmlMaterials)
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

void COERenderData_Impl::DestroyMesh()
{
	SAFE_RELEASE(m_pMesh);
}

void COERenderData_Impl::DestroySkeleton()
{
	SAFE_RELEASE(m_pSkeleton);
	m_fAnimLength = 0.0f;
}

void COERenderData_Impl::DestroyMaterials()
{
	for (TM_MATERIAL::iterator it = m_MaterialMap.begin(); it != m_MaterialMap.end(); ++it)
	{
		IOEMaterial* pMaterial = it->second;
		SAFE_RELEASE(pMaterial);
	}
	m_MaterialMap.clear();
}
