/*!
 * \file OEMessageMgr_Impl.cpp
 * \date 10-12-2009 17:19:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMessageMgr_Impl.h"

COEMessageMgr_Impl::COEMessageMgr_Impl()
{
	g_pOEMessageMgr = this;
	Init();
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
	// TODO: 
	return false;
}

bool COEMessageMgr_Impl::RegisterMessage(uint nMsgID, IOEObject* pHandler, FUNC_MESSAGE pFunc)
{
	// TODO: 
	return false;
}

bool COEMessageMgr_Impl::UnregisterMessage(uint nMsgID, IOEObject* pHandler)
{
	// TODO: 
	return false;
}
