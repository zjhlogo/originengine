/*!
 * \file OEResMgr_Impl.cpp
 * \date 13-2-2010 9:42:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEResMgr_Impl.h"
#include "OEModel_Impl.h"
#include "OEMesh_Impl.h"
#include "OEBones_Impl.h"

#include <IOELogFileMgr.h>
#include <IOEFileMgr.h>
#include <IOETextureMgr.h>
#include <IOEShaderMgr.h>
#include <OEFmtMesh.h>
#include <assert.h>

COEResMgr_Impl::COEResMgr_Impl()
{
	assert(!g_pOEResMgr);
	g_pOEResMgr = this;
	m_bOK = Init();
}

COEResMgr_Impl::~COEResMgr_Impl()
{
	Destroy();
	g_pOEResMgr = NULL;
}

bool COEResMgr_Impl::Init()
{
	// TODO: 
	return true;
}

void COEResMgr_Impl::Destroy()
{
	// TODO: check m_MeshMap/m_BonesMap whether is empty, and logout
}

IOEModel* COEResMgr_Impl::CreateModel(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!GetMediaFilePath(strFilePath, strFile)) return NULL;

	COEModel_Impl* pModel = new COEModel_Impl(strFilePath);
	if (!pModel || !pModel->IsOK())
	{
		LOGOUT(TS("IOEModelMgr::CreateModel Failed %s"), strFilePath.c_str());
		SAFE_DELETE(pModel);
		return NULL;
	}

	return pModel;
}

IOEMesh* COEResMgr_Impl::CreateMesh(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!GetMediaFilePath(strFilePath, strFile)) return NULL;

	// check whether the mesh created
	TM_MESH::iterator itfound = m_MeshMap.find(strFilePath);
	if (itfound != m_MeshMap.end())
	{
		IOEMesh* pMesh = itfound->second;
		pMesh->IncRef();
		return pMesh;
	}

	// create new
	COEMesh_Impl* pMesh = new COEMesh_Impl(strFilePath);
	if (!pMesh || !pMesh->IsOK())
	{
		LOGOUT(TS("IOEMeshMgr::CreateMesh Failed %s"), strFilePath.c_str());
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	m_MeshMap.insert(std::make_pair(strFilePath, pMesh));
	return pMesh;
}

IOEBones* COEResMgr_Impl::CreateBones(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!GetMediaFilePath(strFilePath, strFile)) return NULL;

	// check whether the bones created
	TM_BONES::iterator itfound = m_BonesMap.find(strFilePath);
	if (itfound != m_BonesMap.end())
	{
		IOEBones* pBones = itfound->second;
		pBones->IncRef();
		return pBones;
	}

	// create new
	COEBones_Impl* pBones = new COEBones_Impl(strFilePath);
	if (!pBones || !pBones->IsOK())
	{
		LOGOUT(TS("IOEMeshMgr::CreateMesh Failed %s"), strFilePath.c_str());
		SAFE_RELEASE(pBones);
		return NULL;
	}

	m_BonesMap.insert(std::make_pair(strFilePath, pBones));
	return pBones;
}

bool COEResMgr_Impl::CreateMaterial(MATERIAL& MaterialOut, IOEXmlNode* pXmlMaterial)
{
	if (!pXmlMaterial) return false;

	EmptyMaterial(MaterialOut);

	if (!pXmlMaterial->GetAttribute(MaterialOut.nID, TS("id"))) return false;

	if (!pXmlMaterial->GetAttribute(MaterialOut.nVertDecl, TS("vertdecl"))) return false;

	if (!pXmlMaterial->GetAttribute(MaterialOut.strShader, TS("shader"))) return false;

	if (!pXmlMaterial->GetAttribute(MaterialOut.strTexDiffuse, TS("texdiffuse"))) return false;

	if (!pXmlMaterial->GetAttribute(MaterialOut.strTexNormal, TS("texnormal"))) return false;

	MaterialOut.pShader = CreateShaderFromVertDecl(MaterialOut.nVertDecl, MaterialOut.strShader);
	if (!MaterialOut.pShader) return false;

	MaterialOut.pTexDiffuse = g_pOETextureMgr->CreateTextureFromFile(MaterialOut.strTexDiffuse);
	if (!MaterialOut.pTexDiffuse)
	{
		DestroyMaterial(MaterialOut);
		return false;
	}

	MaterialOut.pTexNormal = g_pOETextureMgr->CreateTextureFromFile(MaterialOut.strTexNormal);
	if (!MaterialOut.pTexDiffuse)
	{
		DestroyMaterial(MaterialOut);
		return false;
	}

	return true;
}

void COEResMgr_Impl::DestroyMaterial(MATERIAL& Material)
{
	SAFE_RELEASE(Material.pShader);
	SAFE_RELEASE(Material.pTexDiffuse);
	SAFE_RELEASE(Material.pTexNormal);
	EmptyMaterial(Material);
}

void COEResMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& COEResMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

void COEResMgr_Impl::EmptyMaterial(MATERIAL& MaterialOut)
{
	MaterialOut.nID = 0;
	MaterialOut.nVertDecl = 0;
	MaterialOut.strShader.clear();
	MaterialOut.strTexDiffuse.clear();
	MaterialOut.strTexNormal.clear();
	MaterialOut.pShader = NULL;
	MaterialOut.pTexDiffuse = NULL;
	MaterialOut.pTexNormal = NULL;
}

IOEShader* COEResMgr_Impl::CreateShaderFromVertDecl(int nVertDecl, const tstring& strFile)
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

bool COEResMgr_Impl::GetMediaFilePath(tstring& strFilePathOut, const tstring& strFile)
{
	strFilePathOut = m_strDefaultDir + TS("\\") + strFile;
	COEOS::tolower(strFilePathOut, strFilePathOut);
	return true;
}

