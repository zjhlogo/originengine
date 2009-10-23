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
	if (!pRootNode || pRootNode->GetName() != t("font")) return false;

	IOEXmlNode* pCommonNode = pRootNode->FirstChild(t("common"));
	if (!pCommonNode) return false;
	if (!ParseCommonInfo(pCommonNode)) return false;

	IOEXmlNode* pPagesNode = pRootNode->FirstChild(t("pages"));
	if (!pPagesNode) return false;
	if (!CreateTextures(pPagesNode)) return false;

	IOEXmlNode* pCharsInfo = pRootNode->FirstChild(t("chars"));
	if (!pCharsInfo) return false;
	if (!CreateCharsInfo(pCharsInfo)) return false;

	IOEXmlNode* pKerningsInfo = pRootNode->FirstChild(t("kernings"));
	if (!pKerningsInfo) return false;
	if (!CreateKerningsInfo(pKerningsInfo)) return false;

	SAFE_RELEASE(m_pXmlDocument);
	return true;
}

bool COEUIBitmapFont_Impl::ParseCommonInfo(IOEXmlNode* pCommonNode)
{
	if (!pCommonNode->GetAttribute(m_fLineHeight, t("lineHeight"))) return false;
	if (!pCommonNode->GetAttribute(m_nPageCount, t("pages"))) return false;
	return true;
}

bool COEUIBitmapFont_Impl::CreateTextures(IOEXmlNode* pPagesNode)
{
	DestroyTextures();

	IOEXmlNode* pPageNode = pPagesNode->FirstChild(t("page"));

	for (int i = 0; i < m_nPageCount && pPageNode != NULL; ++i, pPageNode = pPageNode->NextSibling(t("page")))
	{
		tstring strTexture;
		if (!pPageNode->GetAttribute(strTexture, t("file"))) return false;

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
	if (!pCharsNode->GetAttribute(nCount, t("count"))) return false;

	IOEXmlNode* pCharNode = pCharsNode->FirstChild(t("char"));

	for (int i = 0; i < nCount && pCharNode != NULL; ++i, pCharNode = pCharNode->NextSibling(t("char")))
	{
		CHAR_INFO CharInfo;
		if (!pCharNode->GetAttribute(CharInfo.nID, t("id"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.x, t("x"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.y, t("y"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.width, t("width"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.height, t("height"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fOffsetX, t("xoffset"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fOffsetY, t("yoffset"))) return false;
		if (!pCharNode->GetAttribute(CharInfo.fAdvance, t("xadvance"))) return false;

		int nPage = 0;
		if (!pCharNode->GetAttribute(nPage, t("page"))) return false;
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
	if (!pKerningsNode->GetAttribute(nCount, t("count"))) return false;

	IOEXmlNode* pKerningNode = pKerningsNode->FirstChild(t("kerning"));

	for (int i = 0; i < nCount && pKerningNode != NULL; ++i, pKerningNode = pKerningNode->NextSibling(t("kerning")))
	{
		int nFirstID = 0;
		int nSecondID = 0;
		float fOffset = 0.0f;
		if (!pKerningNode->GetAttribute(nFirstID, t("first"))) return false;
		if (!pKerningNode->GetAttribute(nSecondID, t("second"))) return false;
		if (!pKerningNode->GetAttribute(fOffset, t("amount"))) return false;

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
