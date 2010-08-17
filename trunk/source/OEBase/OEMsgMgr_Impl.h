/*!
 * \file OEMsgMgr_Impl.h
 * \date 11-25-2009 14:09:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMSGMGR_IMPL_H__
#define __OEMSGMGR_IMPL_H__

#include <OEBase/IOEMsgMgr.h>
#include <OECore/IOEObject.h>

#include <map>
#include <queue>

class COEMsgMgr_Impl : public IOEMsgMgr
{
public:
	typedef struct MSG_HANDLER_INFO_tag
	{
		IOEObject* pHandler;
		MSG_FUNC pFunc;
		int nLoopDepth;
	} MSG_HANDLER_INFO;

	typedef std::multimap<uint, MSG_HANDLER_INFO> TM_MSG_HANDLER_INFO;
	typedef std::pair<TM_MSG_HANDLER_INFO::iterator, TM_MSG_HANDLER_INFO::iterator> TP_MSG_HANDLER_INFO;

	typedef std::queue<COEDataBufferWrite*> TQ_DATA_BUFFER;

public:
	COEMsgMgr_Impl();
	virtual ~COEMsgMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual bool SendMessage(COEMsg* pMsg);
	virtual void DispatchMessage();

	virtual bool InvokeMessage(COEMsg* pMsg);

	virtual bool RegisterMessage(uint nMsgID, IOEObject* pHandler, MSG_FUNC pFunc);
	virtual bool UnregisterMessage(uint nMsgID, IOEObject* pHandler);

private:
	bool Init();
	void Destroy();

	void ProcessReceive(uint nMsgID, COEDataBufferRead* pDBRead);

private:
	TM_MSG_HANDLER_INFO m_MsgHandlerInfoMap;
	TQ_DATA_BUFFER m_SendMsgList;

};
#endif // __OEMSGMGR_IMPL_H__
