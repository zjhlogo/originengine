/*!
 * \file OEMessage.h
 * \date 10-12-2009 17:12:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMESSAGE_H__
#define __OEMESSAGE_H__

#include "OEBasicType.h"
#include "OEDataBufferRead.h"
#include "OEDataBufferWrite.h"
#include <assert.h>

class COEMessage
{
public:
	COEMessage(uint nMsgID);
	COEMessage(COEDataBufferRead* pDBRead);
	~COEMessage();

	uint GetMsgID();
	bool ToBuffer(COEDataBufferWrite* pDBWrite);

protected:
	virtual bool PackData();
	virtual bool UnpackData();

	template <typename T> bool Read(T& DataType);
	bool ReadString(tstring& strOut);
	bool ReadBuffer(void* pBuffer, uint nSize);

	template <typename T> bool Write(const T& DataType);
	bool WriteString(const tstring& strIn);
	bool WriteBuffer(const void* pBuffer, uint nSize);

private:
	uint m_nMsgID;
	COEDataBufferRead* m_pDBRead;
	COEDataBufferWrite* m_pDBWrite;

};

template <typename T> bool COEMessage::Read(T& DataType)
{
	if (!m_pDBRead->Read(&DataType, sizeof(DataType)))
	{
		assert(false);
		return false;
	}

	return true;
}

template <typename T> bool COEMessage::Write(const T& DataType)
{
	if (!m_pDBWrite->Write(&DataType, sizeof(DataType)))
	{
		assert(false);
		return false;
	}

	return true;
}

#endif // __OEMESSAGE_H__
