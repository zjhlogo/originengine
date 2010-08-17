/*!
 * \file AppHelper.h
 * \date 11-25-2009 16:43:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __APPHELPER_H__
#define __APPHELPER_H__

#include <windows.h>
#include <libOEBase/OEOS.h>

#define IMPLEMENT_OEAPP(appclass)																\
int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)		\
{																								\
	appclass* pApp = new appclass();															\
	pApp->Run();																				\
	SAFE_DELETE(pApp);																			\
	return 0;																					\
}																								\

#endif // __APPHELPER_H__
