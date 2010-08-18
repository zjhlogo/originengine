/*!
 * \file OEMsgCommand.cpp
 * \date 8-17-2010 21:21:19
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEBase/OEMsgCommand.h>

COEMsgCommand::COEMsgCommand(uint nMsgID)
:IOEMsg(nMsgID)
{
	// TODO: 
}

COEMsgCommand::COEMsgCommand(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	// TODO: 
}

COEMsgCommand::~COEMsgCommand()
{
	// TODO: 
}

bool COEMsgCommand::FromBuffer(COEDataBufferRead* pDBRead)
{
	// TODO: 
	return true;
}

bool COEMsgCommand::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	// TODO: 
	return true;
}
