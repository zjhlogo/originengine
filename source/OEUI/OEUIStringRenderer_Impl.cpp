/*!
 * \file OEUIStringRenderer_Impl.cpp
 * \date 10-26-2010 0:03:16
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEUIStringRenderer_Impl.h"
#include <OECore/OEBaseTypeEx.h>
#include <OEUI/IOEUIRenderSystem.h>

COEUIStringRenderer_Impl::COEUIStringRenderer_Impl(IOEUIFont* pFont)
{
	Init();
	m_bOK = SetFont(pFont);
}

COEUIStringRenderer_Impl::~COEUIStringRenderer_Impl()
{
	Destroy();
}

void COEUIStringRenderer_Impl::Init()
{
	m_pFont = NULL;
	m_nColor = 0xFFFFFFFF;
}

void COEUIStringRenderer_Impl::Destroy()
{
	DestroyRenderChars();
}

void COEUIStringRenderer_Impl::Update(float fDetailTime)
{
	// TODO: 
}

void COEUIStringRenderer_Impl::Render(float fDetailTime)
{
	static const int VERTS_COUNT = 4;
	static const int INDIS_COUNT = 6;
	static const ushort s_Indis[INDIS_COUNT] = {0, 1, 3, 1, 2, 3};

	VERTEX_POLY_UI Verts[VERTS_COUNT];

	for (VCHAR_RENDER_INFO::iterator it = m_vCharRenderInfo.begin(); it != m_vCharRenderInfo.end(); ++it)
	{
		const CHAR_RENDER_INFO& CharRenderInfo = (*it);

		Verts[0].x = m_vPos.x+CharRenderInfo.x;
		Verts[0].y = m_vPos.y+CharRenderInfo.y+CharRenderInfo.pCharInfo->height;
		Verts[0].z = 0.0f;
		Verts[0].w = 1.0f;
		Verts[0].nColor = m_nColor;
		Verts[0].u = CharRenderInfo.pCharInfo->u;
		Verts[0].v = CharRenderInfo.pCharInfo->v+CharRenderInfo.pCharInfo->h;

		Verts[1].x = m_vPos.x+CharRenderInfo.x;
		Verts[1].y = m_vPos.y+CharRenderInfo.y;
		Verts[1].z = 0.0f;
		Verts[1].w = 1.0f;
		Verts[1].nColor = m_nColor;
		Verts[1].u = CharRenderInfo.pCharInfo->u;
		Verts[1].v = CharRenderInfo.pCharInfo->v;

		Verts[2].x = m_vPos.x+CharRenderInfo.x+CharRenderInfo.pCharInfo->width;
		Verts[2].y = m_vPos.y+CharRenderInfo.y;
		Verts[2].z = 0.0f;
		Verts[2].w = 1.0f;
		Verts[2].nColor = m_nColor;
		Verts[2].u = CharRenderInfo.pCharInfo->u+CharRenderInfo.pCharInfo->w;
		Verts[2].v = CharRenderInfo.pCharInfo->v;

		Verts[3].x = m_vPos.x+CharRenderInfo.x+CharRenderInfo.pCharInfo->width;
		Verts[3].y = m_vPos.y+CharRenderInfo.y+CharRenderInfo.pCharInfo->height;
		Verts[3].z = 0.0f;
		Verts[3].w = 1.0f;
		Verts[3].nColor = m_nColor;
		Verts[3].u = CharRenderInfo.pCharInfo->u+CharRenderInfo.pCharInfo->w;
		Verts[3].v = CharRenderInfo.pCharInfo->v+CharRenderInfo.pCharInfo->h;

		g_pOEUIRenderSystem->SetTexture(CharRenderInfo.pCharInfo->pTexture);
		g_pOEUIRenderSystem->DrawTriList(Verts, VERTS_COUNT, s_Indis, INDIS_COUNT);
	}
}

void COEUIStringRenderer_Impl::SetText(const tstring& strText)
{
	m_strText = strText;
	CreateRenderChars();
}

const tstring& COEUIStringRenderer_Impl::GetText() const
{
	return m_strText;
}

void COEUIStringRenderer_Impl::SetTextColor(uint nColor)
{
	m_nColor = nColor;
}

uint COEUIStringRenderer_Impl::GetTextColor() const
{
	return m_nColor;
}

bool COEUIStringRenderer_Impl::SetFont(IOEUIFont* pFont)
{
	m_pFont = pFont;
	return CreateRenderChars();
}

const IOEUIFont* COEUIStringRenderer_Impl::GetFont() const
{
	return m_pFont;
}

void COEUIStringRenderer_Impl::SetPosition(const CPoint& pos)
{
	m_vPos = pos;
}

const CPoint& COEUIStringRenderer_Impl::GetPosition()
{
	return m_vPos;
}

bool COEUIStringRenderer_Impl::CreateRenderChars()
{
	DestroyRenderChars();

	if (!m_pFont) return false;

	float fBaseOffsetX = 0.0f;
	float fBaseOffsetY = 0.0f;

	int nCount = (int)m_strText.size();

	for (int i = 0; i < nCount; ++i)
	{
		if (m_strText[i] == TS('\n'))
		{
			fBaseOffsetX = 0.0f;
			fBaseOffsetY += m_pFont->GetLineHeight();
			continue;
		}

		const IOEUIFont::CHAR_INFO* pCharInfo = m_pFont->GetCharInfo(m_strText[i]);
		if (!pCharInfo) continue;

		CHAR_RENDER_INFO CharRenderInfo;
		CharRenderInfo.x = fBaseOffsetX + pCharInfo->fOffsetX;
		CharRenderInfo.y = fBaseOffsetY + pCharInfo->fOffsetY;
		CharRenderInfo.pCharInfo = pCharInfo;
		m_vCharRenderInfo.push_back(CharRenderInfo);

		if (i < nCount - 1)
		{
			float fKerning = m_pFont->GetKerning(m_strText[i], m_strText[i+1]);
			fBaseOffsetX += (pCharInfo->fAdvance + fKerning);
		}
	}

	return true;
}

void COEUIStringRenderer_Impl::DestroyRenderChars()
{
	m_vCharRenderInfo.clear();
}
