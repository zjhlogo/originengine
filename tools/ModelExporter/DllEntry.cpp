/*!
 * \file DllEntry.cpp
 * \date 1-8-2009 10:25:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "DllEntry.h"
#include "ModelExporterDesc.h"
#include "resource.h"

#include "../common/wxToolUtil.h"

#include <OEOS.h>
#include <wx/app.h>
#include <wx/xrc/xmlres.h>

static int g_nInitCount = 0;

IMPLEMENT_APP_NO_MAIN(wxApp);

static void Initialize(HINSTANCE hInstDLL)
{
	if (g_nInitCount <= 0)
	{
		InitCommonControls();
		//InitCustomControls(hInstDLL);

		COEOS::Initialize(COEOS::IS_FILE | COEOS::IS_XML);

		wxInitialize();
		wxXmlResource::Get()->InitAllHandlers();

		wxToolUtil::AddMemoryXrc(TS("XRC"), IDR_XRC_DLGMODELEXPORTEROPTION, TS("DlgModelExporterOption.xrc"), hInstDLL);
	}

	++g_nInitCount;
}

static void Uninitialize()
{
	--g_nInitCount;

	if (g_nInitCount <= 0)
	{
		wxUninitialize();
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
	return _T("Model Exporter Desc");
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
			static CModelExporterDesc s_MeshExporterDesc;
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
