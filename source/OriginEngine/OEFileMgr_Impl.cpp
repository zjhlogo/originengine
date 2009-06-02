/*!
*	created:	2009/06/01
*	filename: 	OriginEngine\OEFileMgr_Impl.cpp
*	author:		daishuidi

*	purpose:	
*/

#include "OEFileMgr_Impl.h"
#include <OEInterfaces.h>
#include "OEFile_Impl.h"
#include <assert.h>

COEFileMgr_Impl::COEFileMgr_Impl()
{
	g_pOEFileMgr = this;
}

COEFileMgr_Impl::~COEFileMgr_Impl()
{
	g_pOEFileMgr = NULL;
}

IOEFile* COEFileMgr_Impl::OpenFile( const tchar* pstrFileName, uint nFlag )
{
	assert(pstrFileName);
	
	IOEFile *pOEFile = new COEFile_Impl(pstrFileName, nFlag);
	return pOEFile;
}