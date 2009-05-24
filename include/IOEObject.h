/*!
 * \file IOEObject.h
 * \date 24-5-2009 17:49:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEOBJECT_H__
#define __IOEOBJECT_H__

#include "OEBasicType.h"

class IOEObject
{
public:
	IOEObject() {m_bOK = false;};
	virtual ~IOEObject() {};

	virtual bool IsOK() {return m_bOK;};
	virtual void Release() {delete this;};

protected:
	bool m_bOK;

};
#endif // __IOEOBJECT_H__