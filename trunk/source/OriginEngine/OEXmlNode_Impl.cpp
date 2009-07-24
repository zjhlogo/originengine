/*!
 * \file OEXmlNode_Impl.cpp
 * \date 24-7-2009 16:22:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlNode_Impl.h"
#include "../../3rdsrc/tinyxml/tinyxml.h"
#include <OEOS.h>

COEXmlNode_Impl::COEXmlNode_Impl()
{
	// TODO: 
}

COEXmlNode_Impl::~COEXmlNode_Impl()
{
	// TODO: 
}

bool COEXmlNode_Impl::GetAttribute(int& nValue, const tstring& strAttrName)
{
	TiXmlElement* pElement = (TiXmlElement*)this;
	if (pElement->Attribute(COEOS::tchar2char_Fast(strAttrName.c_str()), &nValue)) return true;
	return false;
}

bool COEXmlNode_Impl::GetAttribute(float& fValue, const tstring& strAttrName)
{
	TiXmlElement* pElement = (TiXmlElement*)this;

	double fdValue = 0.0f;
	if (pElement->Attribute(COEOS::tchar2char_Fast(strAttrName.c_str()), &fdValue))
	{
		fValue = (float)fdValue;
		return true;
	}
	return false;
}

bool COEXmlNode_Impl::GetAttribute(tstring& strValue, const tstring& strAttrName)
{
	TiXmlElement* pElement = (TiXmlElement*)this;
	const char* pszValue = pElement->Attribute(COEOS::tchar2char_Fast(strAttrName.c_str()));
	if (pszValue)
	{
		COEOS::char2tchar(strValue, pszValue);
		return true;
	}
	return false;
}

bool COEXmlNode_Impl::SetAttribute(const tstring& strAttrName, int nValue)
{
	TiXmlElement* pElement = (TiXmlElement*)this;
	pElement->SetAttribute(COEOS::tchar2char_Fast(strAttrName.c_str()), nValue);
	return true;
}

bool COEXmlNode_Impl::SetAttribute(const tstring& strAttrName, float fValue)
{
	TiXmlElement* pElement = (TiXmlElement*)this;
	pElement->SetDoubleAttribute(COEOS::tchar2char_Fast(strAttrName.c_str()), (double)fValue);
	return true;
}

bool COEXmlNode_Impl::SetAttribute(const tstring& strAttrName, const tstring& strAttrValue)
{
	TiXmlElement* pElement = (TiXmlElement*)this;

	std::string strValue = COEOS::tchar2char_Fast(strAttrValue.c_str());
	pElement->SetAttribute(COEOS::tchar2char_Fast(strAttrName.c_str()), strValue.c_str());
	return true;
}

IOEXmlNode* COEXmlNode_Impl::FirstChild()
{
	TiXmlElement* pElement = (TiXmlElement*)this;
	return (IOEXmlNode*)pElement->FirstChildElement();
}

IOEXmlNode* COEXmlNode_Impl::FirstChild(const tstring& strNodeName)
{
	TiXmlElement* pElement = (TiXmlElement*)this;
	return (IOEXmlNode*)pElement->FirstChildElement(COEOS::tchar2char_Fast(strNodeName.c_str()));
}

IOEXmlNode* COEXmlNode_Impl::NextChild()
{
	TiXmlElement* pElement = (TiXmlElement*)this;
	return (IOEXmlNode*)pElement->NextSiblingElement();
}

IOEXmlNode* COEXmlNode_Impl::NextChild(const tstring& strNodeName)
{
	TiXmlElement* pElement = (TiXmlElement*)this;
	return (IOEXmlNode*)pElement->NextSiblingElement(COEOS::tchar2char_Fast(strNodeName.c_str()));
}
