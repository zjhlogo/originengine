/*!
 * \file IOETextureMgr.h
 * \date 1-6-2009 14:28:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOETEXTUREMGR_H__
#define __IOETEXTUREMGR_H__

#include "IOETexture.h"

class IOETextureMgr
{
public:
	IOETextureMgr() {};
	virtual ~IOETextureMgr() {};

	virtual IOETexture* CreateTextureFromFile(const tstring& strFileName) = 0;
	virtual IOETexture* CreateTexture(int nWidth, int nHeight, IOETexture::TEXTURE_FORMAT eFormat) = 0;

};

extern IOETextureMgr* g_pOETextureMgr;

#endif // __IOETEXTUREMGR_H__
