/*!
*	created:	2009/06/01
*	filename: 	OEFile_Impl.cpp
*	author:		daishuidi

*	purpose:	
*/
#include "OEFile_Impl.h"
#include <assert.h>

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
	m_pFile = NULL;
}

void COEFile_Impl::Destroy()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

uint COEFile_Impl::Read(void* pBuffOut, uint nSize)
{
	return (uint)fread(pBuffOut, 1, nSize, m_pFile);
}

uint COEFile_Impl::Write(const void* pBuffIn, uint nSize)
{
	if (m_nFlag & OFF_WRITE) return (uint)fwrite(pBuffIn, 1, nSize, m_pFile);
	return 0;
}

uint COEFile_Impl::Seek(uint nOffset)
{
	return fseek(m_pFile, nOffset, SEEK_SET);
}

uint COEFile_Impl::GetSize() const
{
	uint nResult = 0;
	uint nPos = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_END);
	nResult = ftell(m_pFile);
	fseek(m_pFile, nPos, SEEK_SET);

	return nResult;
}

uint COEFile_Impl::Tell() const
{
	return ftell(m_pFile);
}

bool COEFile_Impl::Open(const tstring& strFileName, uint nFlag)
{
	m_nFlag = nFlag;

	if (OFF_READ == m_nFlag)
	{
		m_pFile = _tfopen(strFileName.c_str(), _T("rb"));
	}
	else if (OFF_WRITE == m_nFlag)
	{
		m_pFile = _tfopen(strFileName.c_str(), _T("wb"));
	}
	else if (OFF_READ|OFF_WRITE == m_nFlag)
	{
		m_pFile = _tfopen(strFileName.c_str(), _T("rwb"));
	}

	if (!m_pFile) return false;

	return true;
}
