/*!
 * \file SimpleShape.cpp
 * \date 6-7-2009 11:22:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "SimpleShape.h"
#include <OECore/IOERenderSystem.h>
#include <OECore/IOEShaderMgr.h>
#include <OECore/OERenderSystemUtil.h>

CSimpleShape::CSimpleShape()
{
	m_pShader = NULL;
	m_eShape = S_CUBE;
	m_fScale = 1.0f;
	m_nColor = 0xFFFFFFFF;

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

void CSimpleShape::Update(float fDetailTime)
{
	// TODO: 
}

void CSimpleShape::Render(float fDetailTime)
{
	switch (m_eShape)
	{
	case S_CUBE:
		{
			DrawCube();
		}
		break;
	//case S_SPHERE:
	//	{
	//		// TODO: 
	//	}
	//	break;
	}
}

IOERenderData* CSimpleShape::GetRenderData()
{
	return NULL;
}

void CSimpleShape::SetShape(SHAPE eShape)
{
	m_eShape = eShape;
}

CSimpleShape::SHAPE CSimpleShape::GetShape()
{
	return m_eShape;
}

void CSimpleShape::SetPosition(const CVector3& vPos)
{
	m_vPos = vPos;
}

const CVector3& CSimpleShape::GetPosition()
{
	return m_vPos;
}

void CSimpleShape::SetScale(float fScale)
{
	m_fScale = fScale;
}

float CSimpleShape::GetScale()
{
	return m_fScale;
}

void CSimpleShape::SetColor(uint nColor)
{
	m_nColor = nColor;
}

uint CSimpleShape::GetColor()
{
	return m_nColor;
}

void CSimpleShape::DrawCube()
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
		Verts[i].x = s_Verts[i].x*m_fScale+m_vPos.x;
		Verts[i].y = s_Verts[i].y*m_fScale+m_vPos.y;
		Verts[i].z = s_Verts[i].z*m_fScale+m_vPos.z;
		Verts[i].nColor = m_nColor;
	}

	CDefaultRenderState DefaultState;

	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawTriList(Verts, CUBE_VERTS_COUNT, s_Indis, CUBE_INDIS_COUNT);
}
