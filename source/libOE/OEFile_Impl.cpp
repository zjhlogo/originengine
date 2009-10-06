/*!
*	created:	2009/06/01
*	filename: 	OEFile_Impl.cpp
*	author:		daishuidi

*	purpose:	
*/
#include "OEFile_Impl.h"

COEFile_Impl::COEFile_Impl(const tstring& strFileName, uint nFlag)
{
	Init();
	m_bOK = Open(strFileName, nFlag);
}

COEFile_Impl::~COEFile_Impl()
{
	Destroy();
}

void COEFile_Impl::Init()
{
	m_nFlag = 0;
	m_hFile = NULL;
}

void COEFile_Impl::Destroy()
{
	COEOS::FileClose(m_hFile);
	m_hFile = NULL;
}

uint COEFile_Impl::Read(void* pBuffOut, uint nSize)
{
	return COEOS::FileRead(pBuffOut, nSize, m_hFile);
}

uint COEFile_Impl::Write(const void* pBuffIn, uint nSize)
{
	if (m_nFlag & OFF_WRITE)
	{
		return COEOS::FileWrite(pBuffIn, nSize, m_hFile);
	}

	return 0;
}

uint COEFile_Impl::Seek(uint nOffset)
{
	COEOS::FileSeek(m_hFile, nOffset, SEEK_SET);
	return COEOS::FileTell(m_hFile);
}

uint COEFile_Impl::GetSize() const
{
	uint nBackupPos = COEOS::FileTell(m_hFile);
	COEOS::FileSeek(m_hFile, 0, SEEK_END);
	uint nSize = COEOS::FileTell(m_hFile);
	COEOS::FileSeek(m_hFile, nBackupPos, SEEK_SET);

	return nSize;
}

uint COEFile_Impl::Tell() const
{
	return COEOS::FileTell(m_hFile);
}

bool COEFile_Impl::Open(const tstring& strFileName, uint nFlag)
{
	m_nFlag = nFlag;

	if (OFF_READ == m_nFlag)
	{
		m_hFile = COEOS::FileOpen(strFileName.c_str(), t("rb"));
	}
	else if (OFF_WRITE == m_nFlag)
	{
		m_hFile = COEOS::FileOpen(strFileName.c_str(), t("wb"));
	}
	else if ((OFF_READ|OFF_WRITE) == m_nFlag)
	{
		m_hFile = COEOS::FileOpen(strFileName.c_str(), t("rwb"));
	}

	if (!m_hFile) return false;

	return true;
}
