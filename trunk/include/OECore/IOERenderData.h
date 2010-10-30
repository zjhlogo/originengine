/*!
 * \file IOERenderData.h
 * \date 12-2-2010 20:00:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDERDATA_H__
#define __IOERENDERDATA_H__

#include "../libOEBase/IOEObject.h"
#include "IOEMesh.h"
#include "IOEMaterial.h"
#include "IOEAnimData.h"
#include "IOEMaterialsList.h"

class IOERenderData : public IOEObject
{
public:
	RTTI_DEF(IOERenderData, IOEObject);

	IOERenderData() {};
	virtual ~IOERenderData() {};

	virtual bool SetInt(const tstring& strKey, int nValue) = 0;
	virtual bool RemoveInt(const tstring& strKey) = 0;
	virtual bool GetInt(int& nValue, const tstring& strKey) = 0;

	virtual bool SetFloat(const tstring& strKey, float fValue) = 0;
	virtual bool RemoveFloat(const tstring& strKey) = 0;
	virtual bool GetFloat(float& fValue, const tstring& strKey) = 0;

	virtual bool SetVector(const tstring& strKey, const CVector3& vValue) = 0;
	virtual bool RemoveVector(const tstring& strKey) = 0;
	virtual bool GetVector(CVector3& vValue, const tstring& strKey) = 0;

	virtual bool SetMatrix(const tstring& strKey, const CMatrix4x4& matValue) = 0;
	virtual bool RemoveMatrix(const tstring& strKey) = 0;
	virtual bool GetMatrix(CMatrix4x4& matOut, const tstring& strKey) = 0;

	virtual bool SetTexture(const tstring& strKey, IOETexture* pTexture) = 0;
	virtual bool RemoveTexture(const tstring& strKey) = 0;
	virtual IOETexture* GetTexture(const tstring& strKey) = 0;

	virtual bool SetMesh(const tstring& strKey, IOEMesh* pMesh) = 0;
	virtual bool RemoveMesh(const tstring& strKey) = 0;
	virtual IOEMesh* GetMesh(const tstring& strKey) = 0;

	virtual bool SetAnimData(const tstring& strKey, IOEAnimData* pAnimData) = 0;
	virtual bool RemoveAnimData(const tstring& strKey) = 0;
	virtual IOEAnimData* GetAnimData(const tstring& strKey) = 0;

	virtual bool SetMaterial(const tstring& strKey, IOEMaterial* pMaterial) = 0;
	virtual bool RemoveMaterial(const tstring& strKey) = 0;
	virtual IOEMaterial* GetMaterial(const tstring& strKey) = 0;

	virtual bool SetMaterialsList(const tstring& strKey, IOEMaterialsList* pMaterialsList) = 0;
	virtual bool RemoveMaterialsList(const tstring& strKey) = 0;
	virtual IOEMaterialsList* GetMaterialsList(const tstring& strKey) = 0;

	virtual bool SetObject(const tstring& strKey, IOEObject* pObject) = 0;
	virtual bool RemoveObject(const tstring& strKey) = 0;
	virtual IOEObject* GetObject(const tstring& strKey) = 0;

};
#endif // __IOERENDERDATA_H__
