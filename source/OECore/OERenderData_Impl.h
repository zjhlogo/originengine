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
	typedef std::map<int, IOEMaterial*> TM_MATERIAL;

public:
	RTTI_DEF(COERenderData_Impl, IOERenderData);

	COERenderData_Impl(IOEObject* pHolder, IOEXmlNode* pXmlRenderData);
	virtual ~COERenderData_Impl();

	virtual IOEMesh* GetMesh();

	virtual IOESkeleton* GetSkeleton();
	virtual TV_MATRIX4X4& GetSkinMatrix();
	virtual IOEMaterial* GetMaterial(int nIndex);

	virtual void SetAnimLength(float fAnimLength);
	virtual float GetAnimLength() const;

	virtual void SetTotalTime(float fTotalTime);
	virtual float GetTotalTime() const;

private:
	void Init();
	void Destroy();

	bool CreateRenderData(IOEXmlNode* pXmlRenderData);

	bool LoadMesh(const tstring& strFile);
	bool LoadSkeleton(const tstring& strFile);
	bool LoadMaterials(IOEXmlNode* pXmlMaterials);

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
#endif // __OERENDERDATA_IMPL_H__
