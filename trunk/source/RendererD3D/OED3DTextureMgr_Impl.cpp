/*!
 * \file OED3DTextureMgr_Impl.cpp
 * \date 1-6-2009 14:58:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DTextureMgr_Impl.h"
#include "OED3DTexture_Impl.h"

#include <IOELogFileMgr.h>
#include <OEOS.h>

COED3DTextureMgr_Impl::COED3DTextureMgr_Impl()
{
	g_pOETextureMgr = this;
	m_bOK = Init();
}

COED3DTextureMgr_Impl::~COED3DTextureMgr_Impl()
{
	Destroy();
	g_pOETextureMgr = NULL;
}

bool COED3DTextureMgr_Impl::Init()
{
	m_TextureMap.clear();
	return true;
}

void COED3DTextureMgr_Impl::Destroy()
{
	// TODO: check m_TextureMap whether is empty, and logout
}

IOETexture* COED3DTextureMgr_Impl::CreateTextureFromFile(const tstring& strFileName)
{
	// transform string to lower
	tstring strLowName;
	COEOS::tolower(strLowName, strFileName);

	// find from map, if exist just increase reference and return
	TEXTURE_MAP::iterator itfound = m_TextureMap.find(strLowName);
	if (itfound != m_TextureMap.end())
	{
		IOETexture* pTexture = itfound->second;
		pTexture->IncRef();
		return pTexture;
	}

	// no lucky, really create texture
	COED3DTexture_Impl* pTexture = new COED3DTexture_Impl(strLowName);
	if (!pTexture || !pTexture->IsOK())
	{
		LOGOUT(TS("IOETextureMgr::CreateTextureFromFile Failed \"%s\""), strLowName.c_str());
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	m_TextureMap.insert(std::make_pair(strLowName, pTexture));

	return pTexture;
}

IOETexture* COED3DTextureMgr_Impl::CreateTexture(int nWidth, int nHeight, TEXTURE_FORMAT eFormat)
{
	COED3DTexture_Impl* pTexture = new COED3DTexture_Impl(nWidth, nHeight, eFormat);
	if (!pTexture || !pTexture->IsOK())
	{
		LOGOUT(TS("IOETextureMgr::CreateTextureFromFile Failed width=%d, height=%d, format=%d"), nWidth, nHeight, eFormat);
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	return pTexture;
}
