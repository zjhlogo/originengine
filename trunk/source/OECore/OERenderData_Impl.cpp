/*!
 * \file OERenderData_Impl.cpp
 * \date 10-12-2010 17:14:57
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OERenderData_Impl.h"
#include "OEBone_Impl.h"

COERenderData_Impl::COERenderData_Impl()
{
	m_bOK = Init();
}

COERenderData_Impl::~COERenderData_Impl()
{
	Destroy();
}

bool COERenderData_Impl::Init()
{
	// TODO: 
	return true;
}

void COERenderData_Impl::Destroy()
{
	// TODO: 
}

bool COERenderData_Impl::SetInt(const tstring& strKey, int nValue)
{
	TM_INT::iterator itfound = m_mapInts.find(strKey);
	if (itfound == m_mapInts.end())
	{
		m_mapInts.insert(std::make_pair(strKey, nValue));
	}
	else
	{
		itfound->second = nValue;
	}

	return true;
}

bool COERenderData_Impl::RemoveInt(const tstring& strKey)
{
	TM_INT::iterator itfound = m_mapInts.find(strKey);
	if (itfound == m_mapInts.end()) return false;

	m_mapInts.erase(itfound);
	return true;
}

bool COERenderData_Impl::GetInt(int& nValue, const tstring& strKey)
{
	TM_INT::iterator itfound = m_mapInts.find(strKey);
	if (itfound == m_mapInts.end()) return false;

	nValue = itfound->second;
	return true;
}

bool COERenderData_Impl::SetFloat(const tstring& strKey, float fValue)
{
	TM_FLOAT::iterator itfound = m_mapFloats.find(strKey);
	if (itfound == m_mapFloats.end())
	{
		m_mapFloats.insert(std::make_pair(strKey, fValue));
	}
	else
	{
		itfound->second = fValue;
	}
	return true;
}

bool COERenderData_Impl::RemoveFloat(const tstring& strKey)
{
	TM_FLOAT::iterator itfound = m_mapFloats.find(strKey);
	if (itfound == m_mapFloats.end()) return false;

	m_mapFloats.erase(itfound);
	return true;
}

bool COERenderData_Impl::GetFloat(float& fValue, const tstring& strKey)
{
	TM_FLOAT::iterator itfound = m_mapFloats.find(strKey);
	if (itfound == m_mapFloats.end()) return false;

	fValue = itfound->second;
	return true;
}

bool COERenderData_Impl::SetVector(const tstring& strKey, const CVector3& vValue)
{
	TM_VECTOR::iterator itfound = m_mapVectors.find(strKey);
	if (itfound == m_mapVectors.end())
	{
		m_mapVectors.insert(std::make_pair(strKey, vValue));
	}
	else
	{
		itfound->second = vValue;
	}

	return true;
}

bool COERenderData_Impl::RemoveVector(const tstring& strKey)
{
	TM_VECTOR::iterator itfound = m_mapVectors.find(strKey);
	if (itfound == m_mapVectors.end()) return false;

	m_mapVectors.erase(strKey);
	return true;
}

bool COERenderData_Impl::GetVector(CVector3& vValue, const tstring& strKey)
{
	TM_VECTOR::iterator itfound = m_mapVectors.find(strKey);
	if (itfound == m_mapVectors.end()) return false;

	vValue = itfound->second;
	return true;
}

bool COERenderData_Impl::SetMatrix(const tstring& strKey, const CMatrix4x4& matValue)
{
	TM_MATRIX::iterator itfound = m_mapMatrixs.find(strKey);
	if (itfound == m_mapMatrixs.end())
	{
		m_mapMatrixs.insert(std::make_pair(strKey, matValue));
	}
	else
	{
		itfound->second = matValue;
	}

	return true;
}

bool COERenderData_Impl::RemoveMatrix(const tstring& strKey)
{
	TM_MATRIX::iterator itfound = m_mapMatrixs.find(strKey);
	if (itfound == m_mapMatrixs.end()) return false;

	m_mapMatrixs.erase(itfound);
	return true;
}

bool COERenderData_Impl::GetMatrix(CMatrix4x4& matValue, const tstring& strKey)
{
	TM_MATRIX::iterator itfound = m_mapMatrixs.find(strKey);
	if (itfound == m_mapMatrixs.end()) return false;

	matValue = itfound->second;
	return true;
}

bool COERenderData_Impl::SetTexture(const tstring& strKey, IOETexture* pTexture)
{
	TM_TEXTURE::iterator itfound = m_mapTextures.find(strKey);
	if (itfound == m_mapTextures.end())
	{
		m_mapTextures.insert(std::make_pair(strKey, pTexture));
	}
	else
	{
		itfound->second = pTexture;
	}

	return true;
}

bool COERenderData_Impl::RemoveTexture(const tstring& strKey)
{
	TM_TEXTURE::iterator itfound = m_mapTextures.find(strKey);
	if (itfound == m_mapTextures.end()) return false;

	m_mapTextures.erase(itfound);
	return true;
}

IOETexture* COERenderData_Impl::GetTexture(const tstring& strKey)
{
	TM_TEXTURE::iterator itfound = m_mapTextures.find(strKey);
	if (itfound == m_mapTextures.end()) return false;

	return itfound->second;
}

bool COERenderData_Impl::SetMesh(const tstring& strKey, IOEMesh* pMesh)
{
	TM_MESH::iterator itfound = m_mapMeshes.find(strKey);
	if (itfound == m_mapMeshes.end())
	{
		m_mapMeshes.insert(std::make_pair(strKey, pMesh));
	}
	else
	{
		itfound->second = pMesh;
	}

	return true;
}

bool COERenderData_Impl::RemoveMesh(const tstring& strKey)
{
	TM_MESH::iterator itfound = m_mapMeshes.find(strKey);
	if (itfound == m_mapMeshes.end()) return false;

	m_mapMeshes.erase(itfound);
	return true;
}

IOEMesh* COERenderData_Impl::GetMesh(const tstring& strKey)
{
	TM_MESH::iterator itfound = m_mapMeshes.find(strKey);
	if (itfound == m_mapMeshes.end()) return false;

	return itfound->second;
}

bool COERenderData_Impl::SetAnimData(const tstring& strKey, IOEAnimData* pAnimData)
{
	TM_ANIM_DATA::iterator itfound = m_mapAnimDatas.find(strKey);
	if (itfound == m_mapAnimDatas.end())
	{
		m_mapAnimDatas.insert(std::make_pair(strKey, pAnimData));
	}
	else
	{
		itfound->second = pAnimData;
	}

	return true;
}

bool COERenderData_Impl::RemoveAnimData(const tstring& strKey)
{
	TM_ANIM_DATA::iterator itfound = m_mapAnimDatas.find(strKey);
	if (itfound == m_mapAnimDatas.end()) return false;

	m_mapAnimDatas.erase(itfound);
	return true;
}

IOEAnimData* COERenderData_Impl::GetAnimData(const tstring& strKey)
{
	TM_ANIM_DATA::iterator itfound = m_mapAnimDatas.find(strKey);
	if (itfound == m_mapAnimDatas.end()) return NULL;

	return itfound->second;
}

bool COERenderData_Impl::SetMaterial(const tstring& strKey, IOEMaterial* pMaterial)
{
	TM_MATERIAL::iterator itfound = m_mapMaterials.find(strKey);
	if (itfound == m_mapMaterials.end())
	{
		m_mapMaterials.insert(std::make_pair(strKey, pMaterial));
	}
	else
	{
		itfound->second = pMaterial;
	}

	return true;
}

bool COERenderData_Impl::RemoveMaterial(const tstring& strKey)
{
	TM_MATERIAL::iterator itfound = m_mapMaterials.find(strKey);
	if (itfound == m_mapMaterials.end()) return false;

	m_mapMaterials.erase(itfound);
	return true;
}

IOEMaterial* COERenderData_Impl::GetMaterial(const tstring& strKey)
{
	TM_MATERIAL::iterator itfound = m_mapMaterials.find(strKey);
	if (itfound == m_mapMaterials.end()) return false;

	return itfound->second;
}

bool COERenderData_Impl::SetMaterialsList(const tstring& strKey, IOEMaterialsList* pMaterialsList)
{
	TM_MATERIALS_LIST::iterator itfound = m_mapMaterialsList.find(strKey);
	if (itfound == m_mapMaterialsList.end())
	{
		m_mapMaterialsList.insert(std::make_pair(strKey, pMaterialsList));
	}
	else
	{
		itfound->second = pMaterialsList;
	}

	return true;
}

bool COERenderData_Impl::RemoveMaterialsList(const tstring& strKey)
{
	TM_MATERIALS_LIST::iterator itfound = m_mapMaterialsList.find(strKey);
	if (itfound == m_mapMaterialsList.end()) return false;

	m_mapMaterialsList.erase(itfound);
	return false;
}

IOEMaterialsList* COERenderData_Impl::GetMaterialsList(const tstring& strKey)
{
	TM_MATERIALS_LIST::iterator itfound = m_mapMaterialsList.find(strKey);
	if (itfound == m_mapMaterialsList.end()) return false;

	return itfound->second;
}

bool COERenderData_Impl::SetObject(const tstring& strKey, IOEObject* pObject)
{
	TM_OBJECT::iterator itfound = m_mapObjects.find(strKey);
	if (itfound == m_mapObjects.end())
	{
		m_mapObjects.insert(std::make_pair(strKey, pObject));
	}
	else
	{
		itfound->second = pObject;
	}

	return true;
}

bool COERenderData_Impl::RemoveObject(const tstring& strKey)
{
	TM_OBJECT::iterator itfound = m_mapObjects.find(strKey);
	if (itfound == m_mapObjects.end()) return false;

	m_mapObjects.erase(itfound);
	return true;
}

IOEObject* COERenderData_Impl::GetObject(const tstring& strKey)
{
	TM_OBJECT::iterator itfound = m_mapObjects.find(strKey);
	if (itfound == m_mapObjects.end()) return false;

	return itfound->second;
}
