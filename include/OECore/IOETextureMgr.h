/*!
 * \file IOETextureMgr.h
 * \date 1-6-2009 14:28:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOETEXTUREMGR_H__
#define __IOETEXTUREMGR_H__

#include "IOEMgr.h"
#include "IOETexture.h"

class IOETextureMgr : public IOEMgr
{
public:
	RTTI_DEF(IOETextureMgr, IOEMgr);

	IOETextureMgr() {};
	virtual ~IOETextureMgr() {};

	virtual IOETexture* CreateTextureFromFile(const tstring& strFile) = 0;
	virtual IOETexture* CreateTexture(int nWidth, int nHeight, TEXTURE_FORMAT eFormat) = 0;

	virtual void SetDefaultDir(const tstring& strDir) = 0;
	virtual const tstring& GetDefaultDir() = 0;

};

extern IOETextureMgr* g_pOETextureMgr;

#endif // __IOETEXTUREMGR_H__
