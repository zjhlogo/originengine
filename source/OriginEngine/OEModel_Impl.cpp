/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"
#include <IOEMeshMgr.h>
#include <IOEXmlMgr.h>
#include <IOEShaderMgr.h>
#include <IOETextureMgr.h>
#include <IOELogFileMgr.h>
#include <IOERenderer.h>

#include <OEFmtMesh.h>

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
	m_pMesh = NULL;
	m_fTotalTime = 0.0f;
}

void COEModel_Impl::Destroy()
{
	ClearMaterials();
}

void COEModel_Impl::ClearMaterials()
{
	for (TV_MATERIAL::iterator it = m_vMaterial.begin(); it != m_vMaterial.end(); ++it)
	{
		MATERIAL& Material = (*it);
		SAFE_RELEASE(Material.pShader);
		SAFE_RELEASE(Material.pTexture);
	}

	m_vMaterial.clear();
}

void COEModel_Impl::Update(float fDetailTime)
{
	m_fTotalTime += fDetailTime;

	CMatrix4x4 matAnim;
	int nNumBones = m_pMesh->GetNumBones();
	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pMeshBone = m_pMesh->GetBone(i);

		pMeshBone->SlerpMatrix(m_vmatSkin[i], m_fTotalTime);
		const CMatrix4x4& matLocal = pMeshBone->GetLocalMatrix();

		int nParentID = pMeshBone->GetParentID();
		if (nParentID != COEFmtMesh::INVALID_BONE_ID)
		{
			m_vmatSkin[i] *= m_vmatSkin[nParentID];
		}
	}

	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pMeshBone = m_pMesh->GetBone(i);
		m_vmatSkin[i] = pMeshBone->GetWorldMatrixInv() * m_vmatSkin[i];
	}
}

void COEModel_Impl::Render(float fDetailTime)
{
	CMatrix4x4 matWorldViewProj;
	g_pOERenderer->GetTransform(matWorldViewProj, IOERenderer::TT_WORLD_VIEW_PROJ);

	int nNumPiece = m_pMesh->GetNumPieces();
	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = m_pMesh->GetPiece(i);

		int nMaterialID = pPiece->GetMaterialID();
		if (nMaterialID >= (int)m_vMaterial.size()) continue;
		if (pPiece->GetVertDecl() != m_vMaterial[nMaterialID].nVertDecl) continue;

		m_vMaterial[nMaterialID].pShader->SetMatrix(t("g_matWorldViewProj"), matWorldViewProj);
		m_vMaterial[nMaterialID].pShader->SetTexture(t("g_texBase"), m_vMaterial[nMaterialID].pTexture);
		m_vMaterial[nMaterialID].pShader->SetMatrixArray(t("g_matBoneMatrix"), &m_vmatSkin[0], m_vmatSkin.size());
		g_pOERenderer->SetShader(m_vMaterial[nMaterialID].pShader);

		g_pOERenderer->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}
}

IOEMesh* COEModel_Impl::GetMesh()
{
	return m_pMesh;
}

int COEModel_Impl::GetNumMatrixPalette()
{
	return m_vmatSkin.size();
}

CMatrix4x4* COEModel_Impl::GetMatrixPalette()
{
	if (m_vmatSkin.empty()) return NULL;
	return &m_vmatSkin[0];
}

bool COEModel_Impl::Create(const tstring& strFile)
{
	IOEXmlDocument* pXmlDocument = g_pOEXmlMgr->CreateDocument(strFile);
	if (!pXmlDocument) return false;

	IOEXmlNode* pXmlRoot = pXmlDocument->GetRootNode();

	// create mesh
	IOEXmlNode* pXmlMesh = pXmlRoot->FirstChild(t("Mesh"));
	if (!CreateMesh(pXmlMesh))
	{
		SAFE_RELEASE(pXmlDocument);
		return false;
	}

	// create materials
	IOEXmlNode* pXmlMaterials = pXmlRoot->FirstChild(t("Materials"));
	if (!CreateMaterials(pXmlMaterials))
	{
		SAFE_RELEASE(pXmlDocument);
		LOGOUT(t("IOEModel::CreateMaterials failed"));
		return false;
	}

	// no used any more
	SAFE_RELEASE(pXmlDocument);

	int nNumBones = m_pMesh->GetNumBones();
	m_vmatSkin.resize(nNumBones);

	return true;
}

bool COEModel_Impl::CreateMesh(IOEXmlNode* pXmlMesh)
{
	if (!pXmlMesh) return false;

	// create mesh
	tstring strMeshFile;
	if (!pXmlMesh->GetText(strMeshFile)) return false;

	m_pMesh = g_pOEMeshMgr->CreateMeshFromFile(strMeshFile);
	if (!m_pMesh) return false;

	return true;
}

bool COEModel_Impl::CreateMaterials(IOEXmlNode* pXmlMaterials)
{
	ClearMaterials();
	if (!pXmlMaterials) return false;

	int nMaterialCount = 0;
	if (!pXmlMaterials->GetAttribute(nMaterialCount, t("count"))) return false;

	IOEXmlNode* pXmlMaterial = pXmlMaterials->FirstChild(t("Material"));
	while (nMaterialCount)
	{
		if (!pXmlMaterial) return false;

		MATERIAL Material;
		if (!pXmlMaterial->GetAttribute(Material.nID, t("id"))) return false;
		if (!pXmlMaterial->GetAttribute(Material.nVertDecl, t("vertdecl"))) return false;
		if (!pXmlMaterial->GetAttribute(Material.strShaderFile, t("shader"))) return false;
		if (!pXmlMaterial->GetAttribute(Material.strTextureFile, t("texture"))) return false;

		Material.pShader = CreateShader(Material.nVertDecl, Material.strShaderFile);
		if (!Material.pShader) return false;

		Material.pTexture = g_pOETextureMgr->CreateTextureFromFile(Material.strTextureFile);
		if (!Material.pTexture)
		{
			SAFE_RELEASE(Material.pShader);
			return false;
		}

		m_vMaterial.push_back(Material);

		pXmlMaterial = pXmlMaterial->NextSibling(t("Material"));
		--nMaterialCount;
	}

	return true;
}

IOEShader* COEModel_Impl::CreateShader(int nVertDecl, const tstring& strShaderFile)
{
	std::vector<IOEVertDecl::ELEMENT> vDecl;

	if (nVertDecl & COEFmtMesh::VDM_XYZ)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_FLOAT3;
		Element.eUsage = IOEVertDecl::U_POSITION;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	if (nVertDecl & COEFmtMesh::VDM_COLOR)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_COLOR;
		Element.eUsage = IOEVertDecl::U_COLOR;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	if (nVertDecl & COEFmtMesh::VDM_NXNYNZ)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_FLOAT3;
		Element.eUsage = IOEVertDecl::U_NORMAL;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	if (nVertDecl & COEFmtMesh::VDM_UV)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_FLOAT2;
		Element.eUsage = IOEVertDecl::U_TEXCOORD;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	if (nVertDecl & COEFmtMesh::VDM_BONE)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_UBYTE4;
		Element.eUsage = IOEVertDecl::U_BLENDINDICES;
		Element.nIndex = 0;
		vDecl.push_back(Element);

		Element.eType = IOEVertDecl::T_FLOAT4;
		Element.eUsage = IOEVertDecl::U_BLENDWEIGHT;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	// push back empty element to indicate the end
	IOEVertDecl::ELEMENT Element;
	Element.eType = IOEVertDecl::T_UNKNOWN;
	Element.eUsage = IOEVertDecl::U_UNKNOWN;
	Element.nIndex = 0;
	vDecl.push_back(Element);

	return g_pOEShaderMgr->CreateShader(&vDecl[0], strShaderFile);
}
