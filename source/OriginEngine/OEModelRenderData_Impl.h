/*!
 * \file OEModelRenderData_Impl.h
 * \date 12-2-2010 20:05:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODELRENDERDATA_IMPL_H__
#define __OEMODELRENDERDATA_IMPL_H__

#include <IOERenderData.h>
#include <IOEResMgr.h>
#include <IOEXmlMgr.h>

class COEModelRenderData_Impl : public IOERenderData
{
public:
	COEModelRenderData_Impl();
	virtual ~COEModelRenderData_Impl();

	bool LoadMesh(const tstring& strFile);
	bool LoadBone(const tstring& strFile);
	bool LoadMaterials(IOEXmlNode* pXmlMaterials);

	IOEMesh* GetMesh();
	TV_BONE& GetBones();
	TV_MATRIX& GetSkinMatrix();
	TV_MATERIAL& GetMaterials();

	void SetAnimLength(float fAnimLength);
	float GetAnimLength() const;

	void SetTotalTime(float fTotalTime);
	float GetTotalTime() const;

private:
	void Init();
	void Destroy();

	bool CreateMesh(const tstring& strFile);
	bool CreateBone(const tstring& strFile);
	bool CreateMaterials(IOEXmlNode* pXmlMaterials);
	IOEShader* CreateShader(int nVertDecl, const tstring& strFile);

	void DestroyMesh();
	void DestroyBone();
	void DestroyMaterials();

private:
	IOEMesh* m_pMesh;
	TV_BONE m_vBones;
	TV_MATRIX m_vmatSkin;
	TV_MATERIAL m_vMaterials;

	float m_fAnimLength;
	float m_fTotalTime;

};
#endif // __OEMODELRENDERDATA_IMPL_H__
