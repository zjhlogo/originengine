/*!
 * \file IOEObject.cpp
 * \date 8-17-2010 22:51:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEBase/IOEObject.h>
#include <libOEMsg/OEMsgDestroy.h>
#include <assert.h>

IOEObject::IOEObject()
{
	m_bOK = false;
	m_nRef = 1;
}

IOEObject::~IOEObject()
{
	COEMsgDestroy msgDestroy(this);
	CallEvent(msgDestroy);
}

bool IOEObject::IsOK()
{
	return m_bOK;
}

void IOEObject::Release()
{
	if (DecRef() <= 0) delete this;
}

int IOEObject::IncRef()
{
	return ++m_nRef;
}

int IOEObject::DecRef()
{
	return --m_nRef;
}

int IOEObject::GetRef() const
{
	return m_nRef;
}

void IOEObject::RegisterEvent(uint nMsgID, IOEObject* pHandler, MSG_FUNC pFunc)
{
	EVENT_HANDLER handler;
	handler.pHandler = pHandler;
	handler.pFunc = pFunc;
	handler.nDepth = 0;
	m_EventsMap.insert(std::make_pair(nMsgID, handler));
}

void IOEObject::UnregisterEvent(uint nMsgID, IOEObject* pHandler)
{
	// TODO: 
}

bool IOEObject::CallEvent(IOEMsg& msg)
{
	TP_EVENT_HANDLER range = m_EventsMap.equal_range(msg.GetMsgID());
	for (TM_EVENT_HANDLER::iterator it = range.first; it != range.second; ++it)
	{
		EVENT_HANDLER& handler = it->second;

		// check the loop depth, it must always 0 or 1
		if (handler.nDepth > 0)
		{
			assert(false);
			continue;
		}
		++handler.nDepth;

		if (!(handler.pHandler->*handler.pFunc)(msg))
		{
			assert(false);
		}

		--handler.nDepth;
	}

	return true;
}
