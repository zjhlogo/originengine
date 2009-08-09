/*!
 * \file TestMain.cpp
 * \date 24-5-2009 0:12:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <Windows.h>
#include <OEOS.h>
#include "TestMain.h"
#include "TestApp.h"

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	if (COEOS::Initialize())
	{
		CTestApp* pApp = new CTestApp();
		pApp->Run();
		SAFE_DELETE(pApp);
	}

	COEOS::Terminate();
	return 0;
}
