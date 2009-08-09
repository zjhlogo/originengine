/*!
 * \file OEOSWin.cpp
 * \date 31-5-2009 16:39:52
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEOS.h>
#include <OEHolder.h>

#include <Windows.h>
#include <vector>

#include "../OEFileMgr_Impl.h"
#include "../OELogFileMgr_Impl.h"
#include "../OEXmlMgr_Impl.h"

COEFileMgr_Impl* g_pOEFileMgr_Impl = NULL;
COELogFileMgr_Impl* g_pOELogFileMgr_Impl = NULL;
COEXmlMgr_Impl* g_pOEXmlMgr_Impl = NULL;

bool COEOS::Initialize()
{
	if (!g_pOEFileMgr_Impl)
	{
		g_pOEFileMgr_Impl = new COEFileMgr_Impl();
		if (!g_pOEFileMgr_Impl) return false;
	}

	if (!g_pOELogFileMgr_Impl)
	{
		g_pOELogFileMgr_Impl = new COELogFileMgr_Impl();
		if (!g_pOELogFileMgr_Impl) return false;
	}

	if (!g_pOEXmlMgr_Impl)
	{
		g_pOEXmlMgr_Impl = new COEXmlMgr_Impl();
		if (!g_pOEXmlMgr_Impl) return false;
	}

	return true;
}

void COEOS::Terminate()
{
	SAFE_DELETE(g_pOEXmlMgr_Impl);
	SAFE_DELETE(g_pOELogFileMgr_Impl);
	SAFE_DELETE(g_pOEFileMgr_Impl);
}

#define MODULE_INIT_FUNC_NAME _T("OEModuleInit")
#define MODULE_TERM_FUNC_NAME _T("OEModuleTerm")
#define MODULE_SYNC_FUNC_NAME _T("OEModuleSyncInterfaces")

typedef bool (*FUNC_MODULE_INIT)(COEHolder& Holder);
typedef void (*FUNC_MODULE_TERM)(COEHolder& Holder);
typedef void (*FUNC_MODULE_SYNC)(COEHolder& Holder);

COEOS::OEMODULE COEOS::LoadOEModule(const tchar* pstrModuleName)
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

	if (!OEModuleInit(g_OEHolder))
	{
		FreeLibrary(hWinModule);
		return NULL;
	}

	return (OEMODULE)hWinModule;
}

void COEOS::FreeOEModule(OEMODULE hModule)
{
	if (!hModule) return;

	std::string strModuleFunc;
	if (!tchar2char(strModuleFunc, MODULE_TERM_FUNC_NAME)) return;

	HMODULE hWinModule = (HMODULE)hModule;
	FUNC_MODULE_TERM OEModuleTerm = (FUNC_MODULE_TERM)GetProcAddress(hWinModule, strModuleFunc.c_str());
	if (OEModuleTerm) OEModuleTerm(g_OEHolder);
	FreeLibrary(hWinModule);
}

void COEOS::SyncModuleInterfaces(OEMODULE hModule)
{
	if (!hModule) return;

	std::string strModuleFunc;
	if (!tchar2char(strModuleFunc, MODULE_SYNC_FUNC_NAME)) return;

	HMODULE hWinModule = (HMODULE)hModule;
	FUNC_MODULE_SYNC OEModuleSync = (FUNC_MODULE_SYNC)GetProcAddress(hWinModule, strModuleFunc.c_str());
	if (OEModuleSync) OEModuleSync(g_OEHolder);
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

bool COEOS::int2str(tstring& strOut, int nValue)
{
	static tchar s_strBuffer[1024];
	_sntprintf(s_strBuffer, 1024, _T("%d"), nValue);
	strOut = s_strBuffer;
	return true;
}

bool COEOS::float2str(tstring& strOut, float fValue)
{
	static tchar s_strBuffer[1024];
	_sntprintf(s_strBuffer, 1024, _T("%f"), fValue);
	strOut = s_strBuffer;
	return true;
}

bool COEOS::strformat(tstring& strOut, const tchar* strFormat, ...)
{
	static const int MAX_BUFF_COUNT = 1024*10;
	static tchar s_strBuff[MAX_BUFF_COUNT];

	va_list marker;
	va_start(marker, strFormat);

	int nLength = _vsntprintf(s_strBuff, MAX_BUFF_COUNT, strFormat, marker);
	strOut = s_strBuff;
	va_end(marker);

	return true;
}
