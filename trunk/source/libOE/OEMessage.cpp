/*!
 * \file OEMessage.cpp
 * \date 10-12-2009 17:12:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEMessage.h>
#include <OEMessageID.h>

COEMessage::COEMessage(uint nMsgID)
{
	m_eOpMode = OM_WRITE;
	m_nMsgID = nMsgID;
	m_pDBRead = NULL;
	m_pDBWrite = &m_DBWrite;
	m_pNewDBRead = NULL;
}

COEMessage::COEMessage(COEDataBufferRead* pDBRead)
{
	m_eOpMode = OM_READ;
	m_nMsgID = OMI_UNKNOWN;
	m_pDBRead = pDBRead;
	m_pDBWrite = NULL;
	m_pNewDBRead = NULL;

	Read(m_nMsgID);
	ReadExtendData();
	UnpackData();

	if (m_DBWrite.GetSize() > 0)
	{
		m_pNewDBRead = new COEDataBufferRead(m_DBWrite.GetBuffer(), m_DBWrite.GetSize());
	}

	m_pDBRead = m_pNewDBRead;
}

COEMessage::~COEMessage()
{
	SAFE_DELETE(m_pNewDBRead);
}

uint COEMessage::GetMsgID()
{
	return m_nMsgID;
}

bool COEMessage::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	m_pDBWrite = pDBWrite;

	bool bOK = true;
	if (bOK) bOK = Write(m_nMsgID);
	if (bOK) bOK = WriteExtendData();
	if (bOK) bOK = PackData();

	m_pDBWrite = NULL;
	m_eOpMode = OM_UNKNOWN;
	return bOK;
}

bool COEMessage::ReadString(tstring& strOut)
{
	if (m_eOpMode != OM_READ) return false;

	if (!m_pDBRead)
	{
		assert(false);
		return false;
	}

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
	if (m_eOpMode != OM_READ) return false;

	if (!m_pDBRead)
	{
		assert(false);
		return false;
	}

	if (!m_pDBRead->Read(pBuffer, nSize))
	{
		assert(false);
		return false;
	}

	return true;
}

bool COEMessage::WriteString(const tstring& strIn)
{
	if (m_eOpMode != OM_WRITE) return false;

	if (!m_pDBWrite)
	{
		assert(false);
		return false;
	}

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
	if (m_eOpMode != OM_WRITE) return false;

	if (!m_pDBWrite)
	{
		assert(false);
		return false;
	}

	if (!m_pDBWrite->Write(pBuffer, nSize))
	{
		assert(false);
		return false;
	}

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

bool COEMessage::ReadExtendData()
{
	uint nExtendDataSize = 0;
	if (!m_pDBRead->Read(&nExtendDataSize, sizeof(nExtendDataSize))) return false;

	if (nExtendDataSize <= 0) return true;

	return m_DBWrite.Write(m_pDBRead, nExtendDataSize);
}

bool COEMessage::WriteExtendData()
{
	uint nExtendDataSize = m_DBWrite.GetSize();
	if (!m_pDBWrite->Write(&nExtendDataSize, sizeof(nExtendDataSize))) return false;

	if (nExtendDataSize <= 0) return true;

	return m_pDBWrite->Write(m_DBWrite.GetBuffer(), nExtendDataSize);
}
