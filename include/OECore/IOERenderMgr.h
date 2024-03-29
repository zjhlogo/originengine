/*!
 * \file IOERenderMgr.h
 * \date 13-2-2010 12:28:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDERMGR_H__
#define __IOERENDERMGR_H__

#include "IOEMgr.h"
#include "IOERender.h"

class IOERenderMgr : public IOEMgr
{
public:
	RTTI_DEF(IOERenderMgr, IOEMgr);

	IOERenderMgr() {};
	virtual ~IOERenderMgr() {};

	virtual bool AddRenderHandle(IOERender* pRender) = 0;
	virtual IOERender* GetRender(const tstring& strClassName) = 0;

};

extern IOERenderMgr* g_pOERenderMgr;

#endif // __IOERENDERMGR_H__
