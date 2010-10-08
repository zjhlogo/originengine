/*!
 * \file IOEObject.cpp
 * \date 8-17-2010 22:51:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEBase/IOEObject.h>

IOEObject::IOEObject()
{
	m_bOK = false;
	m_nRef = 1;
}

IOEObject::~IOEObject()
{
	for (TS_OBJECT::iterator it = m_sNotifier.begin(); it != m_sNotifier.end(); ++it)
	{
		IOEObject* pObject = (*it);
		pObject->NotifyDestroy(this);
		pObject->RemoveDestroyReceiver(this);
	}
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

void IOEObject::AddDestroyReceiver(IOEObject* pObject)
{
	m_sNotifier.insert(pObject);
}

void IOEObject::RemoveDestroyReceiver(IOEObject* pObject)
{
	TS_OBJECT::iterator itfound = m_sNotifier.find(pObject);
	if (itfound == m_sNotifier.end()) return;

	m_sNotifier.erase(itfound);
}

void IOEObject::NotifyDestroy(IOEObject* pObject)
{
	// TODO: 
}
