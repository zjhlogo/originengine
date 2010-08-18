/*!
 * \file OEMsgMouse.cpp
 * \date 8-17-2010 19:30:13
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgMouse.h>

COEMsgMouse::COEMsgMouse(uint nMsgID)
:IOEMsg(nMsgID)
{
	m_nX = 0;
	m_nY = 0;
	m_nWheel = 0;
}

COEMsgMouse::COEMsgMouse(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	FromBuffer(pDBRead);
}

COEMsgMouse::~COEMsgMouse()
{
	// TODO: 
}

void COEMsgMouse::SetPos(int nX, int nY)
{
	m_nX = nX;
	m_nY = nY;
}

int COEMsgMouse::GetPosX() const
{
	return m_nX;
}

int COEMsgMouse::GetPosY() const
{
	return m_nY;
}

void COEMsgMouse::SetWheel(int nWheel)
{
	m_nWheel = nWheel;
}

int COEMsgMouse::GetWheel() const
{
	return m_nWheel;
}

bool COEMsgMouse::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_nX, sizeof(m_nX));
	pDBRead->Read(&m_nY, sizeof(m_nY));
	pDBRead->Read(&m_nWheel, sizeof(m_nWheel));
	return true;
}

bool COEMsgMouse::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_nX, sizeof(m_nX));
	pDBWrite->Write(&m_nY, sizeof(m_nY));
	pDBWrite->Write(&m_nWheel, sizeof(m_nWheel));
	return true;
}
