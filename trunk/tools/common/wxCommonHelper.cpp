/*!
 * \file wxCommonHelper.cpp
 * \date 3-3-2010 15:39:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "wxCommonHelper.h"
#include <wx/app.h>
#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>

IMPLEMENT_APP_NO_MAIN(wxApp);

bool wxCommonHelper::Initialize()
{
	if (!wxInitialize()) return false;

	if (!InitializeXrc()) return false;

	return true;
}

void wxCommonHelper::Terminate()
{
	wxUninitialize();
}

bool wxCommonHelper::InitializeXrc()
{
	wxXmlResource::Get()->InitAllHandlers();

	// add memory file system
	wxFileSystem::AddHandler(new wxMemoryFSHandler());

	return true;
}

bool wxCommonHelper::AddMemoryXrc(const tstring& strResType, uint nResID, const tstring& strMemoryFileName, HINSTANCE hInstance /* = NULL */)
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
