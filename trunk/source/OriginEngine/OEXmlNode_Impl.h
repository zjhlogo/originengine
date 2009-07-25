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
	COEXmlNode_Impl(TiXmlElement* pElement);
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

	COEXmlNode_Impl* GetPrevNode() const;
	void SetPrevNode(COEXmlNode_Impl* pNode);

	COEXmlNode_Impl* GetNextNode() const;
	void SetNextNode(COEXmlNode_Impl* pNode);

private:
	void Init();
	void Destroy();

	bool Create(TiXmlElement* pElement);
	void CreateChildMap();
	void DestroyChildMap();

	IOEXmlNode* ToXmlNode(TiXmlElement* pElement);

private:
	TiXmlElement* m_pElement;
	COEXmlNode_Impl* m_pPrevNode;
	COEXmlNode_Impl* m_pNextNode;
	XMLNODE_MAP m_ChildMap;

};

#endif // __OEXMLNODE_IMPL_H__
