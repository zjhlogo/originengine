/*!
 * \file OEXmlMgr_Impl.cpp
 * \date 24-7-2009 16:28:42
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlMgr_Impl.h"
#include "OEXmlDocument_Impl.h"

COEXmlMgr_Impl::COEXmlMgr_Impl()
{
	// TODO: 
}

COEXmlMgr_Impl::~COEXmlMgr_Impl()
{
	// TODO: 
}

IOEXmlDocument* COEXmlMgr_Impl::OpenXmlFile(const tstring& strFileName)
{
	IOEXmlDocument *pDocument = new COEXmlDocument_Impl(strFileName);
	if (!pDocument || !pDocument->IsOK())
	{
		SAFE_RELEASE(pDocument);
		// TODO: logout
		return NULL;
	}

	return pDocument;
}
