/*!
 * \file OERtti.cpp
 * \date 10-5-2010 18:11:31
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEBase/OERtti.h>

COERtti::COERtti(const tstring& strTypeName, const COERtti* pBaseRtti)
{
	m_strTypeName = strTypeName;
	m_pBaseRtti = pBaseRtti;
}

COERtti::~COERtti()
{
	// TODO: 
}

const tstring& COERtti::GetClassName() const
{
	return m_strTypeName;
}

const COERtti* COERtti::GetBaseRtti() const
{
	return m_pBaseRtti;
}

bool COERtti::IsType(const tstring& strTypeName) const
{
	return m_strTypeName == strTypeName;
}

bool COERtti::IsDerived(const tstring& strTypeName) const
{
	const COERtti* pRtti = m_pBaseRtti;

	while (pRtti)
	{
		if (pRtti->GetClassName() == strTypeName) return true;
		pRtti = pRtti->GetBaseRtti();
	}

	return false;
}
