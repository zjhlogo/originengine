/*!
 * \file IOEMsg.h
 * \date 8-17-2010 20:05:23
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOEMSG_H__
#define __IOEMSG_H__

#include "OEBaseType.h"
#include "OEDataBufferRead.h"
#include "OEDataBufferWrite.h"

class IOEMsg
{
public:
	IOEMsg(uint nMsgID);
	IOEMsg(COEDataBufferRead* pDBRead);
	virtual ~IOEMsg();

	uint GetMsgID() const;
	bool ConvertToBuffer(COEDataBufferWrite* pDBWrite);

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead) = 0;
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite) = 0;

private:
	uint m_nMsgID;

};

#endif // __IOEMSG_H__
