/*!
 * \file IOEMsg.cpp
 * \date 8-17-2010 20:07:21
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEBase/IOEMsg.h>

IOEMsg::IOEMsg(uint nMsgID)
{
	m_nMsgID = nMsgID;
}

IOEMsg::IOEMsg(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_nMsgID, sizeof(m_nMsgID));
}

IOEMsg::~IOEMsg()
{
	// TODO: 
}

uint IOEMsg::GetMsgID() const
{
	return m_nMsgID;
}

bool IOEMsg::ConvertToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_nMsgID, sizeof(m_nMsgID));
	return ToBuffer(pDBWrite);
}
