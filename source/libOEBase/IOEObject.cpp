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
	// TODO: 
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
