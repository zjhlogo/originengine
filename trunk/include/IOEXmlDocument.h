/*!
 * \file IOEXmlDocument.h
 * \date 11-10-2009 9:18:32
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

	virtual bool LoadFile(const tstring& strFile) = 0;
	virtual bool SaveFile(const tstring& strFile) = 0;

	virtual bool LoadBuffer(const void* pBuffer, uint nSize) = 0;
	virtual bool SaveBuffer() = 0;

	virtual IOEXmlNode* GetDeclNode() = 0;
	virtual IOEXmlNode* GetRootNode() = 0;
};

#endif // __IOEXMLDOCUMENT_H__
