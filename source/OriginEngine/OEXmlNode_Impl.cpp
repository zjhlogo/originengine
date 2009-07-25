/*!
 * \file OEXmlNode_Impl.cpp
 * \date 24-7-2009 16:22:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlNode_Impl.h"
#include <OEOS.h>

COEXmlNode_Impl::COEXmlNode_Impl(TiXmlElement* pElement)
{
	Init();
	m_bOK = Create(pElement);
}

COEXmlNode_Impl::~COEXmlNode_Impl()
{
	Destroy();
}

void COEXmlNode_Impl::Init()
{
	m_pElement = NULL;
	m_pPrevNode = NULL;
	m_pNextNode = NULL;
}

void COEXmlNode_Impl::Destroy()
{
	DestroyChildMap();
}

bool COEXmlNode_Impl::GetAttribute(int& nValue, const tstring& strAttrName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	if (m_pElement->Attribute(strANSIName.c_str(), &nValue)) return true;
	return false;
}

bool COEXmlNode_Impl::GetAttribute(float& fValue, const tstring& strAttrName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	double fdValue = 0.0f;
	if (m_pElement->Attribute(strANSIName.c_str(), &fdValue))
	{
		fValue = (float)fdValue;
		return true;
	}
	return false;
}

bool COEXmlNode_Impl::GetAttribute(tstring& strValue, const tstring& strAttrName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	const char* pszValue = m_pElement->Attribute(strANSIName.c_str());
	if (pszValue)
	{
		COEOS::char2tchar(strValue, pszValue);
		return true;
	}
	return false;
}

bool COEXmlNode_Impl::SetAttribute(const tstring& strAttrName, int nValue)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	m_pElement->SetAttribute(strANSIName.c_str(), nValue);
	return true;
}

bool COEXmlNode_Impl::SetAttribute(const tstring& strAttrName, float fValue)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	m_pElement->SetDoubleAttribute(strANSIName.c_str(), (double)fValue);
	return true;
}

bool COEXmlNode_Impl::SetAttribute(const tstring& strAttrName, const tstring& strAttrValue)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	std::string strANSIValue;
	if (!COEOS::tchar2char(strANSIValue, strAttrValue.c_str())) return false;

	m_pElement->SetAttribute(strANSIName.c_str(), strANSIValue.c_str());
	return true;
}

IOEXmlNode* COEXmlNode_Impl::FirstChild()
{
	return ToXmlNode(m_pElement->FirstChildElement());
}

IOEXmlNode* COEXmlNode_Impl::FirstChild(const tstring& strNodeName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strNodeName.c_str())) return NULL;

	return ToXmlNode(m_pElement->FirstChildElement(strANSIName.c_str()));
}

IOEXmlNode* COEXmlNode_Impl::NextChild()
{
	return ToXmlNode(m_pElement->NextSiblingElement());
}

IOEXmlNode* COEXmlNode_Impl::NextChild(const tstring& strNodeName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strNodeName.c_str())) return NULL;

	return ToXmlNode(m_pElement->NextSiblingElement(strANSIName.c_str()));
}

COEXmlNode_Impl* COEXmlNode_Impl::GetPrevNode() const
{
	return m_pPrevNode;
}

void COEXmlNode_Impl::SetPrevNode(COEXmlNode_Impl* pNode)
{
	m_pPrevNode = pNode;
}

COEXmlNode_Impl* COEXmlNode_Impl::GetNextNode() const
{
	return m_pNextNode;
}

void COEXmlNode_Impl::SetNextNode(COEXmlNode_Impl* pNode)
{
	m_pNextNode = pNode;
}

bool COEXmlNode_Impl::Create(TiXmlElement* pElement)
{
	m_pElement = pElement;
	CreateChildMap();
	return true;
}

void COEXmlNode_Impl::CreateChildMap()
{
	DestroyChildMap();

	COEXmlNode_Impl* pPrevNode = NULL;
	TiXmlElement* pElementChild = m_pElement->FirstChildElement();
	while (pElementChild)
	{
		COEXmlNode_Impl* pNewNode = new COEXmlNode_Impl(pElementChild);
		pNewNode->SetPrevNode(pPrevNode);
		if (pPrevNode) pPrevNode->SetNextNode(pNewNode);

		m_ChildMap.insert(std::make_pair(pElementChild, pNewNode));

		pPrevNode = pNewNode;
		pElementChild = pElementChild->NextSiblingElement();
	}
}

void COEXmlNode_Impl::DestroyChildMap()
{
	for (XMLNODE_MAP::iterator it = m_ChildMap.begin(); it != m_ChildMap.end(); ++it)
	{
		IOEXmlNode* pXmlNode = it->second;
		SAFE_RELEASE(pXmlNode);
	}
	m_ChildMap.clear();
}

IOEXmlNode* COEXmlNode_Impl::ToXmlNode(TiXmlElement* pElement)
{
	COEXmlNode_Impl::XMLNODE_MAP::iterator itfound = m_ChildMap.find(pElement);
	if (itfound != m_ChildMap.end()) return itfound->second;

	return NULL;
}
