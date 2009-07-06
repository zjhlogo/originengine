/*!
 * \file BumpMapMain.cpp
 * \date 6-7-2009 9:22:53
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <Windows.h>
#include "BumpMapMain.h"
#include "BumpMapApp.h"

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	CBumpMapApp* pApp = new CBumpMapApp();
	pApp->Run();
	SAFE_DELETE(pApp);

	return 0;
}
