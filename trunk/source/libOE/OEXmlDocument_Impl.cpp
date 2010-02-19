/*!
 * \file OEXmlDocument_Impl.cpp
 * \date 11-10-2009 9:39:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlDocument_Impl.h"
#include "XmlGenerator.h"

#include <IOEFileMgr.h>
#include <IOEXmlMgr.h>
#include <OEOS.h>

COEXmlDocument_Impl::COEXmlDocument_Impl()
{
	m_bOK = Init();
}

COEXmlDocument_Impl::~COEXmlDocument_Impl()
{
	Destroy();
}

bool COEXmlDocument_Impl::Init()
{
	m_pNodeDecl = NULL;
	m_pNodeRoot = NULL;
	return true;
}

void COEXmlDocument_Impl::Destroy()
{
	SAFE_RELEASE(m_pNodeDecl);
	SAFE_RELEASE(m_pNodeRoot);
}

bool COEXmlDocument_Impl::LoadFile(const tstring& strFile)
{
	Destroy();

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile);
	if (!pFile) return false;

	bool bOK = CXmlGenerator::Parse(m_pNodeDecl, m_pNodeRoot, pFile);

	SAFE_RELEASE(pFile);
	return bOK;
}

bool COEXmlDocument_Impl::SaveFile(const tstring& strFile)
{
	tstring strDoc;
	if (!SaveBuffer(strDoc)) return false;

	std::string strANSI;
	if (!COEOS::tchar2char(strANSI, strDoc.c_str())) return false;

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	pFile->Write(strANSI.c_str(), strANSI.length());
	SAFE_RELEASE(pFile);

	return true;
}

bool COEXmlDocument_Impl::LoadBuffer(const void* pBuffer, uint nSize)
{
	Destroy();
	return CXmlGenerator::Parse(m_pNodeDecl, m_pNodeRoot, pBuffer, nSize);
}

bool COEXmlDocument_Impl::SaveBuffer(tstring& strBuffer)
{
	if (!m_pNodeRoot) return false;

	strBuffer += TS("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	return m_pNodeRoot->ToString(strBuffer, 0);
}

IOEXmlNode* COEXmlDocument_Impl::GetDeclNode()
{
	return m_pNodeDecl;
}

IOEXmlNode* COEXmlDocument_Impl::GetRootNode()
{
	return m_pNodeRoot;
}

IOEXmlNode* COEXmlDocument_Impl::InsertRootNode(const tstring& strName)
{
	if (m_pNodeRoot) return NULL;

	m_pNodeRoot = (COEXmlNode_Impl*)g_pOEXmlMgr->CreateNode(strName);
	if (!m_pNodeRoot) return NULL;

	return m_pNodeRoot;
}
