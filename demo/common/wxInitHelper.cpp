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

bool wxInitHelper::Initialize()
{
	if (!wxInitialize()) return false;

	wxXmlResource::Get()->InitAllHandlers();

	// add memory file system
	wxFileSystem::AddHandler(new wxMemoryFSHandler());

	return true;
}

void wxInitHelper::Uninitialize()
{
	wxUninitialize();
}

bool wxInitHelper::AddMemoryXrc(const tstring& strResType, uint nResID, const tstring& strMemoryFileName, HINSTANCE hInstance /* = NULL */)
{
	// load the resource
	HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(nResID), strResType.c_str());
	HGLOBAL hResData = LoadResource(hInstance, hRes);
	void* pResBuffer = LockResource(hResData);
	if (!pResBuffer) return false;
	int nResSize = SizeofResource(hInstance, hRes);

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
