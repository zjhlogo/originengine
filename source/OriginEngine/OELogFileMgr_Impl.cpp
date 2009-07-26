/*!
 * \file OELogFileMgr_Impl.cpp
 * \date 26-7-2009 10:33:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OELogFileMgr_Impl.h"
#include <OEOS.h>
#include <stdarg.h>
#include <time.h>

COELogFileMgr_Impl::COELogFileMgr_Impl()
{
	g_pOELogFileMgr = this;
	Init();
	CreateLogFile(m_strLogFileName);
}

COELogFileMgr_Impl::~COELogFileMgr_Impl()
{
	Destroy();
	g_pOELogFileMgr = NULL;
}

void COELogFileMgr_Impl::Init()
{
	m_strLogFileName = _T("log.txt");
	m_pLogFile = NULL;
}

void COELogFileMgr_Impl::Destroy()
{
	SAFE_RELEASE(m_pLogFile);
}

void COELogFileMgr_Impl::LogOut(const tchar* pstrFormat, ...)
{
	static const int MAX_BUFF_COUNT = 1024*10;
	static tchar s_strBuffer[MAX_BUFF_COUNT];

	if (!m_pLogFile) return;

	va_list marker;
	va_start(marker, pstrFormat);
	_vsntprintf(s_strBuffer, MAX_BUFF_COUNT, pstrFormat, marker);
	va_end(marker);

	time_t SysTime = time(NULL);
	tm* pLocalTime = localtime(&SysTime);

	tstring strText;
	COEOS::strformat(strText, _T("[%04d-%02d-%02d %02d:%02d:%02d]: %s\n"),
		pLocalTime->tm_year+1900,
		pLocalTime->tm_mon+1,
		pLocalTime->tm_mday,
		pLocalTime->tm_hour,
		pLocalTime->tm_min,
		pLocalTime->tm_sec,
		s_strBuffer);

	std::string strAnsiText;
	COEOS::tchar2char(strAnsiText, strText.c_str());

	m_pLogFile->Write(strAnsiText.c_str(), (uint)strAnsiText.length());
}

bool COELogFileMgr_Impl::CreateLogFile(const tstring& strFileName)
{
	static uchar s_BOM[] = {0xEF, 0xBB, 0xBF};

	SAFE_RELEASE(m_pLogFile);

	m_pLogFile = g_pOEFileMgr->OpenFile(strFileName, IOEFile::OFF_WRITE);
	if (!m_pLogFile) return false;

	m_pLogFile->Write(s_BOM, 3);

	return true;
}
