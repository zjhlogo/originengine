/*!
 * \file IOEUIFontMgr.h
 * \date 27-7-2009 21:54:35
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEUIFONTMGR_H__
#define __IOEUIFONTMGR_H__

#include "IOEObject.h"
#include "IOEUIFont.h"

class IOEUIFontMgr : public IOEObject
{
public:
	IOEUIFontMgr() {};
	virtual ~IOEUIFontMgr() {};

	virtual IOEUIFont* CreateFont(const tstring& strFile) = 0;
	virtual IOEUIFont* CreateBitmapFont(const tstring& strFile) = 0;

	virtual void SetDefaultDir(const tstring& strDir) = 0;
	virtual const tstring& GetDefaultDir() = 0;

};

extern IOEUIFontMgr* g_pOEUIFontMgr;

#endif // __IOEUIFONTMGR_H__
