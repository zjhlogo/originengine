/*!
 * \file WaterMain.cpp
 * \date 20-6-2009 9:56:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <Windows.h>
#include "WaterMain.h"
#include "WaterApp.h"

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	CWaterApp* pApp = new CWaterApp();
	pApp->Run();
	SAFE_DELETE(pApp);

	return 0;
}
