/*!
 * \file IOEUIResMgr.h
 * \date 10-25-2010 23:59:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOEUIRESMGR_H__
#define __IOEUIRESMGR_H__

#include "../OECore/IOEMgr.h"
#include "IOEUIFont.h"

class IOEUIResMgr : public IOEMgr
{
public:
	RTTI_DEF(IOEUIResMgr, IOEMgr);

	IOEUIResMgr() {};
	virtual ~IOEUIResMgr() {};

	virtual IOEUIFont* CreateFont(const tstring& strFile) = 0;
	virtual IOEUIFont* CreateBitmapFont(const tstring& strFile) = 0;

	virtual void SetDefaultDir(const tstring& strDir) = 0;
	virtual const tstring& GetDefaultDir() = 0;

};

extern IOEUIResMgr* g_pOEUIResMgr;
#endif // __IOEUIRESMGR_H__
