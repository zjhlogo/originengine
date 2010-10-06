/*!
 * \file OED3DTextureMgr_Impl.cpp
 * \date 1-6-2009 14:58:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DTextureMgr_Impl.h"
#include "OED3DTexture_Impl.h"

#include <OEBase/IOELogFileMgr.h>
#include <OECore/IOEConfigFileMgr.h>
#include <libOEBase/OEOS.h>
#include <assert.h>

COED3DTextureMgr_Impl::COED3DTextureMgr_Impl()
{
	assert(!g_pOETextureMgr);
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
	// TODO: 
	return true;
}

void COED3DTextureMgr_Impl::Destroy()
{
	// TODO: 
}

bool COED3DTextureMgr_Impl::Initialize()
{
	tstring strDefaultTextureDir;
	g_pOEConfigFileMgr->GetValue(strDefaultTextureDir, TS("DefaultTextureDir"), TS("media"));
	SetDefaultDir(strDefaultTextureDir);

	return true;
}

void COED3DTextureMgr_Impl::Terminate()
{
	// TODO: check m_FileTextureMap/m_vMemoryTexture whether is empty, and logout
}

IOETexture* COED3DTextureMgr_Impl::CreateTextureFromFile(const tstring& strFile)
{
	// transform string to lower
	tstring strFilePath;
	if (!GetTextureFilePath(strFilePath, strFile)) return NULL;

	// find from map, if exist just increase reference and return
	TM_TEXTURE::iterator itfound = m_FileTextureMap.find(strFilePath);
	if (itfound != m_FileTextureMap.end())
	{
		IOETexture* pTexture = itfound->second;
		pTexture->IncRef();
		return pTexture;
	}

	// no lucky, really create texture
	COED3DTexture_Impl* pTexture = new COED3DTexture_Impl(strFilePath);
	if (!pTexture || !pTexture->IsOK())
	{
		LOGOUT(TS("IOETextureMgr::CreateTextureFromFile Failed \"%s\""), strFilePath.c_str());
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	m_FileTextureMap.insert(std::make_pair(strFilePath, pTexture));
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

	m_vMemoryTexture.push_back(pTexture);
	return pTexture;
}

void COED3DTextureMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& COED3DTextureMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

bool COED3DTextureMgr_Impl::GetTextureFilePath(tstring& strFilePathOut, const tstring& strFile)
{
	strFilePathOut = m_strDefaultDir + TS("\\") + strFile;
	COEOS::tolower(strFilePathOut, strFilePathOut);
	return true;
}
