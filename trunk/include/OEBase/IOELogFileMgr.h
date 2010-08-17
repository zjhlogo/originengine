/*!
 * \file IOELogFileMgr.h
 * \date 26-7-2009 10:20:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOELOGFILEMGR_H__
#define __IOELOGFILEMGR_H__

#include "../OECore/IOEMgr.h"
#include <libOEBase/OEOS.h>

class IOELogFileMgr : public IOEMgr
{
public:
	IOELogFileMgr() {};
	virtual ~IOELogFileMgr() {};

	virtual void SetLogFile(const tstring& strFile) = 0;
	virtual void LogOut(const tstring& strLogMsg) = 0;
	virtual tstring& GetStringBuffer() = 0;
};

extern IOELogFileMgr* g_pOELogFileMgr;

#if (ENABLE_LOGOUT)
	#define LOGOUT(pstrFormat, ...)																\
	if (g_pOELogFileMgr)																		\
	{																							\
		COEOS::strformat(g_pOELogFileMgr->GetStringBuffer(), pstrFormat, __VA_ARGS__);			\
		g_pOELogFileMgr->LogOut(g_pOELogFileMgr->GetStringBuffer());							\
	}
#else
	#define LOGOUT(pstrFormat, ...) 0
#endif

#endif // __IOELOGFILEMGR_H__
