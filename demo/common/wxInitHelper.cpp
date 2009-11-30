/*!
 * \file wxInitHelper.cpp
 * \date 28-11-2009 9:26:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "wxInitHelper.h"
#include <wx/app.h>
#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>

IMPLEMENT_APP_NO_MAIN(wxApp);

static HINSTANCE g_hInst = NULL;

bool wxInitHelper::Initialize(HINSTANCE hInst /* = NULL */)
{
	if (!wxInitialize()) return false;

	g_hInst = hInst;

	// add memory file system
	wxFileSystem::AddHandler(new wxMemoryFSHandler());
	wxXmlResource::Get()->InitAllHandlers();

	return true;
}

void wxInitHelper::Uninitialize()
{
	g_hInst = NULL;
	wxUninitialize();
}

bool wxInitHelper::AddMemoryXrc(const tstring& strResType, uint nResID, const tstring& strMemoryFileName)
{
	// load the resource
	HRSRC hRes = FindResource(g_hInst, MAKEINTRESOURCE(nResID), strResType.c_str());
	HGLOBAL hResData = LoadResource(g_hInst, hRes);
	void* pResBuffer = LockResource(hResData);
	if (!pResBuffer) return false;
	int nResSize = SizeofResource(g_hInst, hRes);

	// add resource into memory file system
	wxMemoryFSHandler::AddFile(strMemoryFileName.c_str(), pResBuffer, nResSize);

	// unload the resource
	UnlockResource(hResData);
	//// MSDN: For 32-bit Windows applications, it is not necessary to free the resources loaded using function LoadResource
	//FreeResource(hResData);

	// add xrc file
	wxString strMemoryPath = wxT("memory:");
	strMemoryPath += strMemoryFileName.c_str();
	if (!wxXmlResource::Get()->Load(strMemoryPath)) return false;

	return true;
}
