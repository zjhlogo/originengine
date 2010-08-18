/*!
 * \file MessageGenerator.cpp
 * \date 8-17-2010 23:42:23
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "MessageGenerator.h"
#include <OEBase/IOEXmlMgr.h>
#include <OEBase/IOEFileMgr.h>
#include <libOEBase/OEOS.h>
#include <set>

CMessageGenerator::CMessageGenerator()
{
	// TODO: 
}

CMessageGenerator::~CMessageGenerator()
{
	// TODO: 
}

bool CMessageGenerator::Convert(const tstring& strXmlFile, const tstring& strModule)
{
	m_strModule = strModule;

	IOEXmlDocument* pDocument = g_pOEXmlMgr->CreateDocument(strXmlFile);
	if (!pDocument) return false;

	pDocument->GetRootNode()->GetAttribute(m_strBaseID, TS("minid"));

	TV_MESSAGE_MAP MsgMap;
	bool bOK = GatherMessage(MsgMap, pDocument);
	SAFE_RELEASE(pDocument);
	if (!bOK) return false;

	tstring strFileName = m_strModule + TS(".h");
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName, IOEFile::OFF_WRITE);
	if (!pFile) return false;
	GenerateH(MsgMap, pFile);
	SAFE_RELEASE(pFile);

	strFileName = m_strModule + TS(".cpp");
	pFile = g_pOEFileMgr->OpenFile(strFileName, IOEFile::OFF_WRITE);
	if (!pFile) return false;
	GenerateCPP(MsgMap, pFile);
	SAFE_RELEASE(pFile);

	return bOK;
}

bool CMessageGenerator::GatherMessage(TV_MESSAGE_MAP& MsgMap, IOEXmlDocument* pDocument)
{
	IOEXmlNode* pMessageList = pDocument->GetRootNode();
	if (!pMessageList) return false;

	m_nMaxIDLength = 0;
	std::set<tstring> sMsgID;
	std::set<tstring> sClass;
	for (TV_MESSAGE_MAP::const_iterator it = MsgMap.begin(); it != MsgMap.end(); ++it)
	{
		const MESSAGE_MAP& Msg = (*it);
		if (m_nMaxIDLength < Msg.strMsgID.length()) m_nMaxIDLength = Msg.strMsgID.length();

		sMsgID.insert(Msg.strMsgID);
		sClass.insert(Msg.strClass);
	}

	for (IOEXmlNode* pMessage = pMessageList->FirstChild(TS("Message")); pMessage != NULL; pMessage = pMessage->NextSibling(TS("Message")))
	{
		MESSAGE_MAP Msg;

		if (!pMessage->GetAttribute(Msg.strMsgID, TS("id"))) continue;

		tstring strClass;
		if (!pMessage->GetAttribute(Msg.strClass, TS("class"))) continue;

		if (sMsgID.find(Msg.strMsgID) != sMsgID.end())
		{
			// TODO: log out duplicate ID
			return false;
		}

		Msg.bDuplicateClass = (sClass.find(Msg.strClass) != sClass.end());
		MsgMap.push_back(Msg);

		if (m_nMaxIDLength < Msg.strMsgID.length()) m_nMaxIDLength = Msg.strMsgID.length();
		sMsgID.insert(Msg.strMsgID);
		sClass.insert(Msg.strClass);
	}

	return true;
}

uint CMessageGenerator::WriteString(const tstring& strString, IOEFile* pFile)
{
	std::string strAnsiText;
	COEOS::tchar2char(strAnsiText, strString.c_str());

	return pFile->Write(strAnsiText.c_str(), (uint)strAnsiText.length());
}

bool CMessageGenerator::GenerateH(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile)
{
	WriteFileHeader(m_strModule, TS("h"), pFile);
	WriteGuardHeader(m_strModule, TS("h"), pFile);

	WriteString(TS("#include <OEBase/IOEMsgMgr.h>\n\n"), pFile);

	WriteEnumulate(MsgMap, pFile);

	tstring strOut;
	COEOS::strformat(strOut, TS("\nextern const IOEMsgMgr::MSG_GENERATE_MAP g_%sDB[%d];\n\n"), m_strModule.c_str(), MsgMap.size()+1);
	WriteString(strOut, pFile);

	WriteGuardTail(m_strModule, TS("h"), pFile);
	return true;
}

bool CMessageGenerator::GenerateCPP(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile)
{
	WriteFileHeader(m_strModule, TS("cpp"), pFile);

	tstring strOut;
	COEOS::strformat(strOut, TS("#include \"%s.h\"\n\n"), m_strModule.c_str());
	WriteString(strOut, pFile);

	WriteIncludeFile(MsgMap, pFile);
	WriteString(TS("\n"), pFile);

	WriteClassesFunc(MsgMap, pFile);
	WriteMessageFunc(MsgMap, pFile);

	return true;
}

bool CMessageGenerator::WriteEnumulate(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile)
{
	static tchar* s_szFormat = TS("\t%s,%s// %s\n");

	tstring strModuleUpper;
	COEOS::toupper(strModuleUpper, m_strModule);

	tstring strOut;
	COEOS::strformat(strOut, TS("enum %s_ID_DEFINE\n{\n"), strModuleUpper.c_str());
	WriteString(strOut, pFile);

	COEOS::strformat(strOut, TS("\t%s_ID_MIN = %s, \n"), strModuleUpper.c_str(), m_strBaseID.c_str());
	WriteString(strOut, pFile);

	int nMaxLength = (m_nMaxIDLength + 8)/4*4;
	for (TV_MESSAGE_MAP::const_iterator it = MsgMap.begin(); it != MsgMap.end(); ++it)
	{
		const MESSAGE_MAP& Msg = (*it);

		tstring strSpace;
		strSpace.append(nMaxLength - Msg.strMsgID.length(), TS(' '));

		COEOS::strformat(strOut, s_szFormat, Msg.strMsgID.c_str(), strSpace.c_str(), Msg.strClass.c_str());
		WriteString(strOut, pFile);
	}

	WriteString(TS("};\n"), pFile);
	return true;
}

bool CMessageGenerator::WriteClassesFunc(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile)
{
	static tchar* s_szFormat = 
		TS("static %s* __AutoGen_%s(COEDataBufferRead* pDBRead)\n")
		TS("{\n")
		TS("\treturn new %s(pDBRead);\n")
		TS("}\n");

	tstring strNewLine = TS("");
	for (TV_MESSAGE_MAP::const_iterator it = MsgMap.begin(); it != MsgMap.end(); ++it)
	{
		const MESSAGE_MAP& Msg = (*it);
		if (Msg.bDuplicateClass) continue;

		tstring strOut;
		COEOS::strformat(strOut, s_szFormat, Msg.strClass.c_str(), Msg.strClass.c_str(), Msg.strClass.c_str());

		WriteString(strNewLine, pFile);
		WriteString(strOut, pFile);

		strNewLine = TS("\n");
	}

	return true;
}

bool CMessageGenerator::WriteMessageFunc(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile)
{
	static tchar* s_szFormat = TS("\t{%s,%s(IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_%s},\n");

	tstring strOut;
	COEOS::strformat(strOut, TS("\nconst IOEMsgMgr::MSG_GENERATE_MAP g_%sDB[%d] = \n{\n"), m_strModule.c_str(), MsgMap.size()+1);
	WriteString(strOut, pFile);

	int nMaxLength = (m_nMaxIDLength + 8)/4*4;
	for (TV_MESSAGE_MAP::const_iterator it = MsgMap.begin(); it != MsgMap.end(); ++it)
	{
		const MESSAGE_MAP& Msg = (*it);

		tstring strSpace;
		strSpace.append(nMaxLength - Msg.strMsgID.length(), TS(' '));
		COEOS::strformat(strOut, s_szFormat, Msg.strMsgID.c_str(), strSpace.c_str(), Msg.strClass.c_str());
		WriteString(strOut, pFile);
	}

	tstring strSpace;
	strSpace.append(nMaxLength - 1, TS(' '));
	COEOS::strformat(strOut, TS("\t{%s,%s(IOEMsgMgr::MSG_GENERATE_FUNC)%s}\n"), TS("0"), strSpace.c_str(), TS("NULL"));
	WriteString(strOut, pFile);

	WriteString(TS("};\n"), pFile);

	return true;
}

bool CMessageGenerator::WriteFileHeader(const tstring& strFileName, const tstring& strFileExt, IOEFile* pFile)
{
	static tchar* s_szFormat = 
		TS("/*!\n")
		TS(" * \\file %s.%s\n")
		TS(" * \\date %d-%d-%d %d:%d:%d\n")
		TS(" * \n")
		TS(" * \n")
		TS(" * \\author MessageGenerator (zjhlogo@gmail.com)\n")
		TS(" */\n");

	COEOS::LOCAL_TIME Time;
	COEOS::TimeLocal(Time, COEOS::TimeNow());

	tstring strOut;
	COEOS::strformat(strOut, s_szFormat,
		strFileName.c_str(), strFileExt.c_str(),
		Time.nMonth, Time.nDayInMonth, Time.nYear, Time.nHour, Time.nMinute, Time.nSecond);

	WriteString(strOut, pFile);

	return true;
}

bool CMessageGenerator::WriteGuardHeader(const tstring& strFileName, const tstring& strFileExt, IOEFile* pFile)
{
	static tchar* s_szFormat = TS("#ifndef __%s_%s__\n")
		TS("#define __%s_%s__\n\n");

	tstring strFileNameUpper;
	tstring strFileExtUpper;
	COEOS::toupper(strFileNameUpper, strFileName);
	COEOS::toupper(strFileExtUpper, strFileExt);

	tstring strOut;
	COEOS::strformat(strOut, s_szFormat, strFileNameUpper.c_str(), strFileExtUpper.c_str(), strFileNameUpper.c_str(), strFileExtUpper.c_str());
	WriteString(strOut, pFile);

	return true;
}

bool CMessageGenerator::WriteGuardTail(const tstring& strFileName, const tstring& strFileExt, IOEFile* pFile)
{
	static tchar* s_szFormat = TS("#endif // __%s_%s__\n");

	tstring strFileNameUpper;
	tstring strFileExtUpper;
	COEOS::toupper(strFileNameUpper, strFileName);
	COEOS::toupper(strFileExtUpper, strFileExt);

	tstring strOut;
	COEOS::strformat(strOut, s_szFormat, strFileNameUpper.c_str(), strFileExtUpper.c_str());
	WriteString(strOut, pFile);

	return true;
}

bool CMessageGenerator::WriteIncludeFile(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile)
{
	static tchar* s_szFormat = TS("#include \"%s.h\"\n");

	for (TV_MESSAGE_MAP::const_iterator it = MsgMap.begin(); it != MsgMap.end(); ++it)
	{
		const MESSAGE_MAP& Msg = (*it);
		if (Msg.bDuplicateClass) continue;

		tstring strFileName = Msg.strClass;
		if (strFileName[0] == TS('C')) strFileName.erase(0, 1);

		tstring strOut;
		COEOS::strformat(strOut, s_szFormat, strFileName.c_str());
		WriteString(strOut, pFile);
	}

	return true;
}
