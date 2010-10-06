/*!
 * \file IOEShaderMgr.h
 * \date 1-6-2009 16:38:41
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOESHADERMGR_H__
#define __IOESHADERMGR_H__

#include "OEBaseTypeEx.h"
#include "IOEMgr.h"
#include "IOEShader.h"
#include "IOEVertDecl.h"

class IOEShaderMgr : public IOEMgr
{
public:
	RTTI_DEF(IOEShaderMgr, IOEMgr);

	IOEShaderMgr() {};
	virtual ~IOEShaderMgr() {};

	virtual IOEShader* CreateShader(const VERT_DECL_ELEMENT* pElement, const tstring& strFile) = 0;
	virtual IOEShader* CreateDefaultShader(DEFAULT_SHADER_TYPE eType) = 0;

	virtual void SetDefaultDir(const tstring& strDir) = 0;
	virtual const tstring& GetDefaultDir() = 0;

};

extern IOEShaderMgr* g_pOEShaderMgr;

#endif // __IOESHADERMGR_H__
