/*!
 * \file OED3DTexture_Impl.cpp
 * \date 1-6-2009 14:36:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DTexture_Impl.h"
#include "OED3DUtil.h"
#include <d3dx9.h>

extern IDirect3DDevice9* g_pd3dDevice;

COED3DTexture_Impl::COED3DTexture_Impl(const tchar* pstrFileName)
{
	Init();
	m_bOK = Create(pstrFileName);
}

COED3DTexture_Impl::COED3DTexture_Impl(int nWidth, int nHeight, TEXTURE_FORMAT eFormat)
{
	Init();
	m_bOK = Create(nWidth, nHeight, eFormat);
}

COED3DTexture_Impl::~COED3DTexture_Impl()
{
	Destroy();
}

void COED3DTexture_Impl::Init()
{
	m_pTexture = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_eFormat = TF_UNKNOWN;
}

void COED3DTexture_Impl::Destroy()
{
	SAFE_RELEASE(m_pTexture);
}

int COED3DTexture_Impl::GetWidth() const
{
	return m_nWidth;
}

int COED3DTexture_Impl::GetHeight() const
{
	return m_nHeight;
}

IOETexture::TEXTURE_FORMAT COED3DTexture_Impl::GetFormat() const
{
	return m_eFormat;
}

bool COED3DTexture_Impl::Create(const tchar* pstrFileName)
{
	HRESULT hRet = D3DXCreateTextureFromFile(g_pd3dDevice, pstrFileName, &m_pTexture);
	if (FAILED(hRet)) return false;

	D3DSURFACE_DESC Desc;
	m_pTexture->GetLevelDesc(0, &Desc);

	m_nWidth = Desc.Width;
	m_nHeight = Desc.Height;
	m_eFormat = COED3DUtil::ToOETexFmt(Desc.Format);

	return true;
}

bool COED3DTexture_Impl::Create(int nWidth, int nHeight, TEXTURE_FORMAT eFormat)
{
	D3DFORMAT eD3DFmt = COED3DUtil::ToD3DTexFmt(eFormat);
	if (eD3DFmt == D3DFMT_UNKNOWN) return false;

	HRESULT hRet = g_pd3dDevice->CreateTexture(nWidth, nHeight, 0, 0, eD3DFmt, D3DPOOL_MANAGED, &m_pTexture, NULL);
	if (FAILED(hRet)) return false;

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_eFormat = eFormat;

	return true;
}

IDirect3DTexture9* COED3DTexture_Impl::GetTexture() const
{
	return m_pTexture;
}
