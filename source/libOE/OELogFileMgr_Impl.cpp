/*!
 * \file OELogFileMgr_Impl.cpp
 * \date 26-7-2009 10:33:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OELogFileMgr_Impl.h"

#include <IOEFileMgr.h>
#include <OEOS.h>

COELogFileMgr_Impl::COELogFileMgr_Impl()
{
	g_pOELogFileMgr = this;
	Init();
	m_bOK = CreateLogFile(m_strLogFileName);
}

COELogFileMgr_Impl::~COELogFileMgr_Impl()
{
	Destroy();
	g_pOELogFileMgr = NULL;
}

void COELogFileMgr_Impl::Init()
{
	m_strLogFileName = t("log.txt");
	m_pLogFile = NULL;
}

void COELogFileMgr_Impl::Destroy()
{
	SAFE_RELEASE(m_pLogFile);
}

void COELogFileMgr_Impl::LogOut(const tstring& strLogMsg)
{
	if (!m_pLogFile) return;

	COEOS::LOCAL_TIME LocalTime;
	COEOS::TimeLocal(LocalTime, COEOS::TimeNow());

	tstring strText;
	COEOS::strformat(strText, t("[%04d-%02d-%02d %02d:%02d:%02d]: %s\n"),
		LocalTime.nYear,
		LocalTime.nMonth,
		LocalTime.nDayInMonth,
		LocalTime.nHour,
		LocalTime.nMinute,
		LocalTime.nSecond,
		strLogMsg.c_str());

	std::string strAnsiText;
	COEOS::tchar2char(strAnsiText, strText.c_str());

	m_pLogFile->Write(strAnsiText.c_str(), (uint)strAnsiText.length());
}

tstring& COELogFileMgr_Impl::GetStringBuffer()
{
	return m_strBuffer;
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