/*!
 * \file OESkinMeshRenderData_Impl.h
 * \date 13-2-2010 19:57:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OESKINMESHRENDERDATA_IMPL_H__
#define __OESKINMESHRENDERDATA_IMPL_H__

#include <OECore/IOERenderData.h>
#include <OECore/IOEResMgr.h>
#include <OEBase/IOEXmlMgr.h>
#include <map>

class COESkinMeshRenderData_Impl : public IOERenderData
{
public:
	typedef std::map<int, IOEMaterial*> TM_MATERIAL;

public:
	RTTI_DEF(COESkinMeshRenderData_Impl, IOERenderData);

	COESkinMeshRenderData_Impl(IOEObject* pHolder);
	virtual ~COESkinMeshRenderData_Impl();

	virtual IOEMesh* GetMesh();

	bool LoadMesh(const tstring& strFile);
	bool LoadSkeleton(const tstring& strFile);
	bool LoadMaterials(IOEXmlNode* pXmlMaterials);

	IOESkeleton* GetSkeleton();
	TV_MATRIX4X4& GetSkinMatrix();
	IOEMaterial* GetMaterial(int nIndex);

	void SetAnimLength(float fAnimLength);
	float GetAnimLength() const;

	void SetTotalTime(float fTotalTime);
	float GetTotalTime() const;

private:
	bool Init();
	void Destroy();

	bool CreateMesh(const tstring& strFile);
	bool CreateSkeleton(const tstring& strFile);
	bool CreateMaterials(IOEXmlNode* pXmlMaterials);

	void DestroyMesh();
	void DestroySkeleton();
	void DestroyMaterials();

private:
	IOEMesh* m_pMesh;
	IOESkeleton* m_pSkeleton;

	TV_MATRIX4X4 m_vmatSkin;
	TM_MATERIAL m_MaterialMap;

	float m_fAnimLength;
	float m_fTotalTime;

};
#endif // __OESKINMESHRENDERDATA_IMPL_H__
