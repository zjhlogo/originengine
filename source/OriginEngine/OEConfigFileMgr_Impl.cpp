/*!
 * \file OEConfigFileMgr_Impl.cpp
 * \date 26-7-2009 9:04:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEConfigFileMgr_Impl.h"

COEConfigFileMgr_Impl::COEConfigFileMgr_Impl()
{
	g_pOEConfigFileMgr = this;
	Init();
	LoadConfigFile(m_strConfigFile);
}

COEConfigFileMgr_Impl::~COEConfigFileMgr_Impl()
{
	Destroy();
	g_pOEConfigFileMgr = NULL;
}

void COEConfigFileMgr_Impl::Init()
{
	m_strConfigFile = _T("Config.xml");
	m_pDocument = NULL;
	m_pRootNode = NULL;
}

void COEConfigFileMgr_Impl::Destroy()
{
	SAFE_RELEASE(m_pDocument);
	m_pRootNode = NULL;
}

bool COEConfigFileMgr_Impl::GetValue(int& nValue, const tstring& strParamName, int nDefaultValue /* = 0 */)
{
	IOEXmlNode* pNode = m_pRootNode->FirstChild(strParamName);
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
	IOEXmlNode* pNode = m_pRootNode->FirstChild(strParamName);
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
	IOEXmlNode* pNode = m_pRootNode->FirstChild(strParamName);
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
	SAFE_RELEASE(m_pDocument);
	m_pRootNode = NULL;

	m_pDocument = g_pOEXmlMgr->OpenXmlFile(strFileName);
	if (!m_pDocument) return false;

	m_pRootNode = m_pDocument->FirstChild();
	return true;
}
