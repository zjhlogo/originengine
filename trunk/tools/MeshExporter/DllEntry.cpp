/*!
 * \file DllEntry.cpp
 * \date 1-8-2009 10:25:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "DllEntry.h"
#include "MeshExporterDesc.h"
#include "resource.h"
#include "../../demo/common/wxInitHelper.h"
#include <OEOS.h>

static HINSTANCE g_hInst = NULL;
static BOOL g_bCtrlInit = FALSE;

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	g_hInst = hInstDLL;
	if (!g_bCtrlInit)
	{
		g_bCtrlInit = TRUE;
		InitCommonControls();
		InitCustomControls(g_hInst);

		COEOS::Initialize(COEOS::IS_FILE | COEOS::IS_XML);

		wxInitHelper::Initialize(g_hInst);
		wxInitHelper::AddMemoryXrc(TS("XRC"), IDR_XRC_DLGMESHEXPORTEROPTION, TS("DlgMeshExporterOption.xrc"));
	}

	return TRUE;
}

__declspec(dllexport) const TCHAR* LibDescription()
{
	return _T("Mesh Exporter Desc");
}

__declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		{
			static CMeshExporterDesc s_MeshExporterDesc;
			return &s_MeshExporterDesc;
		}
		break;
	}

	return NULL;
}

__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}
