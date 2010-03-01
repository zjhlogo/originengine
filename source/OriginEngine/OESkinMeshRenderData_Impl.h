/*!
 * \file OESkinMeshRenderData_Impl.h
 * \date 13-2-2010 19:57:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OESKINMESHRENDERDATA_IMPL_H__
#define __OESKINMESHRENDERDATA_IMPL_H__

#include <IOERenderData.h>
#include <IOEResMgr.h>
#include <IOEXmlMgr.h>

class COESkinMeshRenderData_Impl : public IOERenderData
{
public:
	COESkinMeshRenderData_Impl();
	virtual ~COESkinMeshRenderData_Impl();

	bool LoadMesh(const tstring& strFile);
	bool LoadBone(const tstring& strFile);
	bool LoadMaterials(IOEXmlNode* pXmlMaterials);

	IOEMesh* GetMesh();
	IOEBones* GetBones();
	TV_MATRIX& GetSkinMatrix();
	TV_MATERIAL& GetMaterials();

	void SetAnimLength(float fAnimLength);
	float GetAnimLength() const;

	void SetTotalTime(float fTotalTime);
	float GetTotalTime() const;

private:
	bool Init();
	void Destroy();

	bool CreateMesh(const tstring& strFile);
	bool CreateBone(const tstring& strFile);
	bool CreateMaterials(IOEXmlNode* pXmlMaterials);

	void DestroyMesh();
	void DestroyBone();
	void DestroyMaterials();

private:
	IOEMesh* m_pMesh;
	IOEBones* m_pBones;

	TV_MATRIX m_vmatSkin;
	TV_MATERIAL m_vMaterials;

	float m_fAnimLength;
	float m_fTotalTime;

};
#endif // __OESKINMESHRENDERDATA_IMPL_H__
