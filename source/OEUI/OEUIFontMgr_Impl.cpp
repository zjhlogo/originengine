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

#include <IOELogFileMgr.h>
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
	// TODO: check m_UIFontMap whether is empty, and logout
}

IOEUIFont* COEUIFontMgr_Impl::CreateFont(const tstring& strFileName)
{
	// TODO: 
	return NULL;
}

IOEUIFont* COEUIFontMgr_Impl::CreateBitmapFont(const tstring& strFileName)
{
	// transform string to lower
	tstring strLowName = strFileName;
	std::transform(strLowName.begin(), strLowName.end(), strLowName.begin(), tolower);

	// find from map, if exist just increase reference and return
	UIFONT_MAP::iterator itfound = m_UIFontMap.find(strLowName);
	if (itfound != m_UIFontMap.end())
	{
		IOEUIFont* pFont = itfound->second;
		pFont->IncRef();
		return pFont;
	}

	// no lucky, really create font
	COEUIBitmapFont_Impl* pFont = new COEUIBitmapFont_Impl(strLowName);
	if (!pFont || !pFont->IsOK())
	{
		LOGOUT(TS("IOEUIFontMgr::CreateFont Failed \"%s\""), strLowName.c_str());
		SAFE_RELEASE(pFont);
		return NULL;
	}

	m_UIFontMap.insert(std::make_pair(strLowName, pFont));

	return pFont;
}
