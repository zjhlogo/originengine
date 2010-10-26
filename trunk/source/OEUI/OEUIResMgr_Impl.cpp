/*!
 * \file OEUIResMgr_Impl.cpp
 * \date 10-26-2010 0:06:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEUIResMgr_Impl.h"
#include "OEUIFont_Impl.h"
#include "OEUIBitmapFont_Impl.h"

#include <OEBase/IOELogFileMgr.h>
#include <OECore/IOEConfigFileMgr.h>
#include <algorithm>
#include <assert.h>

COEUIResMgr_Impl::COEUIResMgr_Impl()
{
	assert(!g_pOEUIResMgr);
	g_pOEUIResMgr = this;
	m_bOK = Init();
}

COEUIResMgr_Impl::~COEUIResMgr_Impl()
{
	Destroy();
	g_pOEUIResMgr = NULL;
}

bool COEUIResMgr_Impl::Init()
{
	// TODO: 
	return true;
}

void COEUIResMgr_Impl::Destroy()
{
	// TODO: 
}

bool COEUIResMgr_Impl::Initialize()
{
	tstring strDefaultFontDir;
	g_pOEConfigFileMgr->GetValue(strDefaultFontDir, TS("DefaultFontDir"), TS("media"));
	SetDefaultDir(strDefaultFontDir);

	return true;
}

void COEUIResMgr_Impl::Terminate()
{
	// TODO: check m_UIFontMap whether is empty, and logout
}

IOEUIFont* COEUIResMgr_Impl::CreateFont(const tstring& strFile)
{
	// TODO: 
	return NULL;
}

IOEUIFont* COEUIResMgr_Impl::CreateBitmapFont(const tstring& strFile)
{
	// get font file path
	tstring strFilePath;
	if (!GetFontFilePath(strFilePath, strFile)) return NULL;

	// find from map, if exist just increase reference and return
	TM_FONT::iterator itfound = m_FontMap.find(strFilePath);
	if (itfound != m_FontMap.end())
	{
		IOEUIFont* pFont = itfound->second;
		pFont->IncRef();
		return pFont;
	}

	// no lucky, really create font
	COEUIBitmapFont_Impl* pFont = new COEUIBitmapFont_Impl(strFilePath);
	if (!pFont || !pFont->IsOK())
	{
		LOGOUT(TS("IOEUIFontMgr::CreateFont Failed \"%s\""), strFilePath.c_str());
		SAFE_RELEASE(pFont);
		return NULL;
	}

	m_FontMap.insert(std::make_pair(strFilePath, pFont));
	return pFont;
}

void COEUIResMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& COEUIResMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

bool COEUIResMgr_Impl::GetFontFilePath(tstring& strFilePathOut, const tstring& strFile)
{
	strFilePathOut = m_strDefaultDir + TS("\\") + strFile;
	COEOS::tolower(strFilePathOut, strFilePathOut);
	return true;
}
