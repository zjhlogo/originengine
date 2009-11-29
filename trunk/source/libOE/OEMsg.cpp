/*!
 * \file OEMsg.cpp
 * \date 11-25-2009 14:04:45
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEMsg.h>
#include <OEMsgID.h>

COEMsg::COEMsg(uint nMsgID)
{
	m_eOpMode = OM_WRITE;
	m_nMsgID = nMsgID;
	m_pDBRead = NULL;
	m_pDBWrite = &m_DBWrite;
	m_pNewDBRead = NULL;
}

COEMsg::COEMsg(COEDataBufferRead* pDBRead, bool bFromBuffer /* = true */)
{
	m_eOpMode = OM_READ;
	m_nMsgID = OMI_UNKNOWN;
	m_pDBRead = pDBRead;
	m_pDBWrite = NULL;
	m_pNewDBRead = NULL;

	if (bFromBuffer) FromBuffer(pDBRead);
}

COEMsg::~COEMsg()
{
	SAFE_DELETE(m_pNewDBRead);
}

uint COEMsg::GetMsgID()
{
	return m_nMsgID;
}

bool COEMsg::FromBuffer(COEDataBufferRead* pDBRead)
{
	Read(m_nMsgID);
	ReadExtendData();
	UnpackData();

	if (m_DBWrite.GetSize() > 0)
	{
		m_pNewDBRead = new COEDataBufferRead(m_DBWrite.GetBuffer(), m_DBWrite.GetSize());
	}

	m_pDBRead = m_pNewDBRead;

	return true;
}

bool COEMsg::ToBuffer(COEDataBufferWrite* pDBWrite)
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

bool COEMsg::ReadString(tstring& strOut)
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

	strOut[nLength] = TS('\0');
	return true;
}

bool COEMsg::ReadBuffer(void* pBuffer, uint nSize)
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

bool COEMsg::WriteString(const tstring& strIn)
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

bool COEMsg::WriteBuffer(const void* pBuffer, uint nSize)
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

bool COEMsg::PackData()
{
	return true;
}

bool COEMsg::UnpackData()
{
	return true;
}

bool COEMsg::ReadExtendData()
{
	uint nExtendDataSize = 0;
	if (!m_pDBRead->Read(&nExtendDataSize, sizeof(nExtendDataSize))) return false;

	if (nExtendDataSize <= 0) return true;

	return m_DBWrite.Write(m_pDBRead, nExtendDataSize);
}

bool COEMsg::WriteExtendData()
{
	uint nExtendDataSize = m_DBWrite.GetSize();
	if (!m_pDBWrite->Write(&nExtendDataSize, sizeof(nExtendDataSize))) return false;

	if (nExtendDataSize <= 0) return true;

	return m_pDBWrite->Write(m_DBWrite.GetBuffer(), nExtendDataSize);
}
