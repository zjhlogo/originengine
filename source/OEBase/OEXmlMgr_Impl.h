/*!
 * \file OEXmlMgr_Impl.h
 * \date 24-7-2009 16:28:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEXMLMGR_IMPL_H__
#define __OEXMLMGR_IMPL_H__

#include <OEBase/IOEXmlMgr.h>

class COEXmlMgr_Impl : public IOEXmlMgr
{
public:
	RTTI_DEF(COEXmlMgr_Impl, IOEXmlMgr);

	COEXmlMgr_Impl();
	virtual ~COEXmlMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual IOEXmlDocument* CreateDocument();
	virtual IOEXmlDocument* CreateDocument(const tstring& strFile);

	virtual IOEXmlNode* CreateNode(const tstring& strName);
	virtual IOEXmlAttribute* CreateAttribute(const tstring& strName);

private:
	bool Init();
	void Destroy();

};
#endif // __OEXMLMGR_IMPL_H__
