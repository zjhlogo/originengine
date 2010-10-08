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
#include <set>

class IOEObject
{
public:
	typedef std::set<IOEObject*> TS_OBJECT;

public:
	RTTI_DEF(IOEObject, CNoRtti);

	IOEObject();
	virtual ~IOEObject();

	virtual bool IsOK();
	virtual void Release();

	void AddDestroyReceiver(IOEObject* pObject);
	void RemoveDestroyReceiver(IOEObject* pObject);
	virtual void NotifyDestroy(IOEObject* pObject);

	int IncRef();
	int DecRef();
	int GetRef() const;

protected:
	bool m_bOK;

private:
	int m_nRef;
	TS_OBJECT m_sNotifier;

};

#endif // __IOEOBJECT_H__
