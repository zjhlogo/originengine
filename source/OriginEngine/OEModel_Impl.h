/*!
 * \file OEModel_Impl.h
 * \date 9-8-2009 17:49:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODEL_IMPL_H__
#define __OEMODEL_IMPL_H__

#include <IOEModel.h>
#include <IOEShader.h>
#include <IOETexture.h>
#include <IOEXmlNode.h>

#include <vector>
#include <map>

class COEModel_Impl : public IOEModel
{
public:
	typedef std::vector<MATERIAL> TV_MATERIAL;

	typedef std::vector<CMatrix4x4> TV_MATRIX;

	typedef struct VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		float tx, ty, tz;
		uchar nBoneIndex[4];
		float fWeight[4];
	} VERTEX;

	typedef std::vector<VERTEX> TV_VERTEX;

public:
	COEModel_Impl(const tstring& strFile);
	virtual ~COEModel_Impl();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual IOEMesh* GetMesh();
	virtual int GetNumMatrixPalette();
	virtual CMatrix4x4* GetMatrixPalette();

	virtual int GetNumMaterials();
	virtual MATERIAL* GetMaterial(int nIndex);

private:
	void Init();
	void Destroy();
	void ClearMaterials();

	bool Create(const tstring& strFile);
	bool CreateMesh(IOEXmlNode* pXmlMesh);
	bool CreateMaterials(IOEXmlNode* pXmlMaterials);

	IOEShader* CreateShader(int nVertDecl, const tstring& strShaderFile);

	void SoftSkinned(IOEPiece* pPiece, TV_MATRIX& vmatSkin);

private:
	IOEMesh* m_pMesh;

	TV_MATRIX m_vmatSkin;
	float m_fTotalTime;

	TV_MATERIAL m_vMaterial;

	TV_VERTEX m_vVerts;

};

#endif // __OEMODEL_IMPL_H__
