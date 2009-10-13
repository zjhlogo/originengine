/*!
 * \file OEXmlMgr_Impl.cpp
 * \date 24-7-2009 16:28:42
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlMgr_Impl.h"
#include "OEXmlAttribute_Impl.h"
#include "OEXmlNode_Impl.h"
#include "OEXmlDocument_Impl.h"

#include <IOELogFileMgr.h>

COEXmlMgr_Impl::COEXmlMgr_Impl()
{
	g_pOEXmlMgr = this;
	Init();
	m_bOK = true;
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

IOEXmlDocument* COEXmlMgr_Impl::CreateDocument()
{
	COEXmlDocument_Impl* pXmlDocument = new COEXmlDocument_Impl();
	if (!pXmlDocument || !pXmlDocument->IsOK())
	{
		SAFE_RELEASE(pXmlDocument);
		LOGOUT(t("IOEXmlMgr::CreateDocument Failed"));
		return NULL;
	}

	return pXmlDocument;
}

IOEXmlNode* COEXmlMgr_Impl::CreateNode(const tstring& strName)
{
	COEXmlNode_Impl *pXmlNode = new COEXmlNode_Impl(strName);
	if (!pXmlNode || !pXmlNode->IsOK())
	{
		SAFE_RELEASE(pXmlNode);
		LOGOUT(t("IOEXmlMgr::CreateNode Failed \"%s\""), strName.c_str());
		return NULL;
	}

	return pXmlNode;
}

IOEXmlAttribute* COEXmlMgr_Impl::CreateAttribute(const tstring& strName)
{
	COEXmlAttribute_Impl *pXmlAttribute = new COEXmlAttribute_Impl(strName);
	if (!pXmlAttribute || !pXmlAttribute->IsOK())
	{
		SAFE_RELEASE(pXmlAttribute);
		LOGOUT(t("IOEXmlMgr::CreateAttribute Failed \"%s\""), strName.c_str());
		return NULL;
	}

	return pXmlAttribute;
}
