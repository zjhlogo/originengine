/*!
 * \file IOEControlMgr.h
 * \date 13-2-2010 12:25:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOECONTROLMGR_H__
#define __IOECONTROLMGR_H__

#include "IOEMgr.h"
#include "IOEControl.h"

class IOEControlMgr : public IOEMgr
{
public:
	RTTI_DEF(IOEControlMgr, IOEMgr);

	IOEControlMgr() {};
	virtual ~IOEControlMgr() {};

	virtual bool AddControlHandle(IOEControl* pControl) = 0;
	virtual IOEControl* GetControl(const tstring& strClassName) = 0;

};

extern IOEControlMgr* g_pOEControlMgr;

#endif // __IOECONTROLMGR_H__
