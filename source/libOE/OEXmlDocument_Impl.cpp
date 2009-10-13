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

COEXmlDocument_Impl::COEXmlDocument_Impl()
{
	Init();
	m_bOK = true;
}

COEXmlDocument_Impl::~COEXmlDocument_Impl()
{
	Destroy();
}

void COEXmlDocument_Impl::Init()
{
	m_pNodeDecl = NULL;
	m_pNodeRoot = NULL;
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
	// TODO: 
	return false;
}

bool COEXmlDocument_Impl::LoadBuffer(const void* pBuffer, uint nSize)
{
	Destroy();
	return CXmlGenerator::Parse(m_pNodeDecl, m_pNodeRoot, pBuffer, nSize);
}

bool COEXmlDocument_Impl::SaveBuffer()
{
	// TODO: 
	return false;
}

IOEXmlNode* COEXmlDocument_Impl::GetDeclNode()
{
	return m_pNodeDecl;
}

IOEXmlNode* COEXmlDocument_Impl::GetRootNode()
{
	return m_pNodeRoot;
}
