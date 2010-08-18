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
#include <libOEBase/IOEObject.h>

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
	typedef std::map<uint, MSG_GENERATE_FUNC> TM_MSG_GENERATE_FUNC;

public:
	COEMsgMgr_Impl();
	virtual ~COEMsgMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual bool AddMsgDB(const MSG_GENERATE_MAP* pMsgDB);

	virtual bool SendMessage(IOEMsg* pMsg);
	virtual bool ReceiveMessage();

	virtual bool InvokeMessage(IOEMsg* pMsg);
	virtual bool RegisterMessage(uint nMsgID, IOEObject* pHandler, MSG_FUNC pFunc);
	virtual bool UnregisterMessage(uint nMsgID, IOEObject* pHandler);
	virtual bool UnregisterMessage(IOEObject* pHandler);

private:
	bool Init();
	void Destroy();

	bool ProcessMessage(uint nMsgID, COEDataBufferRead* pDBRead);

private:
	TM_MSG_HANDLER_INFO m_MsgHandlerInfoMap;
	TQ_DATA_BUFFER m_SendMsgList;
	TM_MSG_GENERATE_FUNC m_MsgGenFuncMap;

};
#endif // __OEMSGMGR_IMPL_H__
