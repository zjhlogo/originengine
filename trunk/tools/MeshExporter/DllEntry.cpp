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

static int g_nInitCount = 0;

static void Initialize(HINSTANCE hInstDLL)
{
	if (g_nInitCount <= 0)
	{
		InitCommonControls();
		InitCustomControls(hInstDLL);

		COEOS::Initialize(COEOS::IS_FILE | COEOS::IS_XML);

		wxInitHelper::Initialize(hInstDLL);
		wxInitHelper::AddMemoryXrc(TS("XRC"), IDR_XRC_DLGMESHEXPORTEROPTION, TS("DlgMeshExporterOption.xrc"));
	}

	++g_nInitCount;
}

static void Uninitialize()
{
	--g_nInitCount;

	if (g_nInitCount <= 0)
	{
		wxInitHelper::Uninitialize();
		COEOS::Terminate();
	}
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		{
			Initialize(hInstDLL);
		}
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		{
			Uninitialize();
		}
		break;
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
