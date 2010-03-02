/*!
 * \file wxToolUtil.cpp
 * \date 1-3-2010 12:03:17
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "wxToolUtil.h"
#include <wx/app.h>
#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>

bool wxToolUtil::m_bOK = false;

bool wxToolUtil::InitFileHandler()
{
	// add memory file system
	wxFileSystem::AddHandler(new wxMemoryFSHandler());

	m_bOK = true;
	return m_bOK;
}

bool wxToolUtil::AddMemoryXrc(const tstring& strResType, uint nResID, const tstring& strMemoryFileName, HINSTANCE hInstance /* = NULL */)
{
	if (!m_bOK) InitFileHandler();

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
