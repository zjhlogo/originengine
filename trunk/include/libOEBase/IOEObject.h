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

class IOEObject
{
public:
	IOEObject(const tstring& strClassName);
	virtual ~IOEObject();

	virtual bool IsOK();
	virtual void Release();

	const tstring& GetClassName();

	int IncRef();
	int DecRef();
	int GetRef() const;

protected:
	bool m_bOK;

private:
	int m_nRef;
	tstring m_strClassName;

};

#endif // __IOEOBJECT_H__
