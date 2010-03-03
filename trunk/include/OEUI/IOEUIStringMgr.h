/*!
 * \file IOEUIStringMgr.h
 * \date 28-7-2009 9:38:49
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEUISTRINGMGR_H__
#define __IOEUISTRINGMGR_H__

#include "IOEMgr.h"
#include "IOEUIString.h"

class IOEUIStringMgr : public IOEMgr
{
public:
	IOEUIStringMgr() {};
	virtual ~IOEUIStringMgr() {};

	virtual IOEUIString* CreateUIString(IOEUIFont* pFont) = 0;

};

extern IOEUIStringMgr* g_pOEUIStringMgr;

#endif // __IOEUISTRINGMGR_H__
