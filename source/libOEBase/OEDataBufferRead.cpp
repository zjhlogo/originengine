/*!
 * \file OEDataBufferRead.cpp
 * \date 10-12-2009 17:06:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEDataBufferRead.h>
#include <assert.h>

COEDataBufferRead::COEDataBufferRead(const void* pBuffer, uint nSize)
{
	m_pBuffer = pBuffer;
	m_nSize = nSize;
	m_nCurrPos = 0;
}

COEDataBufferRead::~COEDataBufferRead()
{
	// TODO: 
}

bool COEDataBufferRead::Read(void* pDataOut, uint nSize)
{
	if (m_nCurrPos + nSize > m_nSize)
	{
		assert(false);
		return false;
	}

	const uchar* pDataBytes = reinterpret_cast<const uchar*>(m_pBuffer);
	memcpy(pDataOut, &pDataBytes[m_nCurrPos], nSize);
	m_nCurrPos += nSize;
	return true;
}

void COEDataBufferRead::Reset()
{
	m_nCurrPos = 0;
}
