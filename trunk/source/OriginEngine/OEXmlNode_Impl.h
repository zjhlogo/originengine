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
#include "../../3rdsrc/tinyxml/tinyxml.h"

#include <map>

class COEXmlNode_Impl : public IOEXmlNode
{
public:
	typedef std::map<TiXmlElement*, IOEXmlNode*> XMLNODE_MAP;

public:
	COEXmlNode_Impl(const tstring& strFileName);
	COEXmlNode_Impl(TiXmlElement* pTiElement, COEXmlNode_Impl* pParentNode);
	~COEXmlNode_Impl();

	virtual bool GetAttribute(int& nValue, const tstring& strAttrName);
	virtual bool GetAttribute(float& fValue, const tstring& strAttrName);
	virtual bool GetAttribute(tstring& strValue, const tstring& strAttrName);

	virtual bool SetAttribute(const tstring& strAttrName, int nValue);
	virtual bool SetAttribute(const tstring& strAttrName, float fValue);
	virtual bool SetAttribute(const tstring& strAttrName, const tstring& strAttrValue);

	virtual bool GetText(tstring& strText);
	virtual bool SetText(const tstring& strText);

	virtual IOEXmlNode* FirstChild();
	virtual IOEXmlNode* FirstChild(const tstring& strNodeName);

	virtual IOEXmlNode* NextSibling();
	virtual IOEXmlNode* NextSibling(const tstring& strNodeName);

	virtual void Release();

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFileName);
	bool Create(TiXmlElement* pTiElement, COEXmlNode_Impl* pParentNode);

	void CreateChildMap();
	void DestroyChildMap();

	IOEXmlNode* ToXmlNode(TiXmlElement* pTiElement);

private:
	COEXmlNode_Impl* m_pParent;
	TiXmlNode* m_pTiNode;
	XMLNODE_MAP m_ChildMap;

};

#endif // __OEXMLNODE_IMPL_H__
