/*!
 * \file MessageGenerator.h
 * \date 8-17-2010 23:42:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MESSAGEGENERATOR_H__
#define __MESSAGEGENERATOR_H__

#include <libOEBase/OEBaseType.h>
#include <OEBase/IOEXmlDocument.h>
#include <OEBase/IOEFile.h>
#include <vector>

class CMessageGenerator
{
public:
	typedef struct MESSAGE_MAP_tag
	{
		tstring strMsgID;
		tstring strClass;
		bool bDuplicateClass;
	} MESSAGE_MAP;

	typedef std::vector<MESSAGE_MAP> TV_MESSAGE_MAP;

public:
	CMessageGenerator();
	~CMessageGenerator();

	bool Convert(const tstring& strXmlFile, const tstring& strModule);

private:
	bool GatherMessage(TV_MESSAGE_MAP& MsgMap, IOEXmlDocument* pDocument);
	uint WriteString(const tstring& strString, IOEFile* pFile);

	bool GenerateH(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile);
	bool GenerateCPP(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile);

	bool WriteEnumulate(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile);
	bool WriteClassesFunc(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile);
	bool WriteMessageFunc(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile);
	bool WriteFileHeader(const tstring& strFileName, const tstring& strFileExt, IOEFile* pFile);
	bool WriteGuardHeader(const tstring& strFileName, const tstring& strFileExt, IOEFile* pFile);
	bool WriteGuardTail(const tstring& strFileName, const tstring& strFileExt, IOEFile* pFile);
	bool WriteIncludeFile(const TV_MESSAGE_MAP& MsgMap, IOEFile* pFile);

private:
	tstring m_strModule;
	tstring m_strBaseID;
	uint m_nMaxIDLength;

};
#endif // __MESSAGEGENERATOR_H__
