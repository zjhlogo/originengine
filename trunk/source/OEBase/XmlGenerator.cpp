/*!
 * \file XmlGenerator.cpp
 * \date 9-10-2009 17:13:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "XmlGenerator.h"
#include "OEXmlNode_Impl.h"

#include <IOEXmlMgr.h>
#include <OEOS.h>

#undef yyFlexLexer
#define yyFlexLexer xmlFlexLexer
#include <FlexLexer.h>


static xmlFlexLexer g_xmlFlexLexer;

CXmlGenerator::TV_STRING CXmlGenerator::m_vString;
CXmlGenerator::TV_XML_ATTRIBUTE CXmlGenerator::m_vAttributes;
CXmlGenerator::TV_XML_NODE CXmlGenerator::m_vNodes;

const std::string CXmlGenerator::m_EmptyString;

COEXmlNode_Impl* CXmlGenerator::m_pNodeDecl = NULL;
COEXmlNode_Impl* CXmlGenerator::m_pNodeRoot = NULL;

const char* CXmlGenerator::m_pBuffer = NULL;
int CXmlGenerator::m_nCurrPos = 0;
int CXmlGenerator::m_nBufferSize = 0;
IOEFile* CXmlGenerator::m_pFile = NULL;

extern int xmlparse();

COEXmlAttribute_Impl* CXmlGenerator::CreateAttribute(int nNameIndex, int nValueIndex)
{
	tstring strTemp;
	COEOS::char2tchar(strTemp, GetString(nNameIndex).c_str());

	COEXmlAttribute_Impl* pAttribute = (COEXmlAttribute_Impl*)g_pOEXmlMgr->CreateAttribute(strTemp);
	if (!pAttribute) return NULL;

	COEOS::char2tchar(strTemp, GetString(nValueIndex).c_str());
	pAttribute->SetValue(strTemp);

	m_vAttributes.push_back(pAttribute);
	return pAttribute;
}

COEXmlNode_Impl* CXmlGenerator::CreateNode(int nNameIndex, int nValueIndex)
{
	tstring strTemp;
	COEOS::char2tchar(strTemp, GetString(nNameIndex).c_str());

	COEXmlNode_Impl* pNode = (COEXmlNode_Impl*)g_pOEXmlMgr->CreateNode(strTemp);
	if (!pNode) return NULL;

	COEOS::char2tchar(strTemp, GetString(nValueIndex).c_str());
	pNode->SetValue(strTemp);

	m_vNodes.push_back(pNode);
	return pNode;
}

void CXmlGenerator::SetDocumentNode(COEXmlNode_Impl* pNodeDecl, COEXmlNode_Impl* pNodeRoot)
{
	m_pNodeDecl = pNodeDecl;
	m_pNodeRoot = pNodeRoot;
}

bool CXmlGenerator::Parse(COEXmlNode_Impl*& pNodeDecl, COEXmlNode_Impl*& pNodeRoot, IOEFile* pFile)
{
	// setup parameter
	SetupParam(pFile);

	// start parse and build nodes
	int nErrorCode = xmlparse();

	// check failed
	if (nErrorCode != 0)
	{
		// release node and clear parameter
		ResetParam(true);
	}
	else
	{
		// get nodes
		pNodeDecl = m_pNodeDecl;
		pNodeRoot = m_pNodeRoot;

		ResetParam(false);
	}

	return (nErrorCode == 0);
}

bool CXmlGenerator::Parse(COEXmlNode_Impl*& pNodeDecl, COEXmlNode_Impl*& pNodeRoot, const void* pBuffer, uint nSize)
{
	// setup parameter
	SetupParam(pBuffer, nSize);

	// start parse and build nodes
	int nErrorCode = xmlparse();

	// check failed
	if (nErrorCode != 0)
	{
		// release node and clear parameter
		ResetParam(true);
	}
	else
	{
		// get nodes
		pNodeDecl = m_pNodeDecl;
		pNodeRoot = m_pNodeRoot;

		ResetParam(false);
	}

	return (nErrorCode == 0);
}

int CXmlGenerator::GetCharType(uchar ch)
{
	static const char s_CharTypeTable[256] = 
	{
	//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 3
		0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 4
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, // 5
		0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 6
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // B
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // D
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // E
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F
	};

	return s_CharTypeTable[ch];
}

const std::string& CXmlGenerator::GetString(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int)m_vString.size()) return m_vString[nIndex];
	return m_EmptyString;
}

int CXmlGenerator::MakeName(const char* pszToken)
{
	int nStringIndex = m_vString.size();
	m_vString.push_back(m_EmptyString);

	int nLength = strlen(pszToken);
	for (int i = 0; i < nLength; ++i)
	{
		if (GetCharType(pszToken[i]) > 0)
		{
			m_vString[nStringIndex].append(1, pszToken[i]);
		}
	}

	return nStringIndex;
}

int CXmlGenerator::MakeValue(const char* pszToken)
{
	int nLength = strlen(pszToken);
	if (nLength < 2) return INVALID_STRING_INDEX;

	int nStringIndex = m_vString.size();
	m_vString.push_back(m_EmptyString);
	m_vString[nStringIndex].append(&pszToken[1], nLength-2);
	return nStringIndex;
}

int CXmlGenerator::MakeText(const char* pszToken)
{
	int nStringIndex = m_vString.size();
	m_vString.push_back(m_EmptyString);
	m_vString[nStringIndex].append(pszToken);
	return nStringIndex;
}

int CXmlGenerator::DumpString(char* pBufferOut, int nDumpSize)
{
	// file source
	if (m_pFile) return m_pFile->Read(pBufferOut, nDumpSize);

	// buffer source
	int nRemainLength = m_nBufferSize - m_nCurrPos;
	int nMinLength = nDumpSize < nRemainLength ? nDumpSize : nRemainLength;

	if (nMinLength > 0)
	{
		memcpy(pBufferOut, &m_pBuffer[m_nCurrPos], nMinLength);
		m_nCurrPos += nMinLength;
	}

	return nMinLength;
}

void CXmlGenerator::SetupParam(const void* pBuffer, uint nSize)
{
	ResetParam(true);

	m_pBuffer = (const char*)pBuffer;
	m_nBufferSize = (int)nSize;
}

void CXmlGenerator::SetupParam(IOEFile* pFile)
{
	ResetParam(true);

	m_pFile = pFile;
}

void CXmlGenerator::ResetParam(bool bFree)
{
	m_pBuffer = NULL;
	m_nBufferSize = 0;
	m_nCurrPos = 0;
	m_pFile = NULL;
	m_vString.clear();
	m_pNodeDecl = NULL;
	m_pNodeRoot = NULL;

	if (bFree)
	{
		for (TV_XML_ATTRIBUTE::iterator it = m_vAttributes.begin(); it != m_vAttributes.end(); ++it)
		{
			IOEXmlAttribute* pAttribute = (*it);
			SAFE_RELEASE(pAttribute);
		}

		for (TV_XML_NODE::iterator it = m_vNodes.begin(); it != m_vNodes.end(); ++it)
		{
			COEXmlNode_Impl* pNode = (COEXmlNode_Impl*)(*it);
			pNode->UnlinkAll();
			SAFE_RELEASE(pNode);
		}
	}

	m_vAttributes.clear();
	m_vNodes.clear();
}

int xmlFlexLexer::yywrap()
{
	return 1;
}

int xmllex()
{
	return g_xmlFlexLexer.yylex();
}

void xmlerror(const char* pszErrorMsg)
{
	// TODO: 
	int nLineNo = g_xmlFlexLexer.lineno();
	const char* pszToken = g_xmlFlexLexer.YYText();
}
