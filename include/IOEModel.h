/*!
 * \file IOEModel.h
 * \date 9-8-2009 17:34:58
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMODEL_H__
#define __IOEMODEL_H__

#include "IOEObject.h"
#include "IOEMesh.h"
#include "IOETexture.h"
#include "IOEShader.h"

class IOEModel : public IOEObject
{
public:
	typedef struct MATERIAL_tag
	{
		int nID;
		int nVertDecl;
		tstring strShaderFile;
		tstring strTextureFile;
		tstring strTexNormalFile;
		IOEShader* pShader;
		IOETexture* pTexture;
		IOETexture* pTexNormal;
		// TODO: some additional parameter
	} MATERIAL;

public:
	IOEModel() {};
	virtual ~IOEModel() {};

	virtual void Update(float fDetailTime) = 0;
	virtual void Render(float fDetailTime) = 0;

	virtual IOEMesh* GetMesh() = 0;
	virtual int GetNumMatrixPalette() = 0;
	virtual CMatrix4x4* GetMatrixPalette() = 0;

	virtual int GetNumMaterials() = 0;
	virtual MATERIAL* GetMaterial(int nIndex) = 0;
};

#endif // __IOEMODEL_H__
