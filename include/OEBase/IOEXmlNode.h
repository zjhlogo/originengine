/*!
 * \file IOEXmlNode.h
 * \date 24-7-2009 15:13:29
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEXMLNODE_H__
#define __IOEXMLNODE_H__

#include "../libOEBase/IOEObject.h"
#include "IOEXmlAttribute.h"

class IOEXmlNode : public IOEObject
{
public:
	enum INSERT_NODE_BEHAVE
	{
		INB_PREVIOUS,
		INB_NEXT,
	};

public:
	RTTI_DEF(IOEXmlNode, IOEObject);

	IOEXmlNode() {};
	virtual ~IOEXmlNode() {};

	virtual const tstring& GetName() = 0;
	virtual void SetName(const tstring& strName) = 0;

	virtual const tstring& GetValue() = 0;
	virtual void SetValue(const tstring& strValue) = 0;

	virtual bool GetAttribute(bool& bValue, const tstring& strName) = 0;
	virtual bool GetAttribute(int& nValue, const tstring& strName) = 0;
	virtual bool GetAttribute(float& fValue, const tstring& strName) = 0;
	virtual bool GetAttribute(tstring& strValue, const tstring& strName) = 0;

	virtual bool SetAttribute(const tstring& strName, bool bValue) = 0;
	virtual bool SetAttribute(const tstring& strName, int nValue) = 0;
	virtual bool SetAttribute(const tstring& strName, float fValue) = 0;
	virtual bool SetAttribute(const tstring& strName, const tstring& strValue) = 0;

	virtual IOEXmlAttribute* FirstAttribute() = 0;
	virtual IOEXmlAttribute* FirstAttribute(const tstring& strName) = 0;

	virtual bool GetText(int& nValue) = 0;
	virtual bool GetText(float& fValue) = 0;
	virtual bool GetText(tstring& strText) = 0;

	virtual bool SetText(int nValue) = 0;
	virtual bool SetText(float fValue) = 0;
	virtual bool SetText(const tstring& strText) = 0;

	virtual IOEXmlNode* FirstChild() = 0;
	virtual IOEXmlNode* FirstChild(const tstring& strName) = 0;
	virtual IOEXmlNode* EndChild() = 0;

	virtual IOEXmlNode* NextSibling() = 0;
	virtual IOEXmlNode* NextSibling(const tstring& strName) = 0;
	virtual IOEXmlNode* EndSibling() = 0;

	virtual IOEXmlNode* InsertChild(const tstring& strName, INSERT_NODE_BEHAVE eBehave = INB_NEXT) = 0;
	virtual IOEXmlNode* InsertChild(const IOEXmlNode* pNodeChild, INSERT_NODE_BEHAVE eBehave = INB_NEXT) = 0;

	virtual IOEXmlNode* InsertSibling(const tstring& strName, INSERT_NODE_BEHAVE eBehave = INB_NEXT) = 0;
	virtual IOEXmlNode* InsertSibling(const IOEXmlNode* pNodeBrother, INSERT_NODE_BEHAVE eBehave = INB_NEXT) = 0;

	virtual bool ToString(tstring& strNode, int nLevel = 0) = 0;
	virtual bool IsTextNode() = 0;

};
#endif // __IOEXMLNODE_H__
