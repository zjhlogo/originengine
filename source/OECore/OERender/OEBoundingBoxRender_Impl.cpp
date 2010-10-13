/*!
 * \file OEBoundingBoxRender_Impl.cpp
 * \date 2-3-2010 9:12:46
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEBoundingBoxRender_Impl.h"
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/OERenderSystemUtil.h>

COEBoundingBoxRender_Impl::COEBoundingBoxRender_Impl()
{
	m_pShader = NULL;
	m_bOK = Init();
}

COEBoundingBoxRender_Impl::~COEBoundingBoxRender_Impl()
{
	Destroy();
}

bool COEBoundingBoxRender_Impl::Init()
{
	m_pShader = g_pOEShaderMgr->CreateDefaultShader(DST_LINE);
	if (!m_pShader) return false;

	return true;
}

void COEBoundingBoxRender_Impl::Destroy()
{
	// TODO: 
}

bool COEBoundingBoxRender_Impl::Render(IOERenderData* pRenderData)
{
	static const ushort s_Indis[24] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7,
	};

	IOEMesh* pMesh = pRenderData->GetMesh();
	if (!pMesh) return false;

	const CVector3 vBBoxMin = pMesh->GetBoundingBoxMin();
	const CVector3 vBBoxMax = pMesh->GetBoundingBoxMax();

	VERTEX_LINE Verts[8];

	// vertex 0
	Verts[0].x = vBBoxMax.x;
	Verts[0].y = vBBoxMax.y;
	Verts[0].z = vBBoxMax.z;
	Verts[0].nColor = 0xFF808080;

	// vertex 1
	Verts[1].x = vBBoxMax.x;
	Verts[1].y = vBBoxMin.y;
	Verts[1].z = vBBoxMax.z;
	Verts[1].nColor = 0xFF808080;

	// vertex 2
	Verts[2].x = vBBoxMin.x;
	Verts[2].y = vBBoxMin.y;
	Verts[2].z = vBBoxMax.z;
	Verts[2].nColor = 0xFF808080;

	// vertex 3
	Verts[3].x = vBBoxMin.x;
	Verts[3].y = vBBoxMax.y;
	Verts[3].z = vBBoxMax.z;
	Verts[3].nColor = 0xFF808080;

	// vertex 4
	Verts[4].x = vBBoxMax.x;
	Verts[4].y = vBBoxMax.y;
	Verts[4].z = vBBoxMin.z;
	Verts[4].nColor = 0xFF808080;

	// vertex 5
	Verts[5].x = vBBoxMax.x;
	Verts[5].y = vBBoxMin.y;
	Verts[5].z = vBBoxMin.z;
	Verts[5].nColor = 0xFF808080;

	// vertex 6
	Verts[6].x = vBBoxMin.x;
	Verts[6].y = vBBoxMin.y;
	Verts[6].z = vBBoxMin.z;
	Verts[6].nColor = 0xFF808080;

	// vertex 7
	Verts[7].x = vBBoxMin.x;
	Verts[7].y = vBBoxMax.y;
	Verts[7].z = vBBoxMin.z;
	Verts[7].nColor = 0xFF808080;

	CDefaultRenderState DefaultState;

	CMatrix4x4 matWorldViewProj = pRenderData->GetNode()->GetFinalMatrix();
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_VIEW_PROJ);

	m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawLineList(Verts, 8, s_Indis, 24);

	return true;
}
