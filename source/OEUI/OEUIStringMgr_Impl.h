/*!
 * \file OEUIStringMgr_Impl.h
 * \date 28-7-2009 9:38:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUISTRINGMGR_IMPL_H__
#define __OEUISTRINGMGR_IMPL_H__

#include <OEUI/IOEUIStringMgr.h>

class COEUIStringMgr_Impl : public IOEUIStringMgr
{
public:
	COEUIStringMgr_Impl();
	virtual ~COEUIStringMgr_Impl();

	virtual IOEUIString* CreateUIString(IOEUIFont* pFont);

private:
	void Init();
	void Destroy();

private:

};
#endif // __OEUISTRINGMGR_IMPL_H__
