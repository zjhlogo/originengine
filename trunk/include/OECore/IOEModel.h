/*!
 * \file IOEModel.h
 * \date 9-8-2009 17:34:58
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMODEL_H__
#define __IOEMODEL_H__

#include "IOERenderableObject.h"
#include "IOEMesh.h"

class IOEModel : public IOERenderableObject
{
public:
	RTTI_DEF(IOEModel, IOERenderableObject);

	IOEModel() {};
	virtual ~IOEModel() {};

	virtual bool AddControl(const tstring& strControl) = 0;
	virtual void RemoveControl(const tstring& strControl) = 0;

	virtual bool AddRender(const tstring& strRender) = 0;
	virtual void RemoveRender(const tstring& strRender) = 0;
};

#endif // __IOEMODEL_H__
