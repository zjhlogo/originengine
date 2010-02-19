/*!
 * \file OESkinMeshRenderData_Impl.cpp
 * \date 13-2-2010 19:57:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkinMeshRenderData_Impl.h"
#include "OEBone_Impl.h"

#include <IOEFileMgr.h>
#include <IOETextureMgr.h>
#include <IOEShaderMgr.h>

#include <OEFmtMesh.h>
#include <OEFmtBone.h>

COESkinMeshRenderData_Impl::COESkinMeshRenderData_Impl()
:IOERenderData(OERDT_SKINMESH)
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
	m_fAnimLength = 0.0f;
	m_fTotalTime = 0.0f;
	return true;
}

void COESkinMeshRenderData_Impl::Destroy()
{
	DestroyMesh();
	DestroyBone();
}

bool COESkinMeshRenderData_Impl::LoadMesh(const tstring& strFile)
{
	DestroyMesh();
	return CreateMesh(strFile);
}

bool COESkinMeshRenderData_Impl::LoadBone(const tstring& strFile)
{
	DestroyBone();
	return CreateBone(strFile);
}

bool COESkinMeshRenderData_Impl::LoadMaterials(IOEXmlNode* pXmlMaterials)
{
	DestroyMaterials();
	return CreateMaterials(pXmlMaterials);
}

IOEMesh* COESkinMeshRenderData_Impl::GetMesh()
{
	return m_pMesh;
}

TV_BONE& COESkinMeshRenderData_Impl::GetBones()
{
	return m_vBones;
}

TV_MATRIX& COESkinMeshRenderData_Impl::GetSkinMatrix()
{
	return m_vmatSkin;
}

TV_MATERIAL& COESkinMeshRenderData_Impl::GetMaterials()
{
	return m_vMaterials;
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

bool COESkinMeshRenderData_Impl::CreateBone(const tstring& strFile)
{
	DestroyBone();

	bool bOK = g_pOEResMgr->CreateBones(m_vBones, strFile);
	if (!bOK) return false;

	for (TV_BONE::iterator it = m_vBones.begin(); it != m_vBones.end(); ++it)
	{
		IOEBone* pBone = (*it);
		if (m_fAnimLength < pBone->GetTimeLength()) m_fAnimLength = pBone->GetTimeLength();
	}

	m_vmatSkin.resize(m_vBones.size());

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
		MATERIAL Material;
		if (!g_pOEResMgr->CreateMaterial(Material, pXmlMaterial)) return false;
		m_vMaterials.push_back(Material);

		pXmlMaterial = pXmlMaterial->NextSibling(TS("Material"));
	}

	return true;
}

void COESkinMeshRenderData_Impl::DestroyMesh()
{
	SAFE_RELEASE(m_pMesh);
}

void COESkinMeshRenderData_Impl::DestroyBone()
{
	g_pOEResMgr->DestroyBones(m_vBones);
	m_fAnimLength = 0.0f;
}

void COESkinMeshRenderData_Impl::DestroyMaterials()
{
	for (TV_MATERIAL::iterator it = m_vMaterials.begin(); it != m_vMaterials.end(); ++it)
	{
		MATERIAL& Material = (*it);
		g_pOEResMgr->DestroyMaterial(Material);
	}
	m_vMaterials.clear();
}
