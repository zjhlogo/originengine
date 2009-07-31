/*!
 * \file MeshMain.cpp
 * \date 31-7-2009 20:40:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <Windows.h>
#include "MeshMain.h"
#include "MeshApp.h"

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	CMeshApp* pApp = new CMeshApp();
	pApp->Run();
	SAFE_DELETE(pApp);

	return 0;
}
