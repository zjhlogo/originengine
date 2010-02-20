/*!
 * \file OEDataBufferWrite.h
 * \date 10-12-2009 17:06:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEDATABUFFERWRITE_H__
#define __OEDATABUFFERWRITE_H__

#include "OEBaseType.h"
#include "OEDataBufferRead.h"
#include <vector>

class COEDataBufferWrite
{
public:
	enum CONST_DEFINE
	{
		EXPAND_SIZE = 128,
	};

	typedef std::vector<uchar> TV_DATA_BUFFER;

public:
	COEDataBufferWrite();
	~COEDataBufferWrite();

	/*!
	 * \brief 读取指定缓冲区数据并写入自身缓冲区中
	 * \param pBuffer
	 * \param nSize
	 * \return 
	 */
	bool Write(const void* pBuffer, uint nSize);

	/*!
	 * \brief 从 data buffer read 中读取指定大学数据并写入自身缓冲区中
	 * \param pDBRead
	 * \param nSize
	 * \return 
	 */
	bool Write(COEDataBufferRead* pDBRead, uint nSize);
	void Reset();

	const void* GetBuffer();
	uint GetSize();

private:
	TV_DATA_BUFFER m_vBuffer;
	uint m_nCurrPos;

};
#endif // __OEDATABUFFERWRITE_H__
