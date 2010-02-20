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
#include <assert.h>

COELogFileMgr_Impl::COELogFileMgr_Impl()
{
	assert(!g_pOELogFileMgr);
	g_pOELogFileMgr = this;
	m_bOK = Init();
}

COELogFileMgr_Impl::~COELogFileMgr_Impl()
{
	Destroy();
	g_pOELogFileMgr = NULL;
}

bool COELogFileMgr_Impl::Init()
{
	m_strFile = TS("log.txt");
	m_pLogFile = NULL;
	m_bFirstCreate = true;
	return true;
}

void COELogFileMgr_Impl::Destroy()
{
	SAFE_RELEASE(m_pLogFile);
}

void COELogFileMgr_Impl::SetLogFile(const tstring& strFile)
{
	SAFE_RELEASE(m_pLogFile);
	m_strFile = strFile;
	m_bFirstCreate = true;
}

void COELogFileMgr_Impl::LogOut(const tstring& strLogMsg)
{
	if (!m_pLogFile && !m_bFirstCreate) return;

	if (!m_pLogFile)
	{
		m_bFirstCreate = false;
		m_bOK = CreateLogFile(m_strFile);
		if (!m_bOK || !m_pLogFile) return;
	}

	COEOS::LOCAL_TIME LocalTime;
	COEOS::TimeLocal(LocalTime, COEOS::TimeNow());

	tstring strText;
	COEOS::strformat(strText, TS("[%04d-%02d-%02d %02d:%02d:%02d]: %s\n"),
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
