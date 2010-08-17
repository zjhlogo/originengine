/*!
 * \file OEMaterial_Impl.cpp
 * \date 1-3-2010 19:34:12
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMaterial_Impl.h"
#include <OECore/IOETextureMgr.h>
#include <OECore/IOEShaderMgr.h>
#include <OECore/OEFmtMesh.h>
#include <assert.h>

COEMaterial_Impl::COEMaterial_Impl(IOEXmlNode* pXmlMaterial)
{
	Init();
	m_bOK = Create(pXmlMaterial);
}

COEMaterial_Impl::~COEMaterial_Impl()
{
	Destroy();
}

void COEMaterial_Impl::Init()
{
	m_nID = 0;
	m_nVertDeclMask = 0;
	m_pShader = NULL;
	memset(m_pTexture, 0, sizeof(m_pTexture));
}

void COEMaterial_Impl::Destroy()
{
	SAFE_RELEASE(m_pShader);
	for (int i = 0; i < MTT_NUM; ++i)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
}

int COEMaterial_Impl::GetID()
{
	return m_nID;
}

int COEMaterial_Impl::GetVertDeclMask()
{
	return m_nVertDeclMask;
}

IOEShader* COEMaterial_Impl::GetShader()
{
	return m_pShader;
}

IOETexture* COEMaterial_Impl::GetTexture(MATERIAL_TEXTURE_TYPE eType)
{
	if (eType < 0 || eType >= MTT_NUM) return NULL;
	return m_pTexture[eType];
}

bool COEMaterial_Impl::Create(IOEXmlNode* pXmlMaterial)
{
	if (!pXmlMaterial) return false;

	if (!pXmlMaterial->GetAttribute(m_nID, TS("id"))) return false;
	if (!pXmlMaterial->GetAttribute(m_nVertDeclMask, TS("vertdecl"))) return false;
	if (!pXmlMaterial->GetAttribute(m_strShader, TS("shader"))) return false;

	m_pShader = CreateShaderFromVertDecl(m_nVertDeclMask, m_strShader);
	if (!m_pShader) return false;

	for (int i = 0; i < MTT_NUM; ++i)
	{
		pXmlMaterial->GetAttribute(m_strTexture[i], GetTextureKey((MATERIAL_TEXTURE_TYPE)i));
		if (m_strTexture[i].empty()) continue;

		m_pTexture[i] = g_pOETextureMgr->CreateTextureFromFile(m_strTexture[i]);
	}

	return true;
}

const tstring& COEMaterial_Impl::GetTextureKey(MATERIAL_TEXTURE_TYPE eType)
{
	static const tstring s_EmptyTexture;
	static const tstring s_TextureKey[MTT_NUM] =
	{
		TS("texture0"),
		TS("texture1"),
		TS("texture2"),
		TS("texture3"),
		TS("texture4"),
		TS("texture5"),
		TS("texture6"),
		TS("texture7"),
	};
	assert(MTT_NUM == 8);
	if (eType < 0 || eType >= MTT_NUM) return s_EmptyTexture;
	return s_TextureKey[eType];
}

IOEShader* COEMaterial_Impl::CreateShaderFromVertDecl(int nVertDeclMask, const tstring& strFile)
{
	std::vector<VERT_DECL_ELEMENT> vDecl;
	int nTexCoordIndex = 0;

	// position
	if (nVertDeclMask & COEFmtMesh::VDM_XYZ)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_FLOAT3;
		Element.eUsage = VDU_POSITION;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	// color
	if (nVertDeclMask & COEFmtMesh::VDM_COLOR)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_COLOR;
		Element.eUsage = VDU_COLOR;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	// texcoord
	if (nVertDeclMask & COEFmtMesh::VDM_UV)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_FLOAT2;
		Element.eUsage = VDU_TEXCOORD;
		Element.nIndex = nTexCoordIndex++;
		vDecl.push_back(Element);
	}

	// normal
	if (nVertDeclMask & COEFmtMesh::VDM_NXNYNZ)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_FLOAT3;
		Element.eUsage = VDU_TEXCOORD;
		Element.nIndex = nTexCoordIndex++;
		vDecl.push_back(Element);
	}

	// tangent
	if (nVertDeclMask & COEFmtMesh::VDM_TXTYTZ)
	{
		VERT_DECL_ELEMENT Element;
		Element.eType = VDT_FLOAT3;
		Element.eUsage = VDU_TEXCOORD;
		Element.nIndex = nTexCoordIndex++;
		vDecl.push_back(Element);
	}

	// blendindex, blendweight
	if (nVertDeclMask & COEFmtMesh::VDM_BONE)
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
