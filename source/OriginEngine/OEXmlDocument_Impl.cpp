/*!
 * \file OEXmlDocument_Impl.cpp
 * \date 24-7-2009 17:30:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEXmlDocument_Impl.h"
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
	DestroyChildMap();
	SAFE_DELETE(m_pDocument);
}

IOEXmlNode* COEXmlDocument_Impl::FirstChild()
{
	return ToXmlNode(m_pDocument->FirstChildElement());
}

IOEXmlNode* COEXmlDocument_Impl::FirstChild(const tstring& strNodeName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strNodeName.c_str())) return NULL;

	return ToXmlNode(m_pDocument->FirstChildElement(strANSIName.c_str()));
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

	CreateChildMap();

	return true;
}

void COEXmlDocument_Impl::CreateChildMap()
{
	DestroyChildMap();

	COEXmlNode_Impl* pPrevNode = NULL;
	TiXmlElement* pElementChild = m_pDocument->FirstChildElement();
	while (pElementChild)
	{
		COEXmlNode_Impl* pNewNode = new COEXmlNode_Impl(pElementChild);
		pNewNode->SetPrevNode(pPrevNode);
		if (pPrevNode) pPrevNode->SetNextNode(pNewNode);

		m_ChildMap.insert(std::make_pair(pElementChild, pNewNode));

		pPrevNode = pNewNode;
		pElementChild = pElementChild->NextSiblingElement();
	}
}

void COEXmlDocument_Impl::DestroyChildMap()
{
	for (COEXmlNode_Impl::XMLNODE_MAP::iterator it = m_ChildMap.begin(); it != m_ChildMap.end(); ++it)
	{
		 IOEXmlNode* pXmlNode = it->second;
		 SAFE_RELEASE(pXmlNode);
	}
	m_ChildMap.clear();
}

IOEXmlNode* COEXmlDocument_Impl::ToXmlNode(TiXmlElement* pElement)
{
	COEXmlNode_Impl::XMLNODE_MAP::iterator itfound = m_ChildMap.find(pElement);
	if (itfound != m_ChildMap.end()) return itfound->second;

	return NULL;
}
