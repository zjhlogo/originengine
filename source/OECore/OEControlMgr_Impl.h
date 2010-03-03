/*!
 * \file OEControlMgr_Impl.h
 * \date 13-2-2010 13:14:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OECONTROLMGR_IMPL_H__
#define __OECONTROLMGR_IMPL_H__

#include <IOEControlMgr.h>
#include <map>

class COEControlMgr_Impl : public IOEControlMgr
{
public:
	typedef std::map<uint, IOEControl*> TM_CONTROL;

public:
	COEControlMgr_Impl();
	virtual ~COEControlMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual bool AddControlHandle(IOEControl* pControl);
	virtual IOEControl* GetControl(uint nID);

private:
	TM_CONTROL m_ControlMap;

};
#endif // __OECONTROLMGR_IMPL_H__
