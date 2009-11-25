/*!
 * \file OEMsg.h
 * \date 11-25-2009 14:04:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMSG_H__
#define __OEMSG_H__

#include "OEBasicType.h"
#include "OEDataBufferRead.h"
#include "OEDataBufferWrite.h"
#include <assert.h>

class COEMsg
{
public:
	enum OPERATE_MODE
	{
		OM_UNKNOWN,
		OM_READ,
		OM_WRITE,
	};

public:
	COEMsg(uint nMsgID);
	COEMsg(COEDataBufferRead* pDBRead, bool bFromBuffer = true);
	virtual ~COEMsg();

	uint GetMsgID();
	bool ToBuffer(COEDataBufferWrite* pDBWrite);

	template <typename T> bool Read(T& DataType);
	bool ReadString(tstring& strOut);
	bool ReadBuffer(void* pBuffer, uint nSize);

	template <typename T> bool Write(const T& DataType);
	bool WriteString(const tstring& strIn);
	bool WriteBuffer(const void* pBuffer, uint nSize);

protected:
	bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool PackData();
	virtual bool UnpackData();

private:
	bool ReadExtendData();
	bool WriteExtendData();

private:
	OPERATE_MODE m_eOpMode;
	uint m_nMsgID;
	COEDataBufferRead* m_pDBRead;
	COEDataBufferWrite* m_pDBWrite;

	COEDataBufferRead* m_pNewDBRead;
	COEDataBufferWrite m_DBWrite;

};

template <typename T> bool COEMsg::Read(T& DataType)
{
	if (m_eOpMode != OM_READ) return false;

	if (!m_pDBRead)
	{
		assert(false);
		return false;
	}

	if (!m_pDBRead->Read(&DataType, sizeof(DataType)))
	{
		assert(false);
		return false;
	}

	return true;
}

template <typename T> bool COEMsg::Write(const T& DataType)
{
	if (m_eOpMode != OM_WRITE) return false;

	if (!m_pDBWrite)
	{
		assert(false);
		return false;
	}

	if (!m_pDBWrite->Write(&DataType, sizeof(DataType)))
	{
		assert(false);
		return false;
	}

	return true;
}
#endif // __OEMSG_H__
