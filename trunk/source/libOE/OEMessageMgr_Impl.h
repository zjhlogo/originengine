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

class COEMessageMgr_Impl : public IOEMessageMgr
{
public:
	COEMessageMgr_Impl();
	virtual ~COEMessageMgr_Impl();

	virtual bool SendMessage(COEMessage* pMsg);
	virtual bool RegisterMessage(uint nMsgID, IOEObject* pHandler, FUNC_MESSAGE pFunc);
	virtual bool UnregisterMessage(uint nMsgID, IOEObject* pHandler);

private:
	void Init();
	void Destroy();

};
#endif // __OEMESSAGEMGR_IMPL_H__
