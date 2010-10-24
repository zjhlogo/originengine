/*!
 * \file OEMsgObjectAttach.cpp
 * \date 10-24-2010 8:31:30
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgObjectAttach.h>
#include <libOEMsg/OEMsgList.h>

COEMsgObjectAttach::COEMsgObjectAttach(IOENode* pNode)
:IOEMsg(OMI_OBJECT_ATTACH)
{
	m_pNode = pNode;
}

COEMsgObjectAttach::COEMsgObjectAttach(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	FromBuffer(pDBRead);
}

COEMsgObjectAttach::~COEMsgObjectAttach()
{
	// TODO: 
}

IOENode* COEMsgObjectAttach::GetNode()
{
	return m_pNode;
}

bool COEMsgObjectAttach::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_pNode, sizeof(m_pNode));
	return true;
}

bool COEMsgObjectAttach::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_pNode, sizeof(m_pNode));
	return true;
}
