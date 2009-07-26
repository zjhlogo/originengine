/*!
 *	created:	2009/06/01
 *	filename: 	OriginEngine\OEFileMgr_Impl.cpp
 *	author:		daishuidi

 *	purpose:	
 */
#include "OEFileMgr_Impl.h"
#include "OEFile_Impl.h"

#include <OEInterfaces.h>
#include <assert.h>

COEFileMgr_Impl::COEFileMgr_Impl()
{
	g_pOEFileMgr = this;
	Init();
}

COEFileMgr_Impl::~COEFileMgr_Impl()
{
	Destroy();
	g_pOEFileMgr = NULL;
}

void COEFileMgr_Impl::Init()
{
	// TODO: 
}

void COEFileMgr_Impl::Destroy()
{
	// TODO: 
}

IOEFile* COEFileMgr_Impl::OpenFile(const tstring& strFileName, uint nFlag /* = IOEFile::OFF_READ */)
{
	IOEFile *pFile = new COEFile_Impl(strFileName, nFlag);
	if (!pFile || !pFile->IsOK())
	{
		SAFE_RELEASE(pFile);
		LOGOUT(_T("IOEFileMgr::OpenFile Failed \"%s\""), strFileName.c_str());
		return NULL;
	}

	return pFile;
}
