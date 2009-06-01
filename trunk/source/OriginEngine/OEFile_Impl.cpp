/*!
*	created:	2009/06/01
*	filename: 	OEFile_Impl.cpp
*	author:		daishuidi
	
*	purpose:	
*/

#include "OEFile_Impl.h"
#include <assert.h>
class COEFileMgr_Impl;
COEFile_Impl::COEFile_Impl( FILE* pFile, uint nFlag )
{
	assert(pFile);
	m_pFile = pFile;
	m_nFlag = nFlag;
}

COEFile_Impl::~COEFile_Impl()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		m_nFlag = 0;
	}
}

uint COEFile_Impl::Read( void* pBuffOut, uint nSize )
{
	uint nResult = 0;
	if (OFF_READ == m_nFlag && m_pFile)
	{
		nResult = fread(pBuffOut,sizeof(uchar),nSize,m_pFile);
	}
	return nResult;
}

uint COEFile_Impl::Write( const void* pBuffIn, uint nSize )
{
	uint nResult = 0;
	if (OFF_WRITE == m_nFlag && m_pFile)
	{
		nResult = fwrite(pBuffIn,sizeof(uchar), nSize,m_pFile);
	}
	return nResult;
}

uint COEFile_Impl::Seek( uint nOffset )
{
	uint nResult = 0;
	if (m_pFile)
	{
		nResult = fseek(m_pFile, nOffset, SEEK_SET);
	}
	return nResult;
}

uint COEFile_Impl::GetSize() const
{
	uint nResult = 0;
	if (m_pFile)
	{
		uint nPos = ftell(m_pFile);
		fseek(m_pFile,0,SEEK_END);
		nResult = ftell(m_pFile);
		fseek(m_pFile,nPos,SEEK_SET);
	}
	return nResult;
}

long COEFile_Impl::Tell() const
{
	uint nResult = 0;
	if (m_pFile)
	{
		nResult = ftell(m_pFile);
	}
	return nResult;
}