/*!
 * \file OEMessage.cpp
 * \date 10-12-2009 17:12:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEMessage.h>

COEMessage::COEMessage(uint nMsgID)
{
	m_nMsgID = nMsgID;
}

COEMessage::COEMessage(COEDataBufferRead* pDBRead)
{
	// TODO: 
}

COEMessage::~COEMessage()
{
	// TODO: 
}

uint COEMessage::GetMsgID()
{
	return m_nMsgID;
}
