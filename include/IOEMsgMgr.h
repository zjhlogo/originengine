/*!
 * \file IOEMsgMgr.h
 * \date 11-25-2009 14:09:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMSGMGR_H__
#define __IOEMSGMGR_H__

#include "OEBaseType.h"
#include "IOEObject.h"
#include "OEMsg.h"

class IOEMsgMgr : public IOEObject
{
public:
	IOEMsgMgr(){};
	virtual ~IOEMsgMgr(){};

	virtual bool SendMessage(COEMsg* pMsg) = 0;
	virtual void DispatchMessage() = 0;

	virtual bool InvokeMessage(COEMsg* pMsg) = 0;

	virtual bool RegisterMessage(uint nMsgID, IOEObject* pHandler, MSG_FUNC pFunc) = 0;
	virtual bool UnregisterMessage(uint nMsgID, IOEObject* pHandler) = 0;
};

extern IOEMsgMgr* g_pOEMsgMgr;

#endif // __IOEMSGMGR_H__
