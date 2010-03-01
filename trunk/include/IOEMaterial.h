/*!
 * \file IOEMaterial.h
 * \date 1-3-2010 19:30:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMATERIAL_H__
#define __IOEMATERIAL_H__

#include "OEBaseTypeEx.h"
#include "IOEObject.h"
#include "IOETexture.h"
#include "IOEShader.h"

#include <vector>

class IOEMaterial : public IOEObject
{
public:
	IOEMaterial() {};
	virtual ~IOEMaterial() {};

	virtual int GetID() = 0;
	virtual int GetVertDeclMask() = 0;
	virtual IOEShader* GetShader() = 0;
	virtual IOETexture* GetTexture(MATERIAL_TEXTURE_TYPE eType) = 0;

};

typedef std::vector<IOEMaterial*> TV_MATERIAL;

#endif // __IOEMATERIAL_H__
