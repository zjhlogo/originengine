/*!
 * \file IOEXmlMgr.h
 * \date 24-7-2009 15:27:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEXMLMGR_H__
#define __IOEXMLMGR_H__

#include "IOEXmlNode.h"

class IOEXmlMgr
{
public:
	IOEXmlMgr() {};
	virtual ~IOEXmlMgr() {};

	virtual IOEXmlNode* OpenXmlFile(const tstring& strFileName) = 0;

};

extern IOEXmlMgr* g_pOEXmlMgr;

#endif // __IOEXMLMGR_H__
