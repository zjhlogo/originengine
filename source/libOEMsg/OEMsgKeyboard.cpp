/*!
 * \file OEMsgKeyboard.cpp
 * \date 8-17-2010 19:47:04
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgKeyboard.h>

COEMsgKeyboard::COEMsgKeyboard(uint nMsgID)
:IOEMsg(nMsgID)
{
	m_nKeyCode = 0;
}

COEMsgKeyboard::COEMsgKeyboard(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	FromBuffer(pDBRead);
}

COEMsgKeyboard::~COEMsgKeyboard()
{
	// TODO: 
}

void COEMsgKeyboard::SetKeyCode(uint nKeyCode)
{
	m_nKeyCode = nKeyCode;
}

uint COEMsgKeyboard::GetKeyCode() const
{
	return m_nKeyCode;
}

bool COEMsgKeyboard::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_nKeyCode, sizeof(m_nKeyCode));
	return true;
}

bool COEMsgKeyboard::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_nKeyCode, sizeof(m_nKeyCode));
	return true;
}
