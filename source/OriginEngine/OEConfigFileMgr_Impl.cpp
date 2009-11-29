/*!
 * \file OEConfigFileMgr_Impl.cpp
 * \date 26-7-2009 9:04:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEConfigFileMgr_Impl.h"
#include <IOEXmlMgr.h>

#include <IOEXmlMgr.h>

COEConfigFileMgr_Impl::COEConfigFileMgr_Impl()
{
	g_pOEConfigFileMgr = this;
	Init();
	m_bOK = LoadConfigFile(m_strConfigFile);
}

COEConfigFileMgr_Impl::~COEConfigFileMgr_Impl()
{
	Destroy();
	g_pOEConfigFileMgr = NULL;
}

void COEConfigFileMgr_Impl::Init()
{
	m_strConfigFile = TS("Config.xml");
	m_pXmlDocument = NULL;
}

void COEConfigFileMgr_Impl::Destroy()
{
	SAFE_RELEASE(m_pXmlDocument);
}

bool COEConfigFileMgr_Impl::GetValue(int& nValue, const tstring& strParamName, int nDefaultValue /* = 0 */)
{
	IOEXmlNode* pRootNode = m_pXmlDocument->GetRootNode();
	IOEXmlNode* pNode = pRootNode->FirstChild(strParamName);
	if (!pNode)
	{
		nValue = nDefaultValue;
		return false;
	}

	if (!pNode->GetText(nValue))
	{
		nValue = nDefaultValue;
		return false;
	}

	return true;
}

bool COEConfigFileMgr_Impl::GetValue(float& fValue, const tstring& strParamName, float fDefaultValue /* = 0.0f */)
{
	IOEXmlNode* pRootNode = m_pXmlDocument->GetRootNode();
	IOEXmlNode* pNode = pRootNode->FirstChild(strParamName);
	if (!pNode)
	{
		fValue = fDefaultValue;
		return false;
	}

	if (!pNode->GetText(fValue))
	{
		fValue = fDefaultValue;
		return false;
	}

	return true;
}

bool COEConfigFileMgr_Impl::GetValue(tstring& strValue, const tstring& strParamName, const tstring& strDefaultValue /* = EMPTY_STRING */)
{
	IOEXmlNode* pRootNode = m_pXmlDocument->GetRootNode();
	IOEXmlNode* pNode = pRootNode->FirstChild(strParamName);
	if (!pNode)
	{
		strValue = strDefaultValue;
		return false;
	}

	if (!pNode->GetText(strValue))
	{
		strValue = strDefaultValue;
		return false;
	}

	return true;
}

bool COEConfigFileMgr_Impl::LoadConfigFile(const tstring& strFileName)
{
	if (!m_pXmlDocument)
	{
		m_pXmlDocument = g_pOEXmlMgr->CreateDocument(strFileName);
		if (!m_pXmlDocument) return false;
	}

	return true;
}
