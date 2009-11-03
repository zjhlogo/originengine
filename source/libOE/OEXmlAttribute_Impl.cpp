/*!
 * \file OEXmlAttribute_Impl.cpp
 * \date 11-10-2009 10:07:10
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlAttribute_Impl.h"

COEXmlAttribute_Impl::COEXmlAttribute_Impl(const tstring& strName)
{
	Init();
	SetName(strName);
	m_bOK = true;
}

COEXmlAttribute_Impl::~COEXmlAttribute_Impl()
{
	Destroy();
}

void COEXmlAttribute_Impl::Init()
{
	m_pAttributeSibling = NULL;
}

void COEXmlAttribute_Impl::Destroy()
{
	// TODO: 
}

const tstring& COEXmlAttribute_Impl::GetName()
{
	return m_strName;
}

void COEXmlAttribute_Impl::SetName(const tstring& strName)
{
	m_strName = strName;
}

const tstring& COEXmlAttribute_Impl::GetValue()
{
	return m_strValue;
}

void COEXmlAttribute_Impl::SetValue(const tstring& strValue)
{
	m_strValue = strValue;
}

bool COEXmlAttribute_Impl::ToString(tstring& strNode)
{
	strNode += m_strName;
	strNode += t("=\"");
	strNode += m_strValue;
	strNode += t("\"");
	return true;
}

void COEXmlAttribute_Impl::LinkSibling(COEXmlAttribute_Impl* pAttributeSibling)
{
	if (m_pAttributeSibling)
	{
		COEXmlAttribute_Impl* pEndSibling = m_pAttributeSibling;
		while (pEndSibling->m_pAttributeSibling)
		{
			pEndSibling = pEndSibling->m_pAttributeSibling;
		}

		pEndSibling->m_pAttributeSibling = pAttributeSibling;
	}
	else
	{
		m_pAttributeSibling = pAttributeSibling;
	}
}

COEXmlAttribute_Impl* COEXmlAttribute_Impl::GetNextSibling()
{
	return m_pAttributeSibling;
}
