/*!
 * \file Mesh.cpp
 * \date 10-6-2010 22:26:56
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Mesh.h"
#include <OECore/IOETextureMgr.h>
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOEResMgr.h>
#include <OECore/OERenderSystemUtil.h>

CMesh::CMesh()
{
	m_pTexture = NULL;
	m_pShader = NULL;
	m_pMesh = NULL;
	m_bOK = Init();
}

CMesh::~CMesh()
{
}

bool CMesh::Init()
{
	static const VERT_DECL_ELEMENT s_Decl[] =
	{
		{VDT_FLOAT3, VDU_POSITION, 0},
		{VDT_FLOAT2, VDU_TEXCOORD, 0},
		{VDT_FLOAT3, VDU_NORMAL, 0},
		{VDT_FLOAT3, VDU_TEXCOORD, 1},
		{VDT_UBYTE4, VDU_BLENDINDICES, 0},
		{VDT_FLOAT4, VDU_BLENDWEIGHT, 0},
		{VDT_UNKNOWN, VDU_UNKNOWN, 0},
	};

	m_pTexture = g_pOETextureMgr->CreateTextureFromFile(TS("casual03_diff_spec.png"));
	if (!m_pTexture) return false;

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("demo_mesh.fx"));
	if (!m_pShader) return false;

	m_pMesh = g_pOEResMgr->CreateMesh(TS("casual03_medium.mesh"));
	if (!m_pMesh) return false;

	return true;
}

void CMesh::Destroy()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTexture);
}

void CMesh::Update(float fDetailTime)
{
	// TODO: 
}

void CMesh::Render(float fDetailTime)
{
	int nNumPiece = m_pMesh->GetNumPieces();

	CMatrix4x4 matWorldViewProj;
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_WORLD_VIEW_PROJ);

	CDefaultRenderState DefaultState;

	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = m_pMesh->GetPiece(i);
		if (!pPiece) continue;

		m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
		m_pShader->SetTexture(TS("g_texDiffuse"), m_pTexture);

		g_pOERenderSystem->SetShader(m_pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}
}

IOERenderData* CMesh::GetRenderData()
{
	return NULL;
}

const CVector3& CMesh::GetBoundingBoxMin()
{
	return m_pMesh->GetBoundingBoxMin();
}

const CVector3& CMesh::GetBoundingBoxMax()
{
	return 	m_pMesh->GetBoundingBoxMax();
}
