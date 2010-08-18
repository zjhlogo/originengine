/*!
 * \file OEMsgCommand.h
 * \date 8-17-2010 21:21:32
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGCOMMAND_H__
#define __OEMSGCOMMAND_H__

#include "IOEMsg.h"

class COEMsgCommand : public IOEMsg
{
public:
	COEMsgCommand(uint nMsgID);
	COEMsgCommand(COEDataBufferRead* pDBRead);
	virtual ~COEMsgCommand();

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

};
#endif // __OEMSGCOMMAND_H__
