/*!
 * \file OEUIRenderer_Impl.cpp
 * \date 27-7-2009 18:06:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIRenderer_Impl.h"
#include <OEInterfaces.h>

COEUIRenderer_Impl::COEUIRenderer_Impl()
{
	g_pOEUIRenderer = this;
	Init();
}

COEUIRenderer_Impl::~COEUIRenderer_Impl()
{
	Destroy();
	g_pOEUIRenderer = NULL;
}

void COEUIRenderer_Impl::Init()
{
	m_pDecl = NULL;
	m_pTexture = NULL;
}

void COEUIRenderer_Impl::Destroy()
{
	SAFE_RELEASE(m_pDecl);
}

void COEUIRenderer_Impl::SetTexture(IOETexture* pTexture)
{
	m_pTexture = pTexture;
}

IOETexture* COEUIRenderer_Impl::GetTexture() const
{
	return m_pTexture;
}

void COEUIRenderer_Impl::DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	if (!m_pDecl) Create();

	// TODO: cache it
	g_pOERenderer->SetVertDecl(m_pDecl);
	g_pOERenderer->SetTexture(m_pTexture);
	g_pOERenderer->DrawTriList(pVerts, nVerts, pIndis, nIndis);
}

void COEUIRenderer_Impl::FlushAll()
{
	// TODO: 
}

bool COEUIRenderer_Impl::Create()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT4, IOEVertDecl::U_POSITIONT, 0,
		IOEVertDecl::T_COLOR, IOEVertDecl::U_COLOR, 0,
		IOEVertDecl::T_FLOAT2, IOEVertDecl::U_TEXCOORD, 0,		// normal
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	if (!m_pDecl) return false;

	return true;
}