/*!
 * \file OEXmlDocument_Impl.cpp
 * \date 24-7-2009 17:30:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlDocument_Impl.h"
#include "../../3rdsrc/tinyxml/tinyxml.h"
#include <OEInterfaces.h>
#include <OEOS.h>

COEXmlDocument_Impl::COEXmlDocument_Impl(const tstring& strFileName)
{
	Init();
	m_bOK = Create(strFileName);
}

COEXmlDocument_Impl::~COEXmlDocument_Impl()
{
	Destroy();
}

void COEXmlDocument_Impl::Init()
{
	m_pDocument = NULL;
}

void COEXmlDocument_Impl::Destroy()
{
	SAFE_DELETE(m_pDocument);
}

IOEXmlNode* COEXmlDocument_Impl::FirstChild()
{
	return (IOEXmlNode*)m_pDocument->FirstChildElement();
}

IOEXmlNode* COEXmlDocument_Impl::FirstChild(const tstring& strNodeName)
{
	return (IOEXmlNode*)m_pDocument->FirstChildElement(COEOS::tchar2char_Fast(strNodeName.c_str()));
}

bool COEXmlDocument_Impl::Create(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName);
	if (!pFile) return false;

	uint nSize = pFile->GetSize();
	char* pBuffer = new char[nSize];
	pFile->Read(pBuffer, nSize);
	SAFE_RELEASE(pFile);

	m_pDocument = new TiXmlDocument();
	m_pDocument->Parse((const char*)pBuffer);
	SAFE_DELETE(pBuffer);

	if (m_pDocument->Error())
	{
		SAFE_DELETE(m_pDocument);
		return false;
	}

	return true;
}
