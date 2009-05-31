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

const char* COEOS::tchar2char(const tchar* pstrIn)
{
#ifdef _UNICODE
	static std::vector<char> s_MultiByteBuf;

	int nBufSize = WideCharToMultiByte(CP_UTF8, NULL, pstrIn, -1, NULL, 0, NULL, NULL);
	if ((int)s_MultiByteBuf.size() < nBufSize) s_MultiByteBuf.resize(nBufSize);

	WideCharToMultiByte(CP_UTF8, NULL, pstrIn, -1, &s_MultiByteBuf[0], nBufSize, NULL, NULL);
	return &s_MultiByteBuf[0];
#else
	return pstrIn;
#endif // _UNICODE
}

const tchar* COEOS::char2tchar(const char* pstrIn)
{
#ifdef _UNICODE
	static std::vector<tchar> s_WideCharBuf;

	int nBufSize = MultiByteToWideChar(CP_UTF8, NULL, pstrIn, -1, NULL, 0);
	if ((int)s_WideCharBuf.size() < nBufSize) s_WideCharBuf.resize(nBufSize);

	MultiByteToWideChar(CP_UTF8, NULL, pstrIn, -1, &s_WideCharBuf[0], nBufSize);
	return &s_WideCharBuf[0];
#else
	return pstrIn;
#endif // _UNICODE
}

COEOS::OEMODULE COEOS::LoadOEModule(const tchar* pstrModuleName)
{
	HMODULE hWinModule = LoadLibrary(pstrModuleName);
	if (!hWinModule) return NULL;

	FUNC_MODULE_INIT OEModuleInit = (FUNC_MODULE_INIT)GetProcAddress(hWinModule, tchar2char(MODULE_INIT_FUNC_NAME));
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

	HMODULE hWinModule = (HMODULE)hModule;
	FUNC_MODULE_TERM OEModuleTerm = (FUNC_MODULE_TERM)GetProcAddress(hWinModule, tchar2char(MODULE_TERM_FUNC_NAME));
	if (OEModuleTerm) OEModuleTerm(g_OEHolder);
	FreeLibrary(hWinModule);
}
