/*!
 * \file OEMessage.cpp
 * \date 10-12-2009 17:12:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEMessage.h>

COEMessage::COEMessage(uint nMsgID)
{
	m_nMsgID = nMsgID;
	m_pDBRead = NULL;
	m_pDBWrite = NULL;
}

COEMessage::COEMessage(COEDataBufferRead* pDBRead)
{
	m_nMsgID = OMI_UNKNOWN;
	m_pDBRead = pDBRead;
	m_pDBWrite = NULL;

	Read(m_nMsgID);
	UnpackData();

	m_pDBRead = NULL;
}

COEMessage::~COEMessage()
{
	// TODO: 
}

uint COEMessage::GetMsgID()
{
	return m_nMsgID;
}

bool COEMessage::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	m_pDBWrite = pDBWrite;

	if (!Write(m_nMsgID))
	{
		m_pDBWrite = NULL;
		return false;
	}

	if (!PackData())
	{
		m_pDBWrite = NULL;
		return false;
	}

	m_pDBWrite = NULL;
	return true;
}

bool COEMessage::PackData()
{
	return true;
}

bool COEMessage::UnpackData()
{
	return true;
}

bool COEMessage::ReadString(tstring& strOut)
{
	uint nSize = 0;
	if (!m_pDBRead->Read(&nSize, sizeof(nSize)))
	{
		assert(false);
		return false;
	}

	if (nSize % sizeof(tchar) != 0)
	{
		assert(false);
		return false;
	}

	uint nLength = nSize / sizeof(tchar);

	if (nLength <= 0)
	{
		strOut.clear();
		return true;
	}

	if (strOut.length() < nLength+1) strOut.resize(nLength+1);
	if (!m_pDBRead->Read(&strOut[0], nSize))
	{
		assert(false);
		return false;
	}

	strOut[nLength] = t('\0');
	return true;
}

bool COEMessage::ReadBuffer(void* pBuffer, uint nSize)
{
	if (!m_pDBRead->Read(pBuffer, nSize))
	{
		assert(false);
		return false;
	}

	return true;
}

bool COEMessage::WriteString(const tstring& strIn)
{
	uint nSize = strIn.length()*sizeof(tchar);
	if (!m_pDBWrite->Write(&nSize, sizeof(nSize)))
	{
		assert(false);
		return false;
	}

	if (nSize <= 0) return true;
	if (!m_pDBWrite->Write(strIn.c_str(), nSize))
	{
		assert(false);
		return false;
	}

	return true;
}

bool COEMessage::WriteBuffer(const void* pBuffer, uint nSize)
{
	if (!m_pDBWrite->Write(pBuffer, nSize))
	{
		assert(false);
		return false;
	}

	return true;
}
