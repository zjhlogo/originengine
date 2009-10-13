/*!
 * \file OEMessageMgr_Impl.h
 * \date 10-12-2009 17:18:44
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMESSAGEMGR_IMPL_H__
#define __OEMESSAGEMGR_IMPL_H__

#include <IOEMessageMgr.h>
#include <IOEObject.h>

#include <map>
#include <queue>

class COEMessageMgr_Impl : public IOEMessageMgr
{
public:
	typedef struct MSGHANDLER_INFO_tag
	{
		IOEObject* pHandler;
		FUNC_MESSAGE pFunc;
	} MSGHANDLER_INFO;

	typedef std::multimap<uint, MSGHANDLER_INFO> TM_MSGHANDLER_INFO;
	typedef std::pair<TM_MSGHANDLER_INFO::iterator, TM_MSGHANDLER_INFO::iterator> TP_MSGHANDLER_INFO;

	typedef std::queue<COEDataBufferWrite*> TQ_DATA_BUFFER;

public:
	COEMessageMgr_Impl();
	virtual ~COEMessageMgr_Impl();

	virtual bool SendMessage(COEMessage* pMsg);
	virtual void DispatchRecvMessage();

	virtual bool RegisterMessage(uint nMsgID, IOEObject* pHandler, FUNC_MESSAGE pFunc);
	virtual bool UnregisterMessage(uint nMsgID, IOEObject* pHandler);

private:
	void Init();
	void Destroy();

	void ProcessReceive(uint nMsgID, COEDataBufferRead* pDBRead);

private:
	TM_MSGHANDLER_INFO m_MsgHandlerInfoMap;
	TQ_DATA_BUFFER m_SendMsgList;

};
#endif // __OEMESSAGEMGR_IMPL_H__
