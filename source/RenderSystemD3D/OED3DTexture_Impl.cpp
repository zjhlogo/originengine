/*!
 * \file OED3DTexture_Impl.cpp
 * \date 1-6-2009 14:36:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DTexture_Impl.h"
#include "OED3DUtil.h"

#include <IOEFileMgr.h>
#include <d3dx9.h>

extern IDirect3DDevice9* g_pd3dDevice;

COED3DTexture_Impl::COED3DTexture_Impl(const tstring& strFileName)
{
	Init();
	m_bOK = Create(strFileName);
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

TEXTURE_FORMAT COED3DTexture_Impl::GetFormat() const
{
	return m_eFormat;
}

bool COED3DTexture_Impl::Create(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName);
	if (!pFile) return false;

	uint nSize = pFile->GetSize();
	if (nSize <= 0) return false;

	uchar* pData = new uchar[nSize];
	pFile->Read(pData, nSize);
	SAFE_RELEASE(pFile);

	//HRESULT hRet = D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, pData, nSize, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture);
	HRESULT hRet = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, pData, nSize, &m_pTexture);
	SAFE_DELETE_ARRAY(pData);

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
