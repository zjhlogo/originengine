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
	IOEObject()
	{
		m_bOK = false;
		m_nRef = 1;
	};

	virtual ~IOEObject()
	{
		// TODO: 
	};

	virtual bool IsOK()
	{
		return m_bOK;
	};

	virtual void Release()
	{
		if (DecRef() <= 0) delete this;
	};

	int IncRef()
	{
		return ++m_nRef;
	};

	int DecRef()
	{
		return --m_nRef;
	};

	int GetRef() const
	{
		return m_nRef;
	};

protected:
	bool m_bOK;
	int m_nRef;

};
#endif // __IOEOBJECT_H__
