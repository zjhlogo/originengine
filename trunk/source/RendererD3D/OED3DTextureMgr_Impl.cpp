/*!
 * \file OED3DTextureMgr_Impl.cpp
 * \date 1-6-2009 14:58:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DTextureMgr_Impl.h"
#include "OED3DTexture_Impl.h"

COED3DTextureMgr_Impl::COED3DTextureMgr_Impl()
{
	g_pOETextureMgr = this;
	Init();
}

COED3DTextureMgr_Impl::~COED3DTextureMgr_Impl()
{
	Destroy();
	g_pOETextureMgr = NULL;
}

void COED3DTextureMgr_Impl::Init()
{
	// TODO: 
}

void COED3DTextureMgr_Impl::Destroy()
{
	// TODO: 
}

IOETexture* COED3DTextureMgr_Impl::CreateTextureFromFile(const tchar* pstrFileName)
{
	COED3DTexture_Impl* pTexture = new COED3DTexture_Impl(pstrFileName);
	if (!pTexture || !pTexture->IsOK())
	{
		// TODO: logout
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	return pTexture;
}

IOETexture* COED3DTextureMgr_Impl::CreateTexture(int nWidth, int nHeight, IOETexture::TEXTURE_FORMAT eFormat)
{
	COED3DTexture_Impl* pTexture = new COED3DTexture_Impl(nWidth, nHeight, eFormat);
	if (!pTexture || !pTexture->IsOK())
	{
		// TODO: logout
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	return pTexture;
}
