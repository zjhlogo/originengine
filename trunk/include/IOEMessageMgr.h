/*!
 * \file IOEMessageMgr.h
 * \date 10-12-2009 17:15:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMESSAGEMGR_H__
#define __IOEMESSAGEMGR_H__

#include "OEBasicType.h"
#include "IOEObject.h"
#include "OEMessage.h"

class IOEMessageMgr : public IOEObject
{
public:
	IOEMessageMgr(){};
	virtual ~IOEMessageMgr(){};

	virtual bool SendMessage(COEMessage* pMsg) = 0;
	virtual void DispatchRecvMessage() = 0;

	virtual bool RegisterMessage(uint nMsgID, IOEObject* pHandler, FUNC_MESSAGE pFunc) = 0;
	virtual bool UnregisterMessage(uint nMsgID, IOEObject* pHandler) = 0;
};

extern IOEMessageMgr* g_pOEMessageMgr;

#endif // __IOEMESSAGEMGR_H__
