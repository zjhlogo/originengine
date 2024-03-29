/*!
 *	created:	2009/06/01
 *	filename: 	OriginEngine\OEFileMgr_Impl.cpp
 *	author:		daishuidi

 *	purpose:	
 */
#include "OEFileMgr_Impl.h"
#include "OEFile_Impl.h"

#include <OEBase/IOELogFileMgr.h>
#include <assert.h>

COEFileMgr_Impl::COEFileMgr_Impl()
{
	assert(!g_pOEFileMgr);
	g_pOEFileMgr = this;
	m_bOK = Init();
}

COEFileMgr_Impl::~COEFileMgr_Impl()
{
	Destroy();
	g_pOEFileMgr = NULL;
}

bool COEFileMgr_Impl::Init()
{
	// TODO: 
	return true;
}

void COEFileMgr_Impl::Destroy()
{
	// TODO: 
}

bool COEFileMgr_Impl::Initialize()
{
	// TODO: 
	return true;
}

void COEFileMgr_Impl::Terminate()
{
	// TODO: 
}

IOEFile* COEFileMgr_Impl::OpenFile(const tstring& strFileName, uint nFlag /* = IOEFile::OFF_READ */)
{
	COEFile_Impl *pFile = new COEFile_Impl(strFileName, nFlag);
	if (!pFile || !pFile->IsOK())
	{
		LOGOUT(TS("IOEFileMgr::OpenFile Failed \"%s\""), strFileName.c_str());
		SAFE_RELEASE(pFile);
		return NULL;
	}

	return pFile;
}
