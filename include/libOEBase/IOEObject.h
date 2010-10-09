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
#include "IOEMsg.h"
#include "OERtti.h"
#include <map>

class IOEObject;
typedef bool (IOEObject::*MSG_FUNC)(IOEMsg& msg);

class IOEObject
{
public:
	typedef struct EVENT_HANDLER_tag
	{
		IOEObject* pHandler;
		MSG_FUNC pFunc;
		int nDepth;
	} EVENT_HANDLER;

	typedef std::multimap<uint, EVENT_HANDLER> TM_EVENT_HANDLER;
	typedef std::pair<TM_EVENT_HANDLER::iterator, TM_EVENT_HANDLER::iterator> TP_EVENT_HANDLER;

public:
	RTTI_DEF(IOEObject, CNoRtti);

	IOEObject();
	virtual ~IOEObject();

	virtual bool IsOK();
	virtual void Release();

	virtual void RegisterEvent(uint nMsgID, IOEObject* pHandler, MSG_FUNC pFunc);
	virtual void UnregisterEvent(uint nMsgID, IOEObject* pHandler);
	bool CallEvent(IOEMsg& msg);

	int IncRef();
	int DecRef();
	int GetRef() const;

protected:
	bool m_bOK;

private:
	int m_nRef;
	TM_EVENT_HANDLER m_EventsMap;

};

#endif // __IOEOBJECT_H__
