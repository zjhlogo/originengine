/*!
 * \file UIMain.cpp
 * \date 27-7-2009 15:36:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <Windows.h>
#include <OEOS.h>
#include "UIMain.h"
#include "UIApp.h"

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	if (COEOS::Initialize())
	{
		CUIApp* pApp = new CUIApp();
		pApp->Run();
		SAFE_DELETE(pApp);
	}

	COEOS::Terminate();
	return 0;
}
