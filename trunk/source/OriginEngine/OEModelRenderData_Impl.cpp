/*!
 * \file OEModelRenderData_Impl.cpp
 * \date 12-2-2010 20:06:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModelRenderData_Impl.h"
#include "OEBone_Impl.h"

#include <IOEFileMgr.h>
#include <IOETextureMgr.h>
#include <IOEShaderMgr.h>

#include <OEFmtMesh.h>
#include <OEFmtBone.h>

COEModelRenderData_Impl::COEModelRenderData_Impl()
{
	Init();
}

COEModelRenderData_Impl::~COEModelRenderData_Impl()
{
	Destroy();
}

void COEModelRenderData_Impl::Init()
{
	m_pMesh = NULL;
	m_fAnimLength = 0.0f;
	m_fTotalTime = 0.0f;
	m_bOK = true;
}

void COEModelRenderData_Impl::Destroy()
{
	DestroyMesh();
	DestroyBone();
}

bool COEModelRenderData_Impl::LoadMesh(const tstring& strFile)
{
	DestroyMesh();
	return CreateMesh(strFile);
}

bool COEModelRenderData_Impl::LoadBone(const tstring& strFile)
{
	DestroyBone();
	return CreateBone(strFile);
}

bool COEModelRenderData_Impl::LoadMaterials(IOEXmlNode* pXmlMaterials)
{
	DestroyMaterials();
	return CreateMaterials(pXmlMaterials);
}

IOEMesh* COEModelRenderData_Impl::GetMesh()
{
	return m_pMesh;
}

TV_BONE& COEModelRenderData_Impl::GetBones()
{
	return m_vBones;
}

TV_MATRIX& COEModelRenderData_Impl::GetSkinMatrix()
{
	return m_vmatSkin;
}

TV_MATERIAL& COEModelRenderData_Impl::GetMaterials()
{
	return m_vMaterials;
}

void COEModelRenderData_Impl::SetAnimLength(float fAnimLength)
{
	m_fAnimLength = fAnimLength;
}

float COEModelRenderData_Impl::GetAnimLength() const
{
	return m_fAnimLength;
}

void COEModelRenderData_Impl::SetTotalTime(float fTotalTime)
{
	m_fTotalTime = fTotalTime;
}

float COEModelRenderData_Impl::GetTotalTime() const
{
	return m_fTotalTime;
}

bool COEModelRenderData_Impl::CreateMesh(const tstring& strFile)
{
	DestroyMesh();

	m_pMesh = g_pOEResMgr->CreateMesh(strFile);
	if (!m_pMesh) return false;

	return true;
}

bool COEModelRenderData_Impl::CreateBone(const tstring& strFile)
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

bool COEModelRenderData_Impl::CreateMaterials(IOEXmlNode* pXmlMaterials)
{
	if (!pXmlMaterials) return false;

	int nMaterialCount = 0;
	if (!pXmlMaterials->GetAttribute(nMaterialCount, TS("count"))) return false;

	IOEXmlNode* pXmlMaterial = pXmlMaterials->FirstChild(TS("Material"));
	for (int i = 0; i < nMaterialCount; ++i)
	{
		if (!pXmlMaterial) break;

		MATERIAL Material;
		if (!pXmlMaterial->GetAttribute(Material.nID, TS("id"))) break;
		if (!pXmlMaterial->GetAttribute(Material.nVertDecl, TS("vertdecl"))) break;
		if (!pXmlMaterial->GetAttribute(Material.strShaderFile, TS("shader"))) break;
		if (!pXmlMaterial->GetAttribute(Material.strTextureFile, TS("texture"))) break;
		if (!pXmlMaterial->GetAttribute(Material.strTexNormalFile, TS("texnormal"))) break;

		Material.pShader = CreateShader(Material.nVertDecl, Material.strShaderFile);
		if (!Material.pShader) return false;

		Material.pTexture = g_pOETextureMgr->CreateTextureFromFile(Material.strTextureFile);
		if (!Material.pTexture)
		{
			SAFE_RELEASE(Material.pShader);
			break;
		}

		Material.pTexNormal = g_pOETextureMgr->CreateTextureFromFile(Material.strTexNormalFile);
		if (!Material.pTexture)
		{
			SAFE_RELEASE(Material.pTexture);
			SAFE_RELEASE(Material.pShader);
			break;
		}

		m_vMaterials.push_back(Material);

		pXmlMaterial = pXmlMaterial->NextSibling(TS("Material"));
	}

	if (m_vMaterials.size() != nMaterialCount)
	{
		DestroyMaterials();
		return false;
	}

	return true;
}

IOEShader* COEModelRenderData_Impl::CreateShader(int nVertDecl, const tstring& strFile)
{
	std::vector<VERT_DECL_ELEMENT> vDecl;
	int nTexCoordIndex = 0;

	// position
	if (nVertDecl & COEFmtMesh::VDM_XYZ)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_FLOAT3;
		Element.eUsage = VDU_POSITION;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	// color
	if (nVertDecl & COEFmtMesh::VDM_COLOR)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_COLOR;
		Element.eUsage = VDU_COLOR;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	// texcoord
	if (nVertDecl & COEFmtMesh::VDM_UV)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_FLOAT2;
		Element.eUsage = VDU_TEXCOORD;
		Element.nIndex = nTexCoordIndex++;
		vDecl.push_back(Element);
	}

	// normal
	if (nVertDecl & COEFmtMesh::VDM_NXNYNZ)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_FLOAT3;
		Element.eUsage = VDU_TEXCOORD;
		Element.nIndex = nTexCoordIndex++;
		vDecl.push_back(Element);
	}

	// tangent
	if (nVertDecl & COEFmtMesh::VDM_TXTYTZ)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_FLOAT3;
		Element.eUsage = VDU_TEXCOORD;
		Element.nIndex = nTexCoordIndex++;
		vDecl.push_back(Element);
	}

	// blendindex, blendweight
	if (nVertDecl & COEFmtMesh::VDM_BONE)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_UBYTE4;
		Element.eUsage = VDU_BLENDINDICES;
		Element.nIndex = 0;
		vDecl.push_back(Element);

		Element.eType = VDT_FLOAT4;
		Element.eUsage = VDU_BLENDWEIGHT;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	// push back empty element to indicate the end
	VERT_DECL_ELEMENT Element;
	Element.eType = VDT_UNKNOWN;
	Element.eUsage = VDU_UNKNOWN;
	Element.nIndex = 0;
	vDecl.push_back(Element);

	return g_pOEShaderMgr->CreateShader(&vDecl[0], strFile);
}

void COEModelRenderData_Impl::DestroyMesh()
{
	SAFE_RELEASE(m_pMesh);
}

void COEModelRenderData_Impl::DestroyBone()
{
	g_pOEResMgr->DestroyBones(m_vBones);
	m_fAnimLength = 0.0f;
}

void COEModelRenderData_Impl::DestroyMaterials()
{
	for (TV_MATERIAL::iterator it = m_vMaterials.begin(); it != m_vMaterials.end(); ++it)
	{
		MATERIAL& Material = (*it);
		SAFE_RELEASE(Material.pShader);
		SAFE_RELEASE(Material.pTexture);
		SAFE_RELEASE(Material.pTexNormal);
	}
	m_vMaterials.clear();
}
