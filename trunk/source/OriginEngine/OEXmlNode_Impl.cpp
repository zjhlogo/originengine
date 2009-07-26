/*!
 * \file OEXmlNode_Impl.cpp
 * \date 24-7-2009 16:22:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlNode_Impl.h"
#include <OEOS.h>
#include <OEInterfaces.h>

COEXmlNode_Impl::COEXmlNode_Impl(const tstring& strFileName)
{
	Init();
	m_bOK = Create(strFileName);
}

COEXmlNode_Impl::COEXmlNode_Impl(TiXmlElement* pTiElement, COEXmlNode_Impl* pParentNode)
{
	Init();
	m_bOK = Create(pTiElement, pParentNode);
}

COEXmlNode_Impl::~COEXmlNode_Impl()
{
	Destroy();
}

void COEXmlNode_Impl::Init()
{
	m_pParent = NULL;
	m_pTiNode = NULL;
}

void COEXmlNode_Impl::Destroy()
{
	DestroyChildMap();
	if (m_pTiNode->Type() == TiXmlNode::DOCUMENT)
	{
		SAFE_DELETE(m_pTiNode);
	}
}

bool COEXmlNode_Impl::GetAttribute(int& nValue, const tstring& strAttrName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	if (pTiElement->Attribute(strANSIName.c_str(), &nValue)) return true;
	return false;
}

bool COEXmlNode_Impl::GetAttribute(float& fValue, const tstring& strAttrName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	double fdValue = 0.0f;
	if (pTiElement->Attribute(strANSIName.c_str(), &fdValue))
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

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	const char* pszValue = pTiElement->Attribute(strANSIName.c_str());
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

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	pTiElement->SetAttribute(strANSIName.c_str(), nValue);
	return true;
}

bool COEXmlNode_Impl::SetAttribute(const tstring& strAttrName, float fValue)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	pTiElement->SetDoubleAttribute(strANSIName.c_str(), (double)fValue);
	return true;
}

bool COEXmlNode_Impl::SetAttribute(const tstring& strAttrName, const tstring& strAttrValue)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strAttrName.c_str())) return false;

	std::string strANSIValue;
	if (!COEOS::tchar2char(strANSIValue, strAttrValue.c_str())) return false;

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	pTiElement->SetAttribute(strANSIName.c_str(), strANSIValue.c_str());
	return true;
}

bool COEXmlNode_Impl::GetText(int& nValue)
{
	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	const char* pszText = pTiElement->GetText();
	if (!pszText) return false;

	tstring strText;
	if (!COEOS::char2tchar(strText, pszText)) return false;

	return COEOS::str2int(nValue, strText.c_str());
}

bool COEXmlNode_Impl::GetText(float& fValue)
{
	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	const char* pszText = pTiElement->GetText();
	if (!pszText) return false;

	tstring strText;
	if (!COEOS::char2tchar(strText, pszText)) return false;

	return COEOS::str2float(fValue, strText.c_str());
}

bool COEXmlNode_Impl::GetText(tstring& strText)
{
	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	const char* pszText = pTiElement->GetText();
	if (!pszText) return false;

	return COEOS::char2tchar(strText, pszText);
}

bool COEXmlNode_Impl::SetText(int nValue)
{
	tstring strValue;
	if (!COEOS::int2str(strValue, nValue)) return false;

	std::string strANSIText;
	if (!COEOS::tchar2char(strANSIText, strValue.c_str())) return false;

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	TiXmlNode* pNode = pTiElement->FirstChild();
	if (!pNode)
	{
		TiXmlText* pNewText = new TiXmlText(strANSIText.c_str());
		pTiElement->LinkEndChild(pNewText);
		CreateChildMap();
		return true;
	}

	pNode->SetValue(strANSIText.c_str());
	return true;
}

bool COEXmlNode_Impl::SetText(float fValue)
{
	tstring strValue;
	if (!COEOS::float2str(strValue, fValue)) return false;

	std::string strANSIText;
	if (!COEOS::tchar2char(strANSIText, strValue.c_str())) return false;

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	TiXmlNode* pNode = pTiElement->FirstChild();
	if (!pNode)
	{
		TiXmlText* pNewText = new TiXmlText(strANSIText.c_str());
		pTiElement->LinkEndChild(pNewText);
		CreateChildMap();
		return true;
	}

	pNode->SetValue(strANSIText.c_str());
	return true;
}

bool COEXmlNode_Impl::SetText(const tstring& strText)
{
	std::string strANSIText;
	if (!COEOS::tchar2char(strANSIText, strText.c_str())) return false;

	TiXmlElement* pTiElement = m_pTiNode->ToElement();
	if (!pTiElement) return false;

	TiXmlNode* pNode = pTiElement->FirstChild();
	if (!pNode)
	{
		TiXmlText* pNewText = new TiXmlText(strANSIText.c_str());
		pTiElement->LinkEndChild(pNewText);
		CreateChildMap();
		return true;
	}

	pNode->SetValue(strANSIText.c_str());
	return true;
}

IOEXmlNode* COEXmlNode_Impl::FirstChild()
{
	return ToXmlNode(m_pTiNode->FirstChildElement());
}

IOEXmlNode* COEXmlNode_Impl::FirstChild(const tstring& strNodeName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strNodeName.c_str())) return NULL;

	return ToXmlNode(m_pTiNode->FirstChildElement(strANSIName.c_str()));
}

IOEXmlNode* COEXmlNode_Impl::NextSibling()
{
	if (!m_pParent) return NULL;

	TiXmlElement* pTiElement = m_pTiNode->NextSiblingElement();
	if (!pTiElement) return NULL;

	return m_pParent->ToXmlNode(pTiElement);
}

IOEXmlNode* COEXmlNode_Impl::NextSibling(const tstring& strNodeName)
{
	if (!m_pParent) return NULL;

	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strNodeName.c_str())) return NULL;

	TiXmlElement* pTiElement = m_pTiNode->NextSiblingElement(strANSIName.c_str());
	if (!pTiElement) return NULL;

	return m_pParent->ToXmlNode(pTiElement);
}

void COEXmlNode_Impl::Release()
{
	assert(m_pTiNode->Type() == TiXmlNode::DOCUMENT);
	if (m_pTiNode->Type() == TiXmlNode::DOCUMENT)
	{
		IOEXmlNode::Release();
	}
}

bool COEXmlNode_Impl::Create(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName);
	if (!pFile) return false;

	uint nSize = pFile->GetSize();
	char* pBuffer = new char[nSize];
	pFile->Read(pBuffer, nSize);
	SAFE_RELEASE(pFile);

	TiXmlDocument* pDocument = new TiXmlDocument();
	pDocument->Parse((const char*)pBuffer);
	SAFE_DELETE(pBuffer);

	if (pDocument->Error())
	{
		SAFE_DELETE(pDocument);
		return false;
	}

	m_pTiNode = pDocument;
	CreateChildMap();
	return true;
}

bool COEXmlNode_Impl::Create(TiXmlElement* pTiElement, COEXmlNode_Impl* pParentNode)
{
	m_pTiNode = pTiElement;
	m_pParent = pParentNode;
	CreateChildMap();
	return true;
}

void COEXmlNode_Impl::CreateChildMap()
{
	DestroyChildMap();

	TiXmlElement* pTiElementChild = m_pTiNode->FirstChildElement();
	while (pTiElementChild)
	{
		COEXmlNode_Impl* pNewNode = new COEXmlNode_Impl(pTiElementChild, this);
		m_ChildMap.insert(std::make_pair(pTiElementChild, pNewNode));
		pTiElementChild = pTiElementChild->NextSiblingElement();
	}
}

void COEXmlNode_Impl::DestroyChildMap()
{
	for (XMLNODE_MAP::iterator it = m_ChildMap.begin(); it != m_ChildMap.end(); ++it)
	{
		IOEXmlNode* pXmlNode = it->second;
		SAFE_DELETE(pXmlNode);
	}
	m_ChildMap.clear();
}

IOEXmlNode* COEXmlNode_Impl::ToXmlNode(TiXmlElement* pTiElement)
{
	COEXmlNode_Impl::XMLNODE_MAP::iterator itfound = m_ChildMap.find(pTiElement);
	if (itfound != m_ChildMap.end()) return itfound->second;

	return NULL;
}
