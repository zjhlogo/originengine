/*!
 * \file OEFileMgr_Impl.cpp
 * \date 30-7-2009 17:07:29
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEFileMgr_Impl.h"
#include "OEFile_Impl.h"
#include <assert.h>

COEFileMgr_Impl* g_pOEFileMgr_Impl = NULL;

bool OEFileSystemInit()
{
	if (g_pOEFileMgr_Impl)
	{
		assert(false);
		return false;
	}

	g_pOEFileMgr_Impl = new COEFileMgr_Impl();
	if (!g_pOEFileMgr_Impl) return false;

	return true;
}

void OEFileSystemDestroy()
{
	SAFE_DELETE(g_pOEFileMgr_Impl);
}

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
	COEFile_Impl* pFile = new COEFile_Impl(strFileName, nFlag);
	if (!pFile || !pFile->IsOK())
	{
		SAFE_RELEASE(pFile);
		return NULL;
	}

	return pFile;
}
