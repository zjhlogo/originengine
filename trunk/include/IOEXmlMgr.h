/*!
 * \file IOEXmlMgr.h
 * \date 24-7-2009 15:27:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEXMLMGR_H__
#define __IOEXMLMGR_H__

#include "IOEXmlDocument.h"
#include "IOEXmlNode.h"
#include "IOEXmlAttribute.h"

class IOEXmlMgr
{
public:
	IOEXmlMgr() {};
	virtual ~IOEXmlMgr() {};

	virtual IOEXmlDocument* CreateDocument() = 0;
	virtual IOEXmlNode* CreateNode(const tstring& strName) = 0;
	virtual IOEXmlAttribute* CreateAttribute(const tstring& strName) = 0;

};

extern IOEXmlMgr* g_pOEXmlMgr;

#endif // __IOEXMLMGR_H__
