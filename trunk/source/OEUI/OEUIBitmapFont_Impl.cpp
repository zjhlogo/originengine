/*!
 * \file OEUIBitmapFont_Impl.cpp
 * \date 27-7-2009 19:47:57
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIBitmapFont_Impl.h"
#include <IOEXmlMgr.h>
#include <IOETextureMgr.h>

#include <IOEXmlMgr.h>
#include <IOETextureMgr.h>

COEUIBitmapFont_Impl::COEUIBitmapFont_Impl(const tstring& strFile)
{
	Init();
	m_bOK = Create(strFile);
}

COEUIBitmapFont_Impl::~COEUIBitmapFont_Impl()
{
	Destroy();
}

void COEUIBitmapFont_Impl::Init()
{
	m_pXmlDocument = NULL;
	m_fLineHeight = 0.0f;
	m_nPageCount = 0;
}

void COEUIBitmapFont_Impl::Destroy()
{
	SAFE_RELEASE(m_pXmlDocument);
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

bool COEUIBitmapFont_Impl::Create(const tstring& strFile)
{
	m_pXmlDocument = g_pOEXmlMgr->CreateDocument(strFile);
	if (!m_pXmlDocument) return false;

	IOEXmlNode* pRootNode = m_pXmlDocument->GetRootNode();
	if (!pRootNode || pRootNode->GetName() != TS("font")) return false;

	IOEXmlNode* pCommonNode = pRootNode->FirstChild(TS("common"));
	if (!pCommonNode) return false;
	if (!ParseCommonInfo(pCommonNode)) return false;

	IOEXmlNode* pPagesNode = pRootNode->FirstChild(TS("pages"));
	if (!pPagesNode) return false;
	if (!CreateTextures(pPagesNode)) return false;

	IOEXmlNode* pCharsInfo = pRootNode->FirstChild(TS("chars"));
	if (!pCharsInfo) return false;
	if (!CreateCharsInfo(pCharsInfo)) return false;

	IOEXmlNode* pKerningsInfo = pRootNode->FirstChild(TS("kernings"));
	if (!pKerningsInfo) return false;
	if (!CreateKerningsInfo(pKerningsInfo)) return false;

	SAFE_RELEASE(m_pXmlDocument);
	return true;
}

bool COEUIBitmapFont_Impl::ParseCommonInfo(IOEXmlNode* pCommonNode)
{
	if (!pCommonNode->GetAttribute(m_fLineHeight, TS("lineHeight"))) return false;
	if (!pCommonNode->GetAttribute(m_nPageCount, TS("pages"))) return false;
	return true;
}

bool COEUIBitmapFont_Impl::CreateTextures(IOEXmlNode* pPagesNode)
{
	DestroyTextures();

	IOEXmlNode* pPageNode = pPagesNode->FirstChild(TS("page"));

	for (int i = 0; i < m_nPageCount && pPageNode != NULL; ++i, pPageNode = pPageNode->NextSibling(TS("page")))
	{
		tstring strTexture;
		if (!pPageNode->GetAttribute(strTexture, TS("file"))) return false;

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
	if (!pCharsNode->GetAttribute(nCount, TS("count"))) return false;

	IOEXmlNode* pCharNode = pCharsNode->FirstChild(TS("char"));

	for (int i = 0; i < nCount && pCharNode != NULL; ++i, pCharNode = pCharNode->NextSibling(TS("char")))
	{
		CHAR_INFO CharInfo;
		if (!pCharNode->GetAttribute(CharInfo.nID, TS("id"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.x, TS("x"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.y, TS("y"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.width, TS("width"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.height, TS("height"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fOffsetX, TS("xoffset"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fOffsetY, TS("yoffset"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fAdvance, TS("xadvance"))) return false;

		int nPage = 0;
		if (!pCharNode->GetAttribute(nPage, TS("page"))) return false;
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
	if (!pKerningsNode->GetAttribute(nCount, TS("count"))) return false;

	IOEXmlNode* pKerningNode = pKerningsNode->FirstChild(TS("kerning"));

	for (int i = 0; i < nCount && pKerningNode != NULL; ++i, pKerningNode = pKerningNode->NextSibling(TS("kerning")))
	{
		int nFirstID = 0;
		int nSecondID = 0;
		float fOffset = 0.0f;
		if (!pKerningNode->GetAttribute(nFirstID, TS("first"))) return false;
		if (!pKerningNode->GetAttribute(nSecondID, TS("second"))) return false;
		if (!pKerningNode->GetAttribute(fOffset, TS("amount"))) return false;

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
