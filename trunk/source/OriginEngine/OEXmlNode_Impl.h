/*!
 * \file OEXmlNode_Impl.h
 * \date 24-7-2009 16:21:53
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEXMLNODE_IMPL_H__
#define __OEXMLNODE_IMPL_H__

#include <IOEXmlNode.h>

class COEXmlNode_Impl : public IOEXmlNode
{
public:
	COEXmlNode_Impl();
	~COEXmlNode_Impl();

	virtual bool GetAttribute(int& nValue, const tstring& strAttrName);
	virtual bool GetAttribute(float& fValue, const tstring& strAttrName);
	virtual bool GetAttribute(tstring& strValue, const tstring& strAttrName);

	virtual bool SetAttribute(const tstring& strAttrName, int nValue);
	virtual bool SetAttribute(const tstring& strAttrName, float fValue);
	virtual bool SetAttribute(const tstring& strAttrName, const tstring& strAttrValue);

	virtual IOEXmlNode* FirstChild();
	virtual IOEXmlNode* FirstChild(const tstring& strNodeName);

	virtual IOEXmlNode* NextChild();
	virtual IOEXmlNode* NextChild(const tstring& strNodeName);

};

#endif // __OEXMLNODE_IMPL_H__
