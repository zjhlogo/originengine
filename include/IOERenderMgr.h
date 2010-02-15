/*!
 * \file IOERenderMgr.h
 * \date 13-2-2010 12:28:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDERMGR_H__
#define __IOERENDERMGR_H__

#include "IOEObject.h"
#include "IOERender.h"

class IOERenderMgr : public IOEObject
{
public:
	IOERenderMgr() {};
	virtual ~IOERenderMgr() {};

	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

	virtual bool AddRenderHandle(IOERender* pRender) = 0;
	virtual IOERender* GetRender(uint nID) = 0;

};

extern IOERenderMgr* g_pOERenderMgr;

#endif // __IOERENDERMGR_H__
