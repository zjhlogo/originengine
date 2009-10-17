/*!
 * \file OEXmlMgr_Impl.h
 * \date 24-7-2009 16:28:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEXMLMGR_IMPL_H__
#define __OEXMLMGR_IMPL_H__

#include <IOEXmlMgr.h>

class COEXmlMgr_Impl : public IOEXmlMgr
{
public:
	COEXmlMgr_Impl();
	virtual ~COEXmlMgr_Impl();

	virtual IOEXmlDocument* CreateDocument();
	virtual IOEXmlNode* CreateNode(const tstring& strName);
	virtual IOEXmlAttribute* CreateAttribute(const tstring& strName);

private:
	void Init();
	void Destroy();

};
#endif // __OEXMLMGR_IMPL_H__