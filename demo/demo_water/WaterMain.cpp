/*!
 * \file WaterMain.cpp
 * \date 20-6-2009 9:56:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <Windows.h>
#include <OEOS.h>
#include "WaterMain.h"
#include "WaterApp.h"

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	if (COEOS::Initialize())
	{
		CWaterApp* pApp = new CWaterApp();
		pApp->Run();
		SAFE_DELETE(pApp);
	}

	COEOS::Terminate();
	return 0;
}
