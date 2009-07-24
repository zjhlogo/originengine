/*!
 * \file OED3DTextureMgr_Impl.h
 * \date 1-6-2009 14:57:44
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DTEXTUREMGR_IMPL_H__
#define __OED3DTEXTUREMGR_IMPL_H__

#include <IOETextureMgr.h>
#include <map>

class COED3DTextureMgr_Impl : public IOETextureMgr
{
public:
	typedef std::map<tstring, IOETexture*> TEXTURE_MAP;

public:
	COED3DTextureMgr_Impl();
	virtual ~COED3DTextureMgr_Impl();

	virtual IOETexture* CreateTextureFromFile(const tstring& strFileName);
	virtual IOETexture* CreateTexture(int nWidth, int nHeight, IOETexture::TEXTURE_FORMAT eFormat);

private:
	void Init();
	void Destroy();

private:
	TEXTURE_MAP m_TextureMap;

};

#endif // __OED3DTEXTUREMGR_IMPL_H__
