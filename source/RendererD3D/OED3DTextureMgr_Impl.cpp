/*!
 * \file OED3DTextureMgr_Impl.cpp
 * \date 1-6-2009 14:58:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DTextureMgr_Impl.h"
#include "OED3DTexture_Impl.h"
#include <algorithm>

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
	m_TextureMap.clear();
}

void COED3DTextureMgr_Impl::Destroy()
{
	// TODO: check m_TextureMap whether is empty, and logout
}

IOETexture* COED3DTextureMgr_Impl::CreateTextureFromFile(const tchar* pstrFileName)
{
	// transform string to lower
	tstring strFileName = pstrFileName;
	std::transform(strFileName.begin(), strFileName.end(), strFileName.begin(), tolower);

	// find from map, if exist just increase reference and return
	TEXTURE_MAP::iterator itfound = m_TextureMap.find(strFileName);
	if (itfound != m_TextureMap.end())
	{
		IOETexture* pTexture = itfound->second;
		pTexture->IncRef();
		return pTexture;
	}

	// no lucky, really create texture
	COED3DTexture_Impl* pTexture = new COED3DTexture_Impl(strFileName.c_str());
	if (!pTexture || !pTexture->IsOK())
	{
		// TODO: logout
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	m_TextureMap.insert(std::make_pair(strFileName, pTexture));

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
