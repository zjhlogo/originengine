/*!
 * \file OEOS.cpp
 * \date 3-3-2010 7:36:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <libOEBase/OEOS.h>
#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <algorithm>
#include <vector>

#define MODULE_INIT_FUNC_NAME TS("OEModuleInit")
#define MODULE_TERM_FUNC_NAME TS("OEModuleTerm")
#define MODULE_SYNC_FUNC_NAME TS("OEModuleSyncInterfaces")

typedef bool (*FUNC_MODULE_INIT)(COEHolder& Holder);
typedef void (*FUNC_MODULE_TERM)(COEHolder& Holder);
typedef void (*FUNC_MODULE_SYNC)(COEHolder& Holder);

COEOS::OEMODULE COEOS::LoadOEModule(const tchar* pstrModuleName, COEHolder& Holder)
{
	HMODULE hWinModule = LoadLibrary(pstrModuleName);
	if (!hWinModule) return NULL;

	std::string strModuleName;
	if (!tchar2char(strModuleName, MODULE_INIT_FUNC_NAME)) return NULL;

	FUNC_MODULE_INIT OEModuleInit = (FUNC_MODULE_INIT)GetProcAddress(hWinModule, strModuleName.c_str());
	if (!OEModuleInit)
	{
		FreeLibrary(hWinModule);
		return NULL;
	}

	if (!OEModuleInit(Holder))
	{
		FreeLibrary(hWinModule);
		return NULL;
	}

	return (OEMODULE)hWinModule;
}

void COEOS::FreeOEModule(OEMODULE hModule, COEHolder& Holder)
{
	if (!hModule) return;

	std::string strModuleFunc;
	if (!tchar2char(strModuleFunc, MODULE_TERM_FUNC_NAME)) return;

	HMODULE hWinModule = (HMODULE)hModule;
	FUNC_MODULE_TERM OEModuleTerm = (FUNC_MODULE_TERM)GetProcAddress(hWinModule, strModuleFunc.c_str());
	if (OEModuleTerm) OEModuleTerm(Holder);
	FreeLibrary(hWinModule);
}

void COEOS::SyncModuleInterfaces(OEMODULE hModule, COEHolder& Holder)
{
	if (!hModule) return;

	std::string strModuleFunc;
	if (!tchar2char(strModuleFunc, MODULE_SYNC_FUNC_NAME)) return;

	HMODULE hWinModule = (HMODULE)hModule;
	FUNC_MODULE_SYNC OEModuleSync = (FUNC_MODULE_SYNC)GetProcAddress(hWinModule, strModuleFunc.c_str());
	if (OEModuleSync) OEModuleSync(Holder);
}

bool COEOS::tchar2char(std::string& strOut, const tchar* pstrIn)
{
#ifdef _UNICODE
	static std::vector<char> s_MultiByteBuf;

	int nBufSize = WideCharToMultiByte(CP_UTF8, NULL, pstrIn, -1, NULL, 0, NULL, NULL);
	if ((int)s_MultiByteBuf.size() < nBufSize) s_MultiByteBuf.resize(nBufSize);

	WideCharToMultiByte(CP_UTF8, NULL, pstrIn, -1, &s_MultiByteBuf[0], nBufSize, NULL, NULL);
	strOut = ((char*)(&s_MultiByteBuf[0]));
	return true;
#else
	strOut = pstrIn;
	return true;
#endif // _UNICODE
}

bool COEOS::char2tchar(tstring& strOut, const char* pstrIn)
{
#ifdef _UNICODE
	static std::vector<tchar> s_WideCharBuf;

	int nBufSize = MultiByteToWideChar(CP_UTF8, NULL, pstrIn, -1, NULL, 0);
	if ((int)s_WideCharBuf.size() < nBufSize) s_WideCharBuf.resize(nBufSize);

	MultiByteToWideChar(CP_UTF8, NULL, pstrIn, -1, &s_WideCharBuf[0], nBufSize);
	strOut = ((tchar*)&s_WideCharBuf[0]);
	return true;
#else
	strOut = pstrIn;
	return true;
#endif // _UNICODE
}

bool COEOS::str2bool(bool& bValue, const tchar* pstrIn)
{
	tstring strBool;
	tolower(strBool, pstrIn);
	if (strBool == TS("true")) bValue = true;
	else if (strBool == TS("false")) bValue = false;
	else return false;

	return true;
}

bool COEOS::str2int(int& nValue, const tchar* pstrIn)
{
	nValue = _tstoi(pstrIn);
	return true;
}

bool COEOS::str2float(float& fValue, const tchar* pstrIn)
{
	fValue = (float)_tstof(pstrIn);
	return true;
}

bool COEOS::bool2str(tstring& strOut, bool bValue)
{
	if (bValue) strOut = TS("true");
	else strOut = TS("false");

	return true;
}

void COEOS::int2str(tstring& strOut, int nValue)
{
	static tchar s_strBuffer[1024];
	_sntprintf_s(s_strBuffer, 1024, TS("%d"), nValue);
	strOut = s_strBuffer;
}

void COEOS::float2str(tstring& strOut, float fValue)
{
	static tchar s_strBuffer[1024];
	_sntprintf_s(s_strBuffer, 1024, TS("%f"), fValue);
	strOut = s_strBuffer;
}

bool COEOS::strformat(tstring& strOut, const tchar* strFormat, ...)
{
	static const int MAX_BUFF_COUNT = 1024*10;
	static tchar s_strBuff[MAX_BUFF_COUNT];

	va_list marker;
	va_start(marker, strFormat);

	int nLength = _vsntprintf_s(s_strBuff, MAX_BUFF_COUNT, strFormat, marker);
	strOut = s_strBuff;
	va_end(marker);

	return true;
}

void COEOS::toupper(tstring& strOut, const tstring& strIn)
{
	strOut = strIn;
	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::toupper);
}

void COEOS::tolower(tstring& strOut, const tstring& strIn)
{
	strOut = strIn;
	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);
}

void COEOS::GetFileNameWithoutExt(tstring& strOut, const tstring& strIn)
{
	size_t nPosBegin = strIn.rfind(('\\'));
	if (nPosBegin == tstring::npos)
	{
		nPosBegin = strIn.rfind(('/'));
		if (nPosBegin == tstring::npos) nPosBegin = -1;
	}
	++nPosBegin;

	size_t nPosEnd = strIn.rfind(('.'));
	if (nPosEnd == tstring::npos) nPosEnd = strIn.length();

	strOut = strIn.substr(nPosBegin, nPosEnd-nPosBegin);
}

void COEOS::GetFileNameWithExt(tstring& strOut, const tstring& strIn)
{
	size_t nPosBegin = strIn.rfind(('\\'));
	if (nPosBegin == tstring::npos)
	{
		nPosBegin = strIn.rfind(('/'));
		if (nPosBegin == tstring::npos) nPosBegin = -1;
	}
	++nPosBegin;

	size_t nPosEnd = strIn.length();
	strOut = strIn.substr(nPosBegin, nPosEnd);
}

void COEOS::GetFileExt(tstring& strOut, const tstring& strIn)
{
	size_t nPos = strIn.rfind(TS('.'));
	if (nPos != tstring::npos)
	{
		strOut = strIn.substr(nPos+1, tstring::npos);
	}
	else
	{
		strOut.clear();
	}
}

void COEOS::GetFileDir(tstring& strOut, const tstring& strIn)
{
	size_t nPos = strIn.rfind(TS('\\'));
	if (nPos != tstring::npos)
	{
		strOut = strIn.substr(0, nPos);
	}
	else
	{
		strOut.clear();
	}
}

COEOS::OEFILE COEOS::FileOpen(const tstring& strFile, const tstring& strOption)
{
	FILE* pFile = NULL;
	_tfopen_s(&pFile, strFile.c_str(), strOption.c_str());
	return (OEFILE)pFile;
}

void COEOS::FileClose(OEFILE hFile)
{
	if (hFile) fclose((FILE*)hFile);
}

void COEOS::FileSeek(OEFILE hFile, int nOffset, uint nOrigin)
{
	fseek((FILE*)hFile, nOffset, nOrigin);
}

uint COEOS::FileTell(OEFILE hFile)
{
	return ftell((FILE*)hFile);
}

uint COEOS::FileRead(void* pDataOut, uint nSize, OEFILE hFile)
{
	return fread(pDataOut, 1, nSize, (FILE*)hFile);
}

uint COEOS::FileWrite(const void* pDataIn, uint nSize, OEFILE hFile)
{
	return fwrite(pDataIn, 1, nSize, (FILE*)hFile);
}

uint64 COEOS::TimeNow()
{
	return time(NULL);
}

void COEOS::TimeLocal(LOCAL_TIME& LocalTime, uint64 nTimeStamp)
{
	tm tmLocal;
	localtime_s(&tmLocal, (time_t*)&nTimeStamp);

	LocalTime.nYear = tmLocal.tm_year + 1900;
	LocalTime.nMonth = tmLocal.tm_mon + 1;
	LocalTime.nDayInYear = tmLocal.tm_yday;
	LocalTime.nDayInMonth = tmLocal.tm_mday;
	LocalTime.nDayInWeek = tmLocal.tm_wday;
	LocalTime.nHour = tmLocal.tm_hour;
	LocalTime.nMinute = tmLocal.tm_min;
	LocalTime.nSecond = tmLocal.tm_sec;
}
