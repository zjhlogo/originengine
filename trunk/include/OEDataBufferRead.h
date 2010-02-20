/*!
 * \file OEDataBufferRead.h
 * \date 10-12-2009 17:05:29
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEDATABUFFERREAD_H__
#define __OEDATABUFFERREAD_H__

#include "OEBaseType.h"

class COEDataBufferRead
{
public:
	COEDataBufferRead(const void* pBuffer, uint nSize);
	~COEDataBufferRead();

	bool Read(void* pDataOut, uint nSize);
	void Reset();

private:
	const void* m_pBuffer;
	uint m_nSize;
	uint m_nCurrPos;

};
#endif // __OEDATABUFFERREAD_H__
