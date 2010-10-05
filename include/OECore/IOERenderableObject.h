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

class IOERenderableObject : public IOEObject
{
public:
	IOERenderableObject(const tstring& strRenderableClassName):IOEObject(TS("IOERenderableObject")) {m_strRenderableClassName = strRenderableClassName;};
	virtual ~IOERenderableObject() {};

	const tstring& GetClassName() {return m_strRenderableClassName;};

	virtual void Update(float fDetailTime) = 0;
	virtual void Render(float fDetailTime) = 0;

private:
	tstring m_strRenderableClassName;

};
#endif // __IOERENDERABLEOBJECT_H__
