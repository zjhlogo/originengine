/*!
 * \file OED3DRenderTargetTexture_Impl.cpp
 * \date 10-25-2010 22:35:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */

#include "OED3DRenderTargetTexture_Impl.h"
#include "OED3DUtil.h"

extern IDirect3DDevice9* g_pd3dDevice;

COED3DRenderTargetTexture_Impl::COED3DRenderTargetTexture_Impl(int nWidth, int nHeight, TEXTURE_FORMAT eFormat)
{
	Init();
	m_bOK = Create(nWidth, nHeight, eFormat);
}

COED3DRenderTargetTexture_Impl::~COED3DRenderTargetTexture_Impl()
{
	Destroy();
}

void COED3DRenderTargetTexture_Impl::Init()
{
	m_pTexture = NULL;
	m_pDepthStencilSurface = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_eFormat = TF_UNKNOWN;
}

void COED3DRenderTargetTexture_Impl::Destroy()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pDepthStencilSurface);
}

int COED3DRenderTargetTexture_Impl::GetWidth() const
{
	return m_nWidth;
}

int COED3DRenderTargetTexture_Impl::GetHeight() const
{
	return m_nHeight;
}

TEXTURE_FORMAT COED3DRenderTargetTexture_Impl::GetFormat() const
{
	return m_eFormat;
}

IDirect3DSurface9* COED3DRenderTargetTexture_Impl::GetSurface()
{
	if (!m_pTexture) return NULL;

	IDirect3DSurface9* pD3DSurface = NULL;
	HRESULT hRet = m_pTexture->GetSurfaceLevel(0, &pD3DSurface);
	if (FAILED(hRet)) return NULL;

	return pD3DSurface;
}

IDirect3DSurface9* COED3DRenderTargetTexture_Impl::GetDepthStencilSurface()
{
	return m_pDepthStencilSurface;
}

bool COED3DRenderTargetTexture_Impl::Create(int nWidth, int nHeight, TEXTURE_FORMAT eFormat)
{
	D3DFORMAT eD3DFmt = COED3DUtil::ToD3DTexFmt(eFormat);
	if (eD3DFmt == D3DFMT_UNKNOWN) return false;

	HRESULT hRet = g_pd3dDevice->CreateTexture(nWidth, nHeight, 1, D3DUSAGE_RENDERTARGET, eD3DFmt, D3DPOOL_DEFAULT, &m_pTexture, NULL);
	if (FAILED(hRet)) return false;

	hRet = g_pd3dDevice->CreateDepthStencilSurface(nWidth, nHeight, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, 0, &m_pDepthStencilSurface, 0);
	if (FAILED(hRet)) return false;

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_eFormat = eFormat;

	return true;
}
