/*!
 * \file OEDataBufferWrite.cpp
 * \date 10-12-2009 17:07:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEDataBufferWrite.h>
#include <assert.h>

COEDataBufferWrite::COEDataBufferWrite()
{
	m_nCurrPos = 0;
}

COEDataBufferWrite::~COEDataBufferWrite()
{
	// TODO: 
}

bool COEDataBufferWrite::Write(const void* pBuffer, uint nSize)
{
	if (m_nCurrPos + nSize > m_vBuffer.size())
	{
		int nNewSize = ((m_nCurrPos + nSize)/EXPAND_SIZE+1)*EXPAND_SIZE;
		m_vBuffer.resize(nNewSize);
	}

	memcpy(&m_vBuffer[m_nCurrPos], pBuffer, nSize);
	m_nCurrPos += nSize;
	return true;
}

void COEDataBufferWrite::Reset()
{
	m_nCurrPos = 0;
}

const void* COEDataBufferWrite::GetBuffer()
{
	if (m_vBuffer.empty()) return NULL;
	return &m_vBuffer[0];
}

uint COEDataBufferWrite::GetSize()
{
	return m_nCurrPos;
}
