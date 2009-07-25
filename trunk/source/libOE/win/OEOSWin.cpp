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

#define MODULE_INIT_FUNC_NAME _T("OEModuleInit")
#define MODULE_TERM_FUNC_NAME _T("OEModuleTerm")

typedef bool (*FUNC_MODULE_INIT)(COEHolder& Holder);
typedef void (*FUNC_MODULE_TERM)(COEHolder& Holder);

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

	std::string strModuleName;
	if (!tchar2char(strModuleName, MODULE_TERM_FUNC_NAME)) return;

	HMODULE hWinModule = (HMODULE)hModule;
	FUNC_MODULE_TERM OEModuleTerm = (FUNC_MODULE_TERM)GetProcAddress(hWinModule, strModuleName.c_str());
	if (OEModuleTerm) OEModuleTerm(g_OEHolder);
	FreeLibrary(hWinModule);
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
