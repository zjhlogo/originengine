/*!
 * \file IOEUIRenderableObject.h
 * \date 10-7-2010 21:42:31
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOEUIRENDERABLEOBJECT_H__
#define __IOEUIRENDERABLEOBJECT_H__

#include "../libOEBase/IOEObject.h"

class IOEUIRenderableObject : public IOEObject
{
public:
	RTTI_DEF(IOEUIRenderableObject, IOEObject);

	IOEUIRenderableObject() {};
	virtual ~IOEUIRenderableObject() {};

	virtual void Update(float fDetailTime) = 0;
	virtual void Render(float fDetailTime) = 0;

};
#endif // __IOEUIRENDERABLEOBJECT_H__
