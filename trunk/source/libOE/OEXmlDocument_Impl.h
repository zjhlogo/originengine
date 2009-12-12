/*!
 * \file OEXmlDocument_Impl.h
 * \date 11-10-2009 9:36:57
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEXMLDOCUMENT_IMPL_H__
#define __OEXMLDOCUMENT_IMPL_H__

#include <IOEXmlDocument.h>
#include "OEXmlNode_Impl.h"

class COEXmlDocument_Impl : public IOEXmlDocument
{
public:
	COEXmlDocument_Impl();
	virtual ~COEXmlDocument_Impl();

	virtual bool LoadFile(const tstring& strFile);
	virtual bool SaveFile(const tstring& strFile);

	virtual bool LoadBuffer(const void* pBuffer, uint nSize);
	virtual bool SaveBuffer(tstring& strBuffer);

	virtual IOEXmlNode* GetDeclNode();
	virtual IOEXmlNode* GetRootNode();

	virtual IOEXmlNode* InsertRootNode(const tstring& strName);

private:
	void Init();
	void Destroy();

private:
	COEXmlNode_Impl* m_pNodeDecl;
	COEXmlNode_Impl* m_pNodeRoot;

};
#endif // __OEXMLDOCUMENT_IMPL_H__
