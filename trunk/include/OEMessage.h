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

class COEMessage
{
public:
	COEMessage(uint nMsgID);
	COEMessage(COEDataBufferRead* pDBRead);
	~COEMessage();

	uint GetMsgID();

private:
	uint m_nMsgID;

};
#endif // __OEMESSAGE_H__
