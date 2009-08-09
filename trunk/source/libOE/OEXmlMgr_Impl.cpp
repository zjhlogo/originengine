/*!
 * \file OEXmlMgr_Impl.cpp
 * \date 24-7-2009 16:28:42
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlMgr_Impl.h"
#include "OEXmlNode_Impl.h"
#include <OEInterfaces.h>

COEXmlMgr_Impl::COEXmlMgr_Impl()
{
	g_pOEXmlMgr = this;
	Init();
}

COEXmlMgr_Impl::~COEXmlMgr_Impl()
{
	Destroy();
	g_pOEXmlMgr = NULL;
}

void COEXmlMgr_Impl::Init()
{
	// TODO: 
}

void COEXmlMgr_Impl::Destroy()
{
	// TODO: 
}

IOEXmlNode* COEXmlMgr_Impl::OpenXmlFile(const tstring& strFileName)
{
	IOEXmlNode *pRootNode = new COEXmlNode_Impl(strFileName);
	if (!pRootNode || !pRootNode->IsOK())
	{
		SAFE_RELEASE(pRootNode);
		LOGOUT(_T("IOEXmlMgr::OpenXmlFile Failed \"%s\""), strFileName.c_str());
		return NULL;
	}

	return pRootNode;
}
