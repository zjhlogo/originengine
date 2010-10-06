/*!
 * \file IOERenderableObject.h
 * \date 10-4-2010 22:30:23
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOERENDERABLEOBJECT_H__
#define __IOERENDERABLEOBJECT_H__

#include "../libOEBase/IOEObject.h"
#include "IOERenderData.h"

class IOERenderableObject : public IOEObject
{
public:
	RTTI_DEF(IOERenderableObject, IOEObject);

	IOERenderableObject() {};
	virtual ~IOERenderableObject() {};

	virtual void Update(float fDetailTime) = 0;
	virtual void Render(float fDetailTime) = 0;

	virtual IOERenderData* GetRenderData() = 0;

};
#endif // __IOERENDERABLEOBJECT_H__
