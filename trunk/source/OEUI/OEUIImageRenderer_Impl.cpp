/*!
 * \file OEUIImageRenderer_Impl.cpp
 * \date 10-26-2010 0:13:27
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEUIImageRenderer_Impl.h"
#include <OEUI/IOEUIRenderSystem.h>

COEUIImageRenderer_Impl::COEUIImageRenderer_Impl()
{
	m_bOK = Init();
}

COEUIImageRenderer_Impl::~COEUIImageRenderer_Impl()
{
	Destroy();
}

bool COEUIImageRenderer_Impl::Init()
{
	m_pTexture = NULL;
	return true;
}

void COEUIImageRenderer_Impl::Destroy()
{
	// TODO: 
}

void COEUIImageRenderer_Impl::Update(float fDetailTime)
{
	// TODO: 
}

void COEUIImageRenderer_Impl::Render(float fDetailTime)
{
	static const int VERTS_COUNT = 4;
	static const int INDIS_COUNT = 6;
	static const ushort s_Indis[INDIS_COUNT] = {0, 1, 3, 1, 2, 3};

	if (!m_pTexture) return;

	VERTEX_POLY_UI Verts[VERTS_COUNT];
	float fDepth = g_pOEUIRenderSystem->NextDepth();

	Verts[0].x = m_vPos.x;
	Verts[0].y = m_vPos.y+m_Size.height;
	Verts[0].z = fDepth;
	Verts[0].w = 1.0f;
	Verts[0].nColor = 0xFFFFFFFF;
	Verts[0].u = 0.0f;
	Verts[0].v = 1.0f;

	Verts[1].x = m_vPos.x;
	Verts[1].y = m_vPos.y;
	Verts[1].z = fDepth;
	Verts[1].w = 1.0f;
	Verts[1].nColor = 0xFFFFFFFF;
	Verts[1].u = 0.0f;
	Verts[1].v = 0.0f;

	Verts[2].x = m_vPos.x+m_Size.width;
	Verts[2].y = m_vPos.y;
	Verts[2].z = fDepth;
	Verts[2].w = 1.0f;
	Verts[2].nColor = 0xFFFFFFFF;
	Verts[2].u = 1.0f;
	Verts[2].v = 0.0f;

	Verts[3].x = m_vPos.x+m_Size.width;
	Verts[3].y = m_vPos.y+m_Size.height;
	Verts[3].z = fDepth;
	Verts[3].w = 1.0f;
	Verts[3].nColor = 0xFFFFFFFF;
	Verts[3].u = 1.0f;
	Verts[3].v = 1.0f;

	g_pOEUIRenderSystem->SetTexture(m_pTexture);
	g_pOEUIRenderSystem->DrawSolidTriList(Verts, VERTS_COUNT, s_Indis, INDIS_COUNT);
}

void COEUIImageRenderer_Impl::SetTexture(IOETexture* pTexture, bool bAdjustSizeFromTexture /* = false */)
{
	m_pTexture = pTexture;
	if (bAdjustSizeFromTexture && m_pTexture)
	{
		m_Size.width = (float)m_pTexture->GetWidth();
		m_Size.height = (float)m_pTexture->GetHeight();
	}
}

IOETexture* COEUIImageRenderer_Impl::GetTexture()
{
	return m_pTexture;
}

void COEUIImageRenderer_Impl::SetPosition(const CPoint& pos)
{
	m_vPos = pos;
}

const CPoint& COEUIImageRenderer_Impl::GetPosition()
{
	return m_vPos;
}

void COEUIImageRenderer_Impl::SetSize(const CSize& size)
{
	m_Size = size;
}

const CSize& COEUIImageRenderer_Impl::GetSize()
{
	return m_Size;
}
