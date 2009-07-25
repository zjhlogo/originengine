/*!
 * \file IOEXmlNode.h
 * \date 24-7-2009 15:13:29
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEXMLNODE_H__
#define __IOEXMLNODE_H__

#include "IOEObject.h"

class IOEXmlNode : public IOEObject
{
public:
	IOEXmlNode() {};
	virtual ~IOEXmlNode() {};

	virtual bool GetAttribute(int& nValue, const tstring& strAttrName) = 0;
	virtual bool GetAttribute(float& fValue, const tstring& strAttrName) = 0;
	virtual bool GetAttribute(tstring& strValue, const tstring& strAttrName) = 0;

	virtual bool SetAttribute(const tstring& strAttrName, int nValue) = 0;
	virtual bool SetAttribute(const tstring& strAttrName, float fValue) = 0;
	virtual bool SetAttribute(const tstring& strAttrName, const tstring& strAttrValue) = 0;

	virtual bool GetText(tstring& strText) = 0;
	virtual bool SetText(const tstring& strText) = 0;

	virtual IOEXmlNode* FirstChild() = 0;
	virtual IOEXmlNode* FirstChild(const tstring& strNodeName) = 0;

	virtual IOEXmlNode* NextSibling() = 0;
	virtual IOEXmlNode* NextSibling(const tstring& strNodeName) = 0;

};
#endif // __IOEXMLNODE_H__
