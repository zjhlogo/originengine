/*!
 * \file IOEObject.h
 * \date 24-5-2009 17:49:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEOBJECT_H__
#define __IOEOBJECT_H__

#include "OEBaseType.h"
#include "OEDataBufferRead.h"
#include "OERtti.h"

class IOEObject
{
public:
	RTTI_DEF(IOEObject, CNoRtti);

	IOEObject();
	virtual ~IOEObject();

	virtual bool IsOK();
	virtual void Release();

	int IncRef();
	int DecRef();
	int GetRef() const;

protected:
	bool m_bOK;

private:
	int m_nRef;

};

#endif // __IOEOBJECT_H__
