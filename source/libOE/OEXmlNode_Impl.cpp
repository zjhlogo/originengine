/*!
 * \file OEXmlNode_Impl.cpp
 * \date 24-7-2009 16:22:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlNode_Impl.h"
#include <OEOS.h>

#include <IOEXmlMgr.h>

COEXmlNode_Impl::COEXmlNode_Impl(const tstring& strName)
{
	Init();
	SetName(strName);
	m_bOK = true;
}

COEXmlNode_Impl::~COEXmlNode_Impl()
{
	Destroy();
}

void COEXmlNode_Impl::Init()
{
	m_pAttribute = NULL;
	m_pNodeChild = NULL;
	m_pNodeSibling = NULL;
}

void COEXmlNode_Impl::Destroy()
{
	COEXmlAttribute_Impl* pAttribute = m_pAttribute;
	while (pAttribute)
	{
		COEXmlAttribute_Impl* pAttributeDel = pAttribute;
		pAttribute = pAttribute->GetNextSibling();
		SAFE_RELEASE(pAttributeDel);
	}

	COEXmlNode_Impl* pNodeChild = m_pNodeChild;
	while (pNodeChild)
	{
		COEXmlNode_Impl* pNodeChildDel = pNodeChild;
		pNodeChild = pNodeChild->m_pNodeSibling;
		SAFE_RELEASE(pNodeChildDel);
	}

	m_pAttribute = NULL;
	m_pNodeChild = NULL;
	m_pNodeSibling = NULL;
}

const tstring& COEXmlNode_Impl::GetName()
{
	return m_strName;
}

void COEXmlNode_Impl::SetName(const tstring& strName)
{
	m_strName = strName;
}

bool COEXmlNode_Impl::GetAttribute(int& nValue, const tstring& strName)
{
	IOEXmlAttribute* pAttribute = FirstAttribute(strName);
	if (!pAttribute) return false;

	return COEOS::str2int(nValue, pAttribute->GetValue().c_str());
}

bool COEXmlNode_Impl::GetAttribute(float& fValue, const tstring& strName)
{
	IOEXmlAttribute* pAttribute = FirstAttribute(strName);
	if (!pAttribute) return false;

	return COEOS::str2float(fValue, pAttribute->GetValue().c_str());
}

bool COEXmlNode_Impl::GetAttribute(tstring& strValue, const tstring& strName)
{
	IOEXmlAttribute* pAttribute = FirstAttribute(strName);
	if (!pAttribute) return false;

	strValue = pAttribute->GetValue();
	return true;
}

void COEXmlNode_Impl::SetAttribute(const tstring& strName, int nValue)
{
	IOEXmlAttribute* pAttribute = FirstAttribute(strName);
	if (!pAttribute)
	{
		pAttribute = CreateEndAttribute(strName);
		if (!pAttribute) return;
	}

	tstring strTemp;
	COEOS::int2str(strTemp, nValue);
	pAttribute->SetValue(strTemp);
}

void COEXmlNode_Impl::SetAttribute(const tstring& strName, float fValue)
{
	IOEXmlAttribute* pAttribute = FirstAttribute(strName);
	if (!pAttribute)
	{
		pAttribute = CreateEndAttribute(strName);
		if (!pAttribute) return;
	}

	tstring strTemp;
	COEOS::float2str(strTemp, fValue);
	pAttribute->SetValue(strTemp);
}

void COEXmlNode_Impl::SetAttribute(const tstring& strName, const tstring& strValue)
{
	IOEXmlAttribute* pAttribute = FirstAttribute(strName);
	if (!pAttribute)
	{
		pAttribute = CreateEndAttribute(strName);
		if (!pAttribute) return;
	}

	pAttribute->SetValue(strValue);
}

IOEXmlAttribute* COEXmlNode_Impl::FirstAttribute()
{
	return m_pAttribute;
}

IOEXmlAttribute* COEXmlNode_Impl::FirstAttribute(const tstring& strName)
{
	COEXmlAttribute_Impl* pAttribute = m_pAttribute;

	while (pAttribute)
	{
		if (pAttribute->GetName() == strName) return pAttribute;
		pAttribute = pAttribute->GetNextSibling();
	}

	return NULL;
}

bool COEXmlNode_Impl::GetText(int& nValue)
{
	if (!m_pNodeChild || !m_pNodeChild->m_strName.empty()) return false;
	return COEOS::str2int(nValue, m_pNodeChild->m_strText.c_str());
}

bool COEXmlNode_Impl::GetText(float& fValue)
{
	if (!m_pNodeChild || !m_pNodeChild->m_strName.empty()) return false;
	return COEOS::str2float(fValue, m_pNodeChild->m_strText.c_str());
}

bool COEXmlNode_Impl::GetText(tstring& strText)
{
	if (!m_pNodeChild || !m_pNodeChild->m_strName.empty()) return false;
	strText = m_pNodeChild->m_strText;
	return true;
}

bool COEXmlNode_Impl::SetText(int nValue)
{
	if (m_pNodeChild && !m_pNodeChild->m_strName.empty()) return false;

	if (!m_pNodeChild)
	{
		InsertChild(EMPTY_STRING);
		if (!m_pNodeChild) return false;
	}

	COEOS::int2str(m_pNodeChild->m_strText, nValue);
	return true;
}

bool COEXmlNode_Impl::SetText(float fValue)
{
	if (m_pNodeChild && !m_pNodeChild->m_strName.empty()) return false;

	if (!m_pNodeChild)
	{
		InsertChild(EMPTY_STRING);
		if (!m_pNodeChild) return false;
	}

	COEOS::float2str(m_pNodeChild->m_strText, fValue);
	return true;
}

bool COEXmlNode_Impl::SetText(const tstring& strText)
{
	if (m_pNodeChild && !m_pNodeChild->GetName().empty()) return false;

	if (!m_pNodeChild)
	{
		InsertChild(EMPTY_STRING);
		if (!m_pNodeChild) return false;
	}

	m_pNodeChild->m_strText = strText;
	return true;
}

IOEXmlNode* COEXmlNode_Impl::FirstChild()
{
	return m_pNodeChild;
}

IOEXmlNode* COEXmlNode_Impl::FirstChild(const tstring& strName)
{
	if (!m_pNodeChild) return NULL;
	if (m_pNodeChild->m_strName == strName) return m_pNodeChild;
	return m_pNodeChild->NextSibling(strName);
}

IOEXmlNode* COEXmlNode_Impl::EndChild()
{
	return EndSibling(m_pNodeChild);
}

IOEXmlNode* COEXmlNode_Impl::NextSibling()
{
	return m_pNodeSibling;
}

IOEXmlNode* COEXmlNode_Impl::NextSibling(const tstring& strName)
{
	COEXmlNode_Impl* pNodeSibling = m_pNodeSibling;
	while (pNodeSibling)
	{
		if (pNodeSibling->m_strName == strName) return pNodeSibling;
		pNodeSibling = pNodeSibling->m_pNodeSibling;
	}

	return NULL;
}

IOEXmlNode* COEXmlNode_Impl::EndSibling()
{
	return EndSibling(this);
}

IOEXmlNode* COEXmlNode_Impl::InsertChild(const tstring& strName, INSERT_NODE_BEHAVE eBehave /*= INB_NEXT*/)
{
	COEXmlNode_Impl* pNodeChild = (COEXmlNode_Impl*)g_pOEXmlMgr->CreateNode(strName);
	if (!pNodeChild) return NULL;

	switch (eBehave)
	{
	case INB_PREVIOUS:
		{
			pNodeChild->m_pNodeSibling = m_pNodeChild;
			m_pNodeChild = pNodeChild;
		}
		break;
	case INB_NEXT:
		{
			COEXmlNode_Impl* pEndChild = EndSibling(m_pNodeChild);
			if (pEndChild)
			{
				pEndChild->m_pNodeSibling = pNodeChild;
			}
			else
			{
				m_pNodeChild = pNodeChild;
			}
		}
		break;
	}

	return pNodeChild;
}

IOEXmlNode* COEXmlNode_Impl::InsertChild(const IOEXmlNode* pNodeChild, INSERT_NODE_BEHAVE eBehave /*= INB_NEXT*/)
{
	// TODO: 
	return NULL;
}

IOEXmlNode* COEXmlNode_Impl::InsertSibling(const tstring& strName, INSERT_NODE_BEHAVE eBehave /*= INB_NEXT*/)
{
	COEXmlNode_Impl* pNodeChild = (COEXmlNode_Impl*)g_pOEXmlMgr->CreateNode(strName);
	if (!pNodeChild) return NULL;

	switch (eBehave)
	{
	case INB_PREVIOUS:
		{
			// TODO: 
		}
		break;
	case INB_NEXT:
		{
			pNodeChild->m_pNodeSibling = m_pNodeSibling;
			m_pNodeSibling = pNodeChild;
		}
		break;
	}

	return pNodeChild;
}

IOEXmlNode* COEXmlNode_Impl::InsertSibling(const IOEXmlNode* pNodeBrother, INSERT_NODE_BEHAVE eBehave /*= INB_NEXT*/)
{
	// TODO: 
	return NULL;
}

void COEXmlNode_Impl::SetValue(const tstring& strValue)
{
	m_strText = strValue;
}

void COEXmlNode_Impl::LinkAttribute(COEXmlAttribute_Impl* pAttribute)
{
	if (!m_pAttribute)
	{
		m_pAttribute = pAttribute;
	}
	else
	{
		m_pAttribute->LinkSibling(pAttribute);
	}
}

void COEXmlNode_Impl::LinkChild(COEXmlNode_Impl* pNodeChild)
{
	COEXmlNode_Impl* pEndChild = EndSibling(m_pNodeChild);
	if (pEndChild)
	{
		pEndChild->m_pNodeSibling = pNodeChild;
	}
	else
	{
		m_pNodeChild = pNodeChild;
	}
}

void COEXmlNode_Impl::LinkSibling(COEXmlNode_Impl* pNodeSibling)
{
	COEXmlNode_Impl* pEndSibling = EndSibling(this);
	pEndSibling->m_pNodeSibling = pNodeSibling;
}

void COEXmlNode_Impl::UnlinkAll()
{
	m_pAttribute = NULL;
	m_pNodeChild = NULL;
	m_pNodeSibling = NULL;
}

COEXmlAttribute_Impl* COEXmlNode_Impl::CreateEndAttribute(const tstring& strName)
{
	COEXmlAttribute_Impl* pAttribute = (COEXmlAttribute_Impl*)g_pOEXmlMgr->CreateAttribute(strName);
	if (!pAttribute) return NULL;

	if (!m_pAttribute)
	{
		m_pAttribute->LinkSibling(pAttribute);
	}
	else
	{
		m_pAttribute = pAttribute;
	}

	return pAttribute;
}

COEXmlNode_Impl* COEXmlNode_Impl::EndSibling(COEXmlNode_Impl* pNode)
{
	if (!pNode) return NULL;

	COEXmlNode_Impl* pEndSibling = pNode;
	while (pEndSibling->m_pNodeSibling)
	{
		pEndSibling = pEndSibling->m_pNodeSibling;
	}
	return pEndSibling;
}
