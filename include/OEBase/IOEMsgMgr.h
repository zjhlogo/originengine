/*!
 * \file IOEMsgMgr.h
 * \date 11-25-2009 14:09:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMSGMGR_H__
#define __IOEMSGMGR_H__

#include "../OECore/IOEMgr.h"
#include "../libOEBase/IOEMsg.h"

class IOEMsgMgr : public IOEMgr
{
public:
	typedef IOEMsg* (*MSG_GENERATE_FUNC)(COEDataBufferRead* pDBRead);

	typedef struct MSG_GENERATE_MAP_tag
	{
		uint nMsgID;
		MSG_GENERATE_FUNC pFunc;
	} MSG_GENERATE_MAP;

public:
	RTTI_DEF(IOEMsgMgr, IOEMgr);

	IOEMsgMgr() {};
	virtual ~IOEMsgMgr() {};

	virtual bool AddMsgDB(const MSG_GENERATE_MAP* pMsgDB) = 0;

	virtual bool SendMessage(IOEMsg* pMsg) = 0;
	virtual bool ReceiveMessage() = 0;

	virtual bool InvokeMessage(IOEMsg* pMsg) = 0;

	virtual bool RegisterMessage(uint nMsgID, IOEObject* pHandler, MSG_FUNC pFunc) = 0;
	virtual bool UnregisterMessage(uint nMsgID, IOEObject* pHandler) = 0;
	virtual bool UnregisterMessage(IOEObject* pHandler) = 0;
};

extern IOEMsgMgr* g_pOEMsgMgr;

#endif // __IOEMSGMGR_H__
