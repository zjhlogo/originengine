/*!
 * \file IOEControlMgr.h
 * \date 13-2-2010 12:25:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOECONTROLMGR_H__
#define __IOECONTROLMGR_H__

#include "IOEObject.h"
#include "IOEControl.h"

class IOEControlMgr : public IOEObject
{
public:
	IOEControlMgr() {};
	virtual ~IOEControlMgr() {};

	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;
	virtual bool AddControlHandle(IOEControl* pControl) = 0;
	virtual IOEControl* GetControl(uint nID) = 0;

};

extern IOEControlMgr* g_pOEControlMgr;

#endif // __IOECONTROLMGR_H__
