/*!
 * \file SimpleShape.cpp
 * \date 6-7-2009 11:22:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "SimpleShape.h"
#include <IOERenderSystem.h>

CSimpleShape::CSimpleShape()
{
	m_bOK = Init();
}

CSimpleShape::~CSimpleShape()
{
	Destroy();
}

bool CSimpleShape::Init()
{
	m_pShader = g_pOEShaderMgr->CreateDefaultShader(DST_POLYC);
	if (!m_pShader) return false;

	return true;
}

void CSimpleShape::Destroy()
{
	SAFE_RELEASE(m_pShader);
}

bool CSimpleShape::IsOK()
{
	return m_bOK;
}

void CSimpleShape::DrawCube(const CVector3& vPos, float fScale /* = 1.0f */, uint nColor /* = 0xFFFFFFFF */)
{
	static const VERTEX_POLYC s_Verts[CUBE_VERTS_COUNT] = 
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

	static const ushort s_Indis[CUBE_INDIS_COUNT] =
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

	VERTEX_POLYC Verts[CUBE_VERTS_COUNT];
	for (int i = 0; i < CUBE_VERTS_COUNT; ++i)
	{
		Verts[i].x = s_Verts[i].x*fScale+vPos.x;
		Verts[i].y = s_Verts[i].y*fScale+vPos.y;
		Verts[i].z = s_Verts[i].z*fScale+vPos.z;
		Verts[i].nColor = nColor;
	}

	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawTriList(&Verts, CUBE_VERTS_COUNT, s_Indis, CUBE_INDIS_COUNT);
}
