/*!
*	created:	2009/06/01
*	filename: 	OEFile_Impl.cpp
*	author:		daishuidi

*	purpose:	
*/

#include "OEFile_Impl.h"
#include <assert.h>
class COEFileMgr_Impl;
COEFile_Impl::COEFile_Impl(const tchar* pstrFileName, uint nFlag)
{
	m_nFlag = nFlag;
	m_bOK = Open(pstrFileName, nFlag);
}

COEFile_Impl::~COEFile_Impl()
{
	fclose(m_pFile);
	m_pFile = NULL;
	m_nFlag = 0;
}

uint COEFile_Impl::Read( void* pBuffOut, uint nSize )
{
	return  fread(pBuffOut, 1, nSize, m_pFile);
}

uint COEFile_Impl::Write( const void* pBuffIn, uint nSize )
{
	return fwrite(pBuffIn, 1, nSize, m_pFile);
}

uint COEFile_Impl::Seek( uint nOffset )
{
	return fseek(m_pFile, nOffset, SEEK_SET);
}

uint COEFile_Impl::GetSize() const
{
	uint nResult = 0;
	uint nPos = ftell(m_pFile);
	fseek(m_pFile,0,SEEK_END);
	nResult = ftell(m_pFile);
	fseek(m_pFile,nPos,SEEK_SET);

	return nResult;
}

long COEFile_Impl::Tell() const
{
	return ftell(m_pFile);
}

bool COEFile_Impl::Open( const tchar* pstrFileName, uint nFlag )
{
	assert(pstrFileName);
	bool bResult = false;

	if (OFF_READ == nFlag)
	{
		m_pFile = _tfopen(pstrFileName, _T("rb"));
	}
	else if (OFF_WRITE == nFlag)
	{
		m_pFile = _tfopen(pstrFileName, _T("wb"));
	}
	if (m_pFile)
	{
		bResult = true;
	}

	return bResult;
}