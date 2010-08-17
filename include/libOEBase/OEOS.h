/*!
 * \file OEOS.h
 * \date 24-5-2009 9:57:58
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEOS_H__
#define __OEOS_H__

#include "OEBaseType.h"
#include <libOEBase/OEHolder.h>

class COEOS
{
public:
	typedef struct LOCAL_TIME_tag
	{
		int nYear;			// years [1900, 9999]
		int nMonth;			// month [1, 12]
		int nDayInYear;		// day in year [0, 365]
		int nDayInMonth;	// day in month [1, 31]
		int nDayInWeek;		// day in week [0, 6]
		int nHour;			// hour [0, 23]
		int nMinute;		// minute [0, 59]
		int nSecond;		// second [0, 59]
	} LOCAL_TIME;

	typedef void* OEMODULE;
	typedef void* OEFILE;

public:
	// module relate
	static OEMODULE LoadOEModule(const tchar* pstrModuleName, COEHolder& Holder);
	static void FreeOEModule(OEMODULE hModule, COEHolder& Holder);
	static void SyncModuleInterfaces(OEMODULE hModule, COEHolder& Holder);

	// string relate
	static bool str2bool(bool& bValue, const tchar* pstrIn);
	static bool str2int(int& nValue, const tchar* pstrIn);
	static bool str2float(float& fValue, const tchar* pstrIn);
	static bool tchar2char(std::string& strOut, const tchar* pstrIn);
	static bool char2tchar(tstring& strOut, const char* pstrIn);
	static bool bool2str(tstring& strOut, bool bValue);
	static void int2str(tstring& strOut, int nValue);
	static void float2str(tstring& strOut, float fValue);
	static bool strformat(tstring& strOut, const tchar* strFormat, ...);
	static void toupper(tstring& strOut, const tstring& strIn);
	static void tolower(tstring& strOut, const tstring& strIn);

	static void GetFileName(tstring& strOut, const tstring& strIn);
	static void GetFileExt(tstring& strOut, const tstring& strIn);
	static void GetFileDir(tstring& strOut, const tstring& strIn);

	// file system relate
	static OEFILE FileOpen(const tstring& strFile, const tstring& strOption);
	static void FileClose(OEFILE hFile);
	static void FileSeek(OEFILE hFile, int nOffset, uint nOrigin);
	static uint FileTell(OEFILE hFile);
	static uint FileRead(void* pDataOut, uint nSize, OEFILE hFile);
	static uint FileWrite(const void* pDataIn, uint nSize, OEFILE hFile);

	// time relate
	static uint64 TimeNow();
	static void TimeLocal(LOCAL_TIME& LocalTime, uint64 nTimeStamp);

};

#endif // __OEOS_H__
