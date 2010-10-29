/*!
 * \file OED3DRenderTargetTexture_Impl.h
 * \date 10-25-2010 22:31:54
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OED3DRENDERTARGETTEXTURE_IMPL_H__
#define __OED3DRENDERTARGETTEXTURE_IMPL_H__

#include <OECore/IOETexture.h>
#include <d3d9.h>

class COED3DRenderTargetTexture_Impl : public IOETexture
{
public:
	RTTI_DEF(COED3DRenderTargetTexture_Impl, IOETexture);

	COED3DRenderTargetTexture_Impl(int nWidth, int nHeight, TEXTURE_FORMAT eFormat);
	virtual ~COED3DRenderTargetTexture_Impl();

	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual TEXTURE_FORMAT GetFormat() const;

	IDirect3DSurface9* GetSurface();
	IDirect3DSurface9* GetDepthStencilSurface();

private:
	void Init();
	void Destroy();

	bool Create(int nWidth, int nHeight, TEXTURE_FORMAT eFormat);

private:
	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* m_pDepthStencilSurface;
	int m_nWidth;
	int m_nHeight;
	TEXTURE_FORMAT m_eFormat;

};
#endif // __OED3DRENDERTARGETTEXTURE_IMPL_H__
