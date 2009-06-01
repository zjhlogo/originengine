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

class COED3DTextureMgr_Impl : public IOETextureMgr
{
public:
	COED3DTextureMgr_Impl();
	virtual ~COED3DTextureMgr_Impl();

	virtual IOETexture* CreateTextureFromFile(const tchar* pstrFileName);
	virtual IOETexture* CreateTexture(int nWidth, int nHeight, IOETexture::TEXTURE_FORMAT eFormat);

private:
	void Init();
	void Destroy();

};

#endif // __OED3DTEXTUREMGR_IMPL_H__
