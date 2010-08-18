/*!
 * \file OEMessageMgr_Impl.cpp
 * \date 10-12-2009 17:19:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMsgMgr_Impl.h"

#include <libOEBase/OEMsgID.h>
#include <libOEBase/OEDataBufferRead.h>
#include <libOEBase/OEDataBufferWrite.h>
#include <assert.h>

COEMsgMgr_Impl::COEMsgMgr_Impl()
{
	assert(!g_pOEMsgMgr);
	g_pOEMsgMgr = this;
	m_bOK = Init();
}

COEMsgMgr_Impl::~COEMsgMgr_Impl()
{
	Destroy();
	g_pOEMsgMgr = NULL;
}

bool COEMsgMgr_Impl::Init()
{
	// TODO: 
	return true;
}

void COEMsgMgr_Impl::Destroy()
{
	// TODO: 
}

bool COEMsgMgr_Impl::Initialize()
{
	// TODO: 
	return true;
}

void COEMsgMgr_Impl::Terminate()
{
	// TODO: 
}

bool COEMsgMgr_Impl::SendMessage(IOEMsg* pMsg)
{
	// TODO: thread lock

	// alloc a new buffer and write message buffer
	COEDataBufferWrite* pDBWrite = new COEDataBufferWrite();
	if (!pMsg->ConvertToBuffer(pDBWrite))
	{
		delete pDBWrite;
		return false;
	}

	// push it into a queue
	m_SendMsgList.push(pDBWrite);

	// TODO: thread unlock
	return true;
}

bool COEMsgMgr_Impl::ReceiveMessage()
{
	// TODO: thread lock

	while (m_SendMsgList.size() > 0)		// TODO: receive data from net
	{
		COEDataBufferWrite* pDBWrite = m_SendMsgList.front();
		m_SendMsgList.pop();

		COEDataBufferRead dbRead(pDBWrite->GetBuffer(), pDBWrite->GetSize());

		uint nMsgID = OMI_UNKNOWN;
		dbRead.Read(&nMsgID, sizeof(nMsgID));
		dbRead.Reset();

		ProcessMessage(nMsgID, &dbRead);

		delete pDBWrite;
	}

	// TODO: thread unlock
	return true;
}

bool COEMsgMgr_Impl::InvokeMessage(IOEMsg* pMsg)
{
	TP_MSG_HANDLER_INFO Range = m_MsgHandlerInfoMap.equal_range(pMsg->GetMsgID());
	for (TM_MSG_HANDLER_INFO::iterator it = Range.first; it != Range.second; ++it)
	{
		MSG_HANDLER_INFO& msgHandlerInfo = it->second;

		// check the loop depth, it must always 0 or 1
		if (msgHandlerInfo.nLoopDepth > 0)
		{
			assert(false);
			continue;
		}
		++msgHandlerInfo.nLoopDepth;

		if (!(msgHandlerInfo.pHandler->*msgHandlerInfo.pFunc)(*pMsg))
		{
			assert(false);
		}

		--msgHandlerInfo.nLoopDepth;
	}

	return true;
}

bool COEMsgMgr_Impl::RegisterMessage(uint nMsgID, IOEObject* pHandler, MSG_FUNC pFunc)
{
	MSG_HANDLER_INFO msgHandlerInfo;
	msgHandlerInfo.pHandler = pHandler;
	msgHandlerInfo.pFunc = pFunc;
	msgHandlerInfo.nLoopDepth = 0;
	m_MsgHandlerInfoMap.insert(std::make_pair(nMsgID, msgHandlerInfo));
	return true;
}

bool COEMsgMgr_Impl::UnregisterMessage(uint nMsgID, IOEObject* pHandler)
{
	// TODO: 
	return false;
}

bool COEMsgMgr_Impl::UnregisterMessage(IOEObject* pHandler)
{
	// TODO: 
	return false;
}

void COEMsgMgr_Impl::ProcessMessage(uint nMsgID, COEDataBufferRead* pDBRead)
{
	//TP_MSG_HANDLER_INFO Range = m_MsgHandlerInfoMap.equal_range(nMsgID);
	//for (TM_MSG_HANDLER_INFO::iterator it = Range.first; it != Range.second; ++it)
	//{
	//	MSG_HANDLER_INFO& msgHandlerInfo = it->second;

	//	// check the loop depth, it must always 0 or 1
	//	if (msgHandlerInfo.nLoopDepth > 0)
	//	{
	//		assert(false);
	//		continue;
	//	}
	//	++msgHandlerInfo.nLoopDepth;

	//	pDBRead->Reset();
	//	if (!(msgHandlerInfo.pHandler->*msgHandlerInfo.pFunc)(nMsgID, pDBRead))
	//	{
	//		assert(false);
	//	}

	//	--msgHandlerInfo.nLoopDepth;
	//}
}
