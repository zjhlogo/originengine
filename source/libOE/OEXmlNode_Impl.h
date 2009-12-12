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
#include "OEXmlAttribute_Impl.h"

class COEXmlNode_Impl : public IOEXmlNode
{
public:
	COEXmlNode_Impl(const tstring& strFileName);
	virtual ~COEXmlNode_Impl();

	virtual const tstring& GetName();
	virtual void SetName(const tstring& strName);

	virtual const tstring& GetValue();
	virtual void SetValue(const tstring& strValue);

	virtual bool GetAttribute(bool& bValue, const tstring& strName);
	virtual bool GetAttribute(int& nValue, const tstring& strName);
	virtual bool GetAttribute(float& fValue, const tstring& strName);
	virtual bool GetAttribute(tstring& strValue, const tstring& strName);

	virtual bool SetAttribute(const tstring& strName, bool bValue);
	virtual bool SetAttribute(const tstring& strName, int nValue);
	virtual bool SetAttribute(const tstring& strName, float fValue);
	virtual bool SetAttribute(const tstring& strName, const tstring& strValue);

	virtual IOEXmlAttribute* FirstAttribute();
	virtual IOEXmlAttribute* FirstAttribute(const tstring& strName);

	virtual bool GetText(int& nValue);
	virtual bool GetText(float& fValue);
	virtual bool GetText(tstring& strText);

	virtual bool SetText(int nValue);
	virtual bool SetText(float fValue);
	virtual bool SetText(const tstring& strText);

	virtual IOEXmlNode* FirstChild();
	virtual IOEXmlNode* FirstChild(const tstring& strName);
	virtual IOEXmlNode* EndChild();

	virtual IOEXmlNode* NextSibling();
	virtual IOEXmlNode* NextSibling(const tstring& strName);
	virtual IOEXmlNode* EndSibling();

	virtual IOEXmlNode* InsertChild(const tstring& strName, INSERT_NODE_BEHAVE eBehave = INB_NEXT);
	virtual IOEXmlNode* InsertChild(const IOEXmlNode* pNodeChild, INSERT_NODE_BEHAVE eBehave = INB_NEXT);

	virtual IOEXmlNode* InsertSibling(const tstring& strName, INSERT_NODE_BEHAVE eBehave = INB_NEXT);
	virtual IOEXmlNode* InsertSibling(const IOEXmlNode* pNodeBrother, INSERT_NODE_BEHAVE eBehave = INB_NEXT);

	virtual bool ToString(tstring& strNode, int nLevel = 0);
	virtual bool IsTextNode();

	void LinkAttribute(COEXmlAttribute_Impl* pAttribute);
	void LinkChild(COEXmlNode_Impl* pNodeChild);
	void LinkSibling(COEXmlNode_Impl* pNodeSibling);
	void UnlinkAll();

private:
	void Init();
	void Destroy();

	COEXmlAttribute_Impl* CreateEndAttribute(const tstring& strName);
	COEXmlNode_Impl* EndSibling(COEXmlNode_Impl* pNode);

private:
	tstring m_strName;
	tstring m_strValue;
	COEXmlAttribute_Impl* m_pAttribute;
	COEXmlNode_Impl* m_pNodeChild;
	COEXmlNode_Impl* m_pNodeSibling;

};

#endif // __OEXMLNODE_IMPL_H__
