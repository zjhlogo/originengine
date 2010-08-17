/*!
 * \file OED3DTexture_Impl.h
 * \date 1-6-2009 14:32:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DTEXTURE_IMPL_H__
#define __OED3DTEXTURE_IMPL_H__

#include <OECore/IOETexture.h>
#include <d3d9.h>

class COED3DTexture_Impl : public IOETexture
{
public:
	COED3DTexture_Impl(const tstring& strFileName);
	COED3DTexture_Impl(int nWidth, int nHeight, TEXTURE_FORMAT eFormat);
	virtual ~COED3DTexture_Impl();

	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual TEXTURE_FORMAT GetFormat() const;

	IDirect3DTexture9* GetTexture() const;

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFileName);
	bool Create(int nWidth, int nHeight, TEXTURE_FORMAT eFormat);

private:
	IDirect3DTexture9* m_pTexture;
	int m_nWidth;
	int m_nHeight;
	TEXTURE_FORMAT m_eFormat;

};

#endif // __OED3DTEXTURE_IMPL_H__
