/*!
 * \file OEUIBitmapFont_Impl.cpp
 * \date 27-7-2009 19:47:57
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIBitmapFont_Impl.h"
#include <OEInterfaces.h>

COEUIBitmapFont_Impl::COEUIBitmapFont_Impl(const tstring& strFileName)
{
	Init();
	m_bOK = Create(strFileName);
}

COEUIBitmapFont_Impl::~COEUIBitmapFont_Impl()
{
	Destroy();
}

void COEUIBitmapFont_Impl::Init()
{
	m_pDocument = NULL;
	m_fLineHeight = 0.0f;
	m_nPageCount = 0;
}

void COEUIBitmapFont_Impl::Destroy()
{
	SAFE_RELEASE(m_pDocument);
	DestroyTextures();
	DestroyKerningsInfo();
	DestroyCharsInfo();
}

float COEUIBitmapFont_Impl::GetLineHeight() const
{
	return m_fLineHeight;
}

const IOEUIFont::CHAR_INFO* COEUIBitmapFont_Impl::GetCharInfo(int nID) const
{
	CHAR_INFO_MAP::const_iterator itfound = m_CharInfoMap.find(nID);
	if (itfound != m_CharInfoMap.end())
	{
		return &itfound->second;
	}

	return NULL;
}

float COEUIBitmapFont_Impl::GetKerning(int nFirstID, int nSecondID) const
{
	int nHaskKey = HashKerningID(nFirstID, nSecondID);
	KERNING_INFO_MAP::const_iterator itfound = m_KerningInfoMap.find(nHaskKey);
	if (itfound != m_KerningInfoMap.end())
	{
		return itfound->second;
	}

	return 0.0f;
}

bool COEUIBitmapFont_Impl::Create(const tstring& strFileName)
{
	m_pDocument = g_pOEXmlMgr->OpenXmlFile(strFileName);
	if (!m_pDocument) return false;

	IOEXmlNode* pRootNode = m_pDocument->FirstChild(_T("font"));
	if (!pRootNode) return false;

	IOEXmlNode* pCommonNode = pRootNode->FirstChild(_T("common"));
	if (!pCommonNode) return false;
	if (!ParseCommonInfo(pCommonNode)) return false;

	IOEXmlNode* pPagesNode = pRootNode->FirstChild(_T("pages"));
	if (!pPagesNode) return false;
	if (!CreateTextures(pPagesNode)) return false;

	IOEXmlNode* pCharsInfo = pRootNode->FirstChild(_T("chars"));
	if (!pCharsInfo) return false;
	if (!CreateCharsInfo(pCharsInfo)) return false;

	IOEXmlNode* pKerningsInfo = pRootNode->FirstChild(_T("kernings"));
	if (!pKerningsInfo) return false;
	if (!CreateKerningsInfo(pKerningsInfo)) return false;

	SAFE_RELEASE(m_pDocument);
	return true;
}

bool COEUIBitmapFont_Impl::ParseCommonInfo(IOEXmlNode* pCommonNode)
{
	if (!pCommonNode->GetAttribute(m_fLineHeight, _T("lineHeight"))) return false;
	if (!pCommonNode->GetAttribute(m_nPageCount, _T("pages"))) return false;
	return true;
}

bool COEUIBitmapFont_Impl::CreateTextures(IOEXmlNode* pPagesNode)
{
	DestroyTextures();

	IOEXmlNode* pPageNode = pPagesNode->FirstChild(_T("page"));

	for (int i = 0; i < m_nPageCount && pPageNode != NULL; ++i, pPageNode = pPageNode->NextSibling(_T("page")))
	{
		tstring strTexture;
		if (!pPageNode->GetAttribute(strTexture, _T("file"))) return false;

		IOETexture* pTexture = g_pOETextureMgr->CreateTextureFromFile(strTexture);
		if (!pTexture) return false;

		m_vTexture.push_back(pTexture);
	}

	return true;
}

void COEUIBitmapFont_Impl::DestroyTextures()
{
	for (VOETEXTURE::iterator it = m_vTexture.begin(); it != m_vTexture.end(); ++it)
	{
		IOETexture* pTexture = *it;
		SAFE_RELEASE(pTexture);
	}
	m_vTexture.clear();
}

bool COEUIBitmapFont_Impl::CreateCharsInfo(IOEXmlNode* pCharsNode)
{
	DestroyCharsInfo();

	int nCount = 0;
	if (!pCharsNode->GetAttribute(nCount, _T("count"))) return false;

	IOEXmlNode* pCharNode = pCharsNode->FirstChild(_T("char"));

	for (int i = 0; i < nCount && pCharNode != NULL; ++i, pCharNode = pCharNode->NextSibling(_T("char")))
	{
		CHAR_INFO CharInfo;
		if (!pCharNode->GetAttribute(CharInfo.nID, _T("id"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.x, _T("x"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.y, _T("y"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.width, _T("width"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.height, _T("height"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fOffsetX, _T("xoffset"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fOffsetY, _T("yoffset"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fAdvance, _T("xadvance"))) return false;

		int nPage = 0;
		if (!pCharNode->GetAttribute(nPage, _T("page"))) return false;
		if (nPage >= (int)m_vTexture.size()) return false;

		CharInfo.pTexture = m_vTexture[nPage];
		CharInfo.nRef = 0;

		CharInfo.u = (CharInfo.x+0.5f)/CharInfo.pTexture->GetWidth();
		CharInfo.v = (CharInfo.y+0.5f)/CharInfo.pTexture->GetHeight();
		CharInfo.w = CharInfo.width/CharInfo.pTexture->GetWidth();
		CharInfo.h = CharInfo.height/CharInfo.pTexture->GetHeight();

		m_CharInfoMap.insert(std::make_pair(CharInfo.nID, CharInfo));
	}

	return true;
}

void COEUIBitmapFont_Impl::DestroyCharsInfo()
{
	m_CharInfoMap.clear();
}

bool COEUIBitmapFont_Impl::CreateKerningsInfo(IOEXmlNode* pKerningsNode)
{
	DestroyKerningsInfo();

	int nCount = 0;
	if (!pKerningsNode->GetAttribute(nCount, _T("count"))) return false;

	IOEXmlNode* pKerningNode = pKerningsNode->FirstChild(_T("kerning"));

	for (int i = 0; i < nCount && pKerningNode != NULL; ++i, pKerningNode = pKerningNode->NextSibling(_T("kerning")))
	{
		int nFirstID = 0;
		int nSecondID = 0;
		float fOffset = 0.0f;
		if (!pKerningNode->GetAttribute(nFirstID, _T("first"))) return false;
		if (!pKerningNode->GetAttribute(nSecondID, _T("second"))) return false;
		if (!pKerningNode->GetAttribute(fOffset, _T("amount"))) return false;

		int nHaskKey = HashKerningID(nFirstID, nSecondID);

		m_KerningInfoMap.insert(std::make_pair(nHaskKey, fOffset));
	}

	return true;
}

void COEUIBitmapFont_Impl::DestroyKerningsInfo()
{
	m_KerningInfoMap.clear();
}

int COEUIBitmapFont_Impl::HashKerningID(int nFirstID, int nSecondID) const
{
	int nHashKey = (nFirstID << 16) | (nSecondID & 0x0000FFFF);
	return nHashKey;
}
