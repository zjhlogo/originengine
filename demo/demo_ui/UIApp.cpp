/*!
 * \file UIApp.cpp
 * \date 27-7-2009 15:38:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "UIApp.h"
#include <OEInterfaces.h>

CUIApp::CUIApp()
{
	Init();
}

CUIApp::~CUIApp()
{
	Destroy();
}

void CUIApp::Init()
{
	m_pFont = NULL;
}

void CUIApp::Destroy()
{
	// TODO: 
}

bool CUIApp::Initialize()
{
	m_pFont = g_pOEUIFontMgr->CreateBitmapFont(_T("12px_Tahoma.fnt"));
	if (!m_pFont) return false;

	g_pOERenderer->SetCullMode(IOERenderer::CMT_NONE);

	return true;
}

void CUIApp::Terminate()
{
	SAFE_RELEASE(m_pFont);
}

void CUIApp::Update(float fDetailTime)
{
	// TODO: 
}

void CUIApp::Render(float fDetailTime)
{
	static OEUI_VERTEX s_Verts[4];
	static ushort s_Indis[] = {0, 1, 3, 1, 2, 3};

	const IOEUIFont::CHAR_INFO* pCharInfo = m_pFont->GetCharInfo(_T('A'));

	s_Verts[0].x = 100.0f+pCharInfo->fOffsetX;
	s_Verts[0].y = 100.0f+pCharInfo->fOffsetY+pCharInfo->height;
	s_Verts[0].z = 1.0f;
	s_Verts[0].w = 1.0f;
	s_Verts[0].nColor = 0xFFFFFFFF;
	s_Verts[0].u = pCharInfo->u;
	s_Verts[0].v = pCharInfo->v+pCharInfo->h;

	s_Verts[1].x = 100.0f+pCharInfo->fOffsetX;
	s_Verts[1].y = 100.0f+pCharInfo->fOffsetY;
	s_Verts[1].z = 1.0f;
	s_Verts[1].w = 1.0f;
	s_Verts[1].nColor = 0xFFFFFFFF;
	s_Verts[1].u = pCharInfo->u;
	s_Verts[1].v = pCharInfo->v;

	s_Verts[2].x = 100.0f+pCharInfo->fOffsetX+pCharInfo->width;
	s_Verts[2].y = 100.0f+pCharInfo->fOffsetY;
	s_Verts[2].z = 1.0f;
	s_Verts[2].w = 1.0f;
	s_Verts[2].nColor = 0xFFFFFFFF;
	s_Verts[2].u = pCharInfo->u+pCharInfo->w;
	s_Verts[2].v = pCharInfo->v;

	s_Verts[3].x = 100.0f+pCharInfo->fOffsetX+pCharInfo->width;
	s_Verts[3].y = 100.0f+pCharInfo->fOffsetY+pCharInfo->height;
	s_Verts[3].z = 1.0f;
	s_Verts[3].w = 1.0f;
	s_Verts[3].nColor = 0xFFFFFFFF;
	s_Verts[3].u = pCharInfo->u+pCharInfo->w;
	s_Verts[3].v = pCharInfo->v+pCharInfo->h;

	g_pOEUIRenderer->SetTexture(pCharInfo->pTexture);
	g_pOEUIRenderer->DrawTriList(s_Verts, 4, s_Indis, 6);
}
