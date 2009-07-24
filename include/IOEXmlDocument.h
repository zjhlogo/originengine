/*!
 * \file IOEXmlDocument.h
 * \date 24-7-2009 17:25:59
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEXMLDOCUMENT_H__
#define __IOEXMLDOCUMENT_H__

#include "IOEObject.h"
#include "IOEXmlNode.h"

class IOEXmlDocument: public IOEObject
{
public:
	IOEXmlDocument() {};
	virtual ~IOEXmlDocument() {};

	virtual IOEXmlNode* FirstChild() = 0;
	virtual IOEXmlNode* FirstChild(const tstring& strNodeName) = 0;

};

#endif // __IOEXMLDOCUMENT_H__
