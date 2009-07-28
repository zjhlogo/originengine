/*!
 * \file SimpleShape.cpp
 * \date 6-7-2009 11:22:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "SimpleShape.h"

CSimpleShape::CSimpleShape()
{
	Init();
}

CSimpleShape::~CSimpleShape()
{
	Destroy();
}

void CSimpleShape::Init()
{
	m_pDecl = NULL;
	m_bOK = false;
}

void CSimpleShape::Destroy()
{
	SAFE_RELEASE(m_pDecl);
}

bool CSimpleShape::Initialize()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
		IOEVertDecl::T_COLOR, IOEVertDecl::U_COLOR, 0,
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	if (!m_pDecl) return false;

	m_bOK = true;
	return true;
}

void CSimpleShape::DrawCube(IOERenderer* pRenderer, const CVector3& vPos, float fScale /* = 1.0f */, uint nColor /* = 0xFFFFFFFF */)
{
	static VERTEX s_VertsTemplate[8] = 
	{
		{-0.5f, -0.5f, -0.5f, 0xFFFFFFFF},
		{-0.5f, -0.5f,  0.5f, 0xFFFFFFFF},
		{ 0.5f, -0.5f,  0.5f, 0xFFFFFFFF},
		{ 0.5f, -0.5f, -0.5f, 0xFFFFFFFF},
		{-0.5f,  0.5f, -0.5f, 0xFFFFFFFF},
		{-0.5f,  0.5f,  0.5f, 0xFFFFFFFF},
		{ 0.5f,  0.5f,  0.5f, 0xFFFFFFFF},
		{ 0.5f,  0.5f, -0.5f, 0xFFFFFFFF}
	};

	static ushort s_Indis[36] =
	{
		0, 2, 1,
		0, 3, 2,
		4, 5, 6,
		4, 6, 7,
		0, 5, 4,
		0, 1, 5,
		3, 7, 6,
		3, 6, 2,
		0, 4, 7,
		0, 7, 3,
		1, 6, 5,
		1, 2, 6
	};

	if (!m_bOK || !pRenderer) return;

	VERTEX Verts[8];
	for (int i = 0; i < 8; ++i)
	{
		Verts[i].x = s_VertsTemplate[i].x*fScale+vPos.x;
		Verts[i].y = s_VertsTemplate[i].y*fScale+vPos.y;
		Verts[i].z = s_VertsTemplate[i].z*fScale+vPos.z;
		Verts[i].nColor = nColor;
	}

	pRenderer->SetVertDecl(m_pDecl);
	pRenderer->SetTexture(NULL);
	pRenderer->DrawTriList(&Verts, 8, s_Indis, 36);
}
