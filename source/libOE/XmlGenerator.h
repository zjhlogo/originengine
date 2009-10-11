/*!
 * \file XmlGenerator.h
 * \date 9-10-2009 17:13:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __XMLGENERATOR_H__
#define __XMLGENERATOR_H__

#include "OEXmlAttribute_Impl.h"
#include "OEXmlNode_Impl.h"

#include <OEInterfaces.h>
#include <vector>
#include <string>

class CXmlGenerator
{
public:
	enum CONST_DEFINE
	{
		INVALID_STRING_INDEX = -1,
	};

	typedef std::vector<std::string> TV_STRING;
	typedef std::vector<COEXmlAttribute_Impl*> TV_XML_ATTRIBUTE;
	typedef std::vector<COEXmlNode_Impl*> TV_XML_NODE;

public:
	static COEXmlAttribute_Impl* CreateAttribute(int nNameIndex, int nValueIndex);
	static COEXmlNode_Impl* CreateNode(int nNameIndex, int nValueIndex);

	static void SetDocumentNode(COEXmlNode_Impl* pNodeDecl, COEXmlNode_Impl* pNodeRoot);

	static bool Parse(COEXmlNode_Impl*& pNodeDecl, COEXmlNode_Impl*& pNodeRoot, IOEFile* pFile);
	static bool Parse(COEXmlNode_Impl*& pNodeDecl, COEXmlNode_Impl*& pNodeRoot, const void* pBuffer, uint nSize);

	static int GetCharType(uchar ch);
	static const std::string& GetString(int nIndex);
	static int MakeName(const char* pszToken);
	static int MakeValue(const char* pszToken);
	static int MakeText(const char* pszToken);
	static int DumpString(char* pBufferOut, int nDumpSize);

private:
	static void SetupParam(const void* pBuffer, uint nSize);
	static void SetupParam(IOEFile* pFile);
	static void ResetParam(bool bFree);

private:
	static TV_STRING m_vString;
	static TV_XML_ATTRIBUTE m_vAttributes;
	static TV_XML_NODE m_vNodes;

	static const std::string m_EmptyString;

	static COEXmlNode_Impl* m_pNodeDecl;
	static COEXmlNode_Impl* m_pNodeRoot;

	static const char* m_pBuffer;
	static int m_nCurrPos;
	static int m_nBufferSize;

	static IOEFile* m_pFile;
};

extern int xmllex();
extern void xmlerror(const char* pszErrorMsg);

#endif // __XMLGENERATOR_H__
