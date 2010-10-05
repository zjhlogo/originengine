/*!
 * \file IOEObject.cpp
 * \date 8-17-2010 22:51:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEBase/IOEObject.h>

IOEObject::IOEObject(const tstring& strClassName)
{
	m_bOK = false;
	m_nRef = 1;
	m_strClassName = strClassName;
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

const tstring& IOEObject::GetClassName()
{
	return m_strClassName;
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
