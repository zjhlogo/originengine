/*!
 * \file IOEShaderMgr.h
 * \date 1-6-2009 16:38:41
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOESHADERMGR_H__
#define __IOESHADERMGR_H__

#include "IOEShader.h"
#include "IOEVertDecl.h"

class IOEShaderMgr
{
public:
	IOEShaderMgr() {};
	virtual ~IOEShaderMgr() {};

	virtual IOEShader* CreateShader(const IOEVertDecl::ELEMENT* pElement, const tchar* pstrFileName) = 0;

};

extern IOEShaderMgr* g_pOEShaderMgr;

#endif // __IOESHADERMGR_H__
