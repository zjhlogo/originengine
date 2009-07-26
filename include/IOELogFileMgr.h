/*!
 * \file IOELogFileMgr.h
 * \date 26-7-2009 10:20:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOELOGFILEMGR_H__
#define __IOELOGFILEMGR_H__

#include "OEBasicType.h"

class IOELogFileMgr
{
public:
	IOELogFileMgr() {};
	virtual ~IOELogFileMgr() {};

	virtual void LogOut(const tchar* pstrFormat, ...) = 0;
};

extern IOELogFileMgr* g_pOELogFileMgr;

#if (ENABLE_LOGOUT)
#define LOGOUT g_pOELogFileMgr->LogOut
#else
#define LOGOUT
#endif

#endif // __IOELOGFILEMGR_H__
