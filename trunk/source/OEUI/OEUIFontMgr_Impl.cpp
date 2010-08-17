/*!
 * \file OEUIFontMgr_Impl.cpp
 * \date 27-7-2009 21:59:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIFontMgr_Impl.h"
#include "OEUIFont_Impl.h"
#include "OEUIBitmapFont_Impl.h"

#include <OEBase/IOELogFileMgr.h>
#include <OECore/IOEConfigFileMgr.h>
#include <algorithm>
#include <assert.h>

COEUIFontMgr_Impl::COEUIFontMgr_Impl()
{
	assert(!g_pOEUIFontMgr);
	g_pOEUIFontMgr = this;
	m_bOK = Init();
}

COEUIFontMgr_Impl::~COEUIFontMgr_Impl()
{
	Destroy();
	g_pOEUIFontMgr = NULL;
}

bool COEUIFontMgr_Impl::Init()
{
	// TODO: 
	return true;
}

void COEUIFontMgr_Impl::Destroy()
{
	// TODO: 
}

bool COEUIFontMgr_Impl::Initialize()
{
	tstring strDefaultFontDir;
	g_pOEConfigFileMgr->GetValue(strDefaultFontDir, TS("DefaultFontDir"), TS("media"));
	SetDefaultDir(strDefaultFontDir);

	return true;
}

void COEUIFontMgr_Impl::Terminate()
{
	// TODO: check m_UIFontMap whether is empty, and logout
}

IOEUIFont* COEUIFontMgr_Impl::CreateFont(const tstring& strFile)
{
	// TODO: 
	return NULL;
}

IOEUIFont* COEUIFontMgr_Impl::CreateBitmapFont(const tstring& strFile)
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

void COEUIFontMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& COEUIFontMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

bool COEUIFontMgr_Impl::GetFontFilePath(tstring& strFilePathOut, const tstring& strFile)
{
	strFilePathOut = m_strDefaultDir + TS("\\") + strFile;
	COEOS::tolower(strFilePathOut, strFilePathOut);
	return true;
}
