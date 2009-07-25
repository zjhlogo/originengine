/*!
 * \file OEXmlDocument_Impl.h
 * \date 24-7-2009 17:28:04
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEXMLDOCUMENT_IMPL_H__
#define __OEXMLDOCUMENT_IMPL_H__

#include <IOEXmlDocument.h>
#include "OEXmlNode_Impl.h"
#include "../../3rdsrc/tinyxml/tinyxml.h"

class COEXmlDocument_Impl : public IOEXmlDocument
{
public:
	COEXmlDocument_Impl(const tstring& strFileName);
	virtual ~COEXmlDocument_Impl();

	virtual IOEXmlNode* FirstChild();
	virtual IOEXmlNode* FirstChild(const tstring& strNodeName);

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFileName);
	void CreateChildMap();
	void DestroyChildMap();

	IOEXmlNode* ToXmlNode(TiXmlElement* pElement);

private:
	TiXmlDocument* m_pDocument;
	COEXmlNode_Impl::XMLNODE_MAP m_ChildMap;

};
#endif // __OEXMLDOCUMENT_IMPL_H__
