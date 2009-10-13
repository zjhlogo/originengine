/*!
 * \file OEMessageMgr_Impl.cpp
 * \date 10-12-2009 17:19:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMessageMgr_Impl.h"

#include <OEMessageID.h>
#include <OEDataBufferRead.h>
#include <OEDataBufferWrite.h>

COEMessageMgr_Impl::COEMessageMgr_Impl()
{
	g_pOEMessageMgr = this;
	Init();
	m_bOK = true;
}

COEMessageMgr_Impl::~COEMessageMgr_Impl()
{
	Destroy();
	g_pOEMessageMgr = NULL;
}

void COEMessageMgr_Impl::Init()
{
	// TODO: 
}

void COEMessageMgr_Impl::Destroy()
{
	// TODO: 
}

bool COEMessageMgr_Impl::SendMessage(COEMessage* pMsg)
{
	// TODO: thread lock

	// alloc a new buffer and write message buffer
	COEDataBufferWrite* pDBWrite = new COEDataBufferWrite();
	if (!pMsg->ToBuffer(pDBWrite))
	{
		delete pDBWrite;
		return false;
	}

	// push it into a queue
	m_SendMsgList.push(pDBWrite);

	// TODO: thread unlock
	return true;
}

void COEMessageMgr_Impl::DispatchRecvMessage()
{
	// TODO: thread lock

	while (m_SendMsgList.size() > 0)
	{
		COEDataBufferWrite* pDBWrite = m_SendMsgList.front();
		m_SendMsgList.pop();

		COEDataBufferRead DBRead(pDBWrite->GetBuffer(), pDBWrite->GetSize());

		uint nMsgID = OMI_UNKNOWN;
		DBRead.Read(&nMsgID, sizeof(nMsgID));

		ProcessReceive(nMsgID, &DBRead);

		delete pDBWrite;
	}

	// TODO: thread unlock
}

bool COEMessageMgr_Impl::RegisterMessage(uint nMsgID, IOEObject* pHandler, FUNC_MESSAGE pFunc)
{
	MSGHANDLER_INFO MsgHandlerInfo;
	MsgHandlerInfo.pHandler = pHandler;
	MsgHandlerInfo.pFunc = pFunc;
	m_MsgHandlerInfoMap.insert(std::make_pair(nMsgID, MsgHandlerInfo));
	return true;
}

bool COEMessageMgr_Impl::UnregisterMessage(uint nMsgID, IOEObject* pHandler)
{
	// TODO: 
	return false;
}

void COEMessageMgr_Impl::ProcessReceive(uint nMsgID, COEDataBufferRead* pDBRead)
{
	TP_MSGHANDLER_INFO Range = m_MsgHandlerInfoMap.equal_range(nMsgID);
	for (TM_MSGHANDLER_INFO::iterator it = Range.first; it != Range.second; ++it)
	{
		const MSGHANDLER_INFO& MsgHandlerInfo = it->second;

		pDBRead->Reset();
		if (!(MsgHandlerInfo.pHandler->*MsgHandlerInfo.pFunc)(nMsgID, pDBRead))
		{
			assert(false);
		}
	}
}
