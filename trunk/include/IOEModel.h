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

class IOEModel : public IOEObject
{
public:
	IOEModel() {};
	virtual ~IOEModel() {};

	virtual void Update(float fDetailTime) = 0;
	virtual void Render(float fDetailTime) = 0;

	virtual IOEMesh* GetMesh() = 0;
	virtual int GetNumMatrixPalette() = 0;
	virtual CMatrix4x4* GetMatrixPalette() = 0;
};

#endif // __IOEMODEL_H__
