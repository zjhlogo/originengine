/*!
 * \file OERenderData_Impl.h
 * \date 10-12-2010 17:14:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OERENDERDATA_IMPL_H__
#define __OERENDERDATA_IMPL_H__

#include <OECore/IOERenderData.h>
#include <OECore/IOEResMgr.h>
#include <OEBase/IOEXmlMgr.h>
#include <map>

class COERenderData_Impl : public IOERenderData
{
public:
	typedef std::map<tstring, int> TM_INT;
	typedef std::map<tstring, float> TM_FLOAT;
	typedef std::map<tstring, CVector3> TM_VECTOR;
	typedef std::map<tstring, CMatrix4x4> TM_MATRIX;
	typedef std::map<tstring, IOETexture*> TM_TEXTURE;
	typedef std::map<tstring, IOEMesh*> TM_MESH;
	typedef std::map<tstring, IOEAnimData*> TM_ANIM_DATA;
	typedef std::map<tstring, IOEMaterial*> TM_MATERIAL;
	typedef std::map<tstring, IOEMaterialsList*> TM_MATERIALS_LIST;
	typedef std::map<tstring, IOEObject*> TM_OBJECT;

public:
	RTTI_DEF(COERenderData_Impl, IOERenderData);

	COERenderData_Impl();
	virtual ~COERenderData_Impl();

	virtual bool SetInt(const tstring& strKey, int nValue);
	virtual bool RemoveInt(const tstring& strKey);
	virtual bool GetInt(int& nValue, const tstring& strKey);

	virtual bool SetFloat(const tstring& strKey, float fValue);
	virtual bool RemoveFloat(const tstring& strKey);
	virtual bool GetFloat(float& fValue, const tstring& strKey);

	virtual bool SetVector(const tstring& strKey, const CVector3& vValue);
	virtual bool RemoveVector(const tstring& strKey);
	virtual bool GetVector(CVector3& vValue, const tstring& strKey);

	virtual bool SetMatrix(const tstring& strKey, const CMatrix4x4& matValue);
	virtual bool RemoveMatrix(const tstring& strKey);
	virtual bool GetMatrix(CMatrix4x4& matValue, const tstring& strKey);

	virtual bool SetTexture(const tstring& strKey, IOETexture* pTexture);
	virtual bool RemoveTexture(const tstring& strKey);
	virtual IOETexture* GetTexture(const tstring& strKey);

	virtual bool SetMesh(const tstring& strKey, IOEMesh* pMesh);
	virtual bool RemoveMesh(const tstring& strKey);
	virtual IOEMesh* GetMesh(const tstring& strKey);

	virtual bool SetAnimData(const tstring& strKey, IOEAnimData* pAnimData);
	virtual bool RemoveAnimData(const tstring& strKey);
	virtual IOEAnimData* GetAnimData(const tstring& strKey);

	virtual bool SetMaterial(const tstring& strKey, IOEMaterial* pMaterial);
	virtual bool RemoveMaterial(const tstring& strKey);
	virtual IOEMaterial* GetMaterial(const tstring& strKey);

	virtual bool SetMaterialsList(const tstring& strKey, IOEMaterialsList* pMaterialsList);
	virtual bool RemoveMaterialsList(const tstring& strKey);
	virtual IOEMaterialsList* GetMaterialsList(const tstring& strKey);

	virtual bool SetObject(const tstring& strKey, IOEObject* pObject);
	virtual bool RemoveObject(const tstring& strKey);
	virtual IOEObject* GetObject(const tstring& strKey);

private:
	bool Init();
	void Destroy();

private:
	TM_INT m_mapInts;
	TM_FLOAT m_mapFloats;
	TM_VECTOR m_mapVectors;
	TM_MATRIX m_mapMatrixs;
	TM_TEXTURE m_mapTextures;
	TM_MESH m_mapMeshes;
	TM_ANIM_DATA m_mapAnimDatas;
	TM_MATERIAL m_mapMaterials;
	TM_MATERIALS_LIST m_mapMaterialsList;
	TM_OBJECT m_mapObjects;

};
#endif // __OERENDERDATA_IMPL_H__
