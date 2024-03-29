/*!
 * \file IOEXmlMgr.h
 * \date 24-7-2009 15:27:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEXMLMGR_H__
#define __IOEXMLMGR_H__

#include "../OECore/IOEMgr.h"
#include "IOEXmlDocument.h"
#include "IOEXmlNode.h"
#include "IOEXmlAttribute.h"

class IOEXmlMgr : public IOEMgr
{
public:
	RTTI_DEF(IOEXmlMgr, IOEMgr);

	IOEXmlMgr() {};
	virtual ~IOEXmlMgr() {};

	virtual IOEXmlDocument* CreateDocument() = 0;
	virtual IOEXmlDocument* CreateDocument(const tstring& strFile) = 0;
	virtual IOEXmlNode* CreateNode(const tstring& strName) = 0;
	virtual IOEXmlAttribute* CreateAttribute(const tstring& strName) = 0;

};

extern IOEXmlMgr* g_pOEXmlMgr;

#endif // __IOEXMLMGR_H__
