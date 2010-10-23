/*!
 * \file OENormalRender_Impl.cpp
 * \date 10-12-2010 20:15:25
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OENormalRender_Impl.h"
#include <OECore/IOEShaderMgr.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/OERenderSystemUtil.h>
#include <OECore/OEFmtMesh.h>

COENormalRender_Impl::COENormalRender_Impl()
{
	m_pShader = NULL;
	m_bOK = Init();
}

COENormalRender_Impl::~COENormalRender_Impl()
{
	Destroy();
}

bool COENormalRender_Impl::Init()
{
	m_pShader = g_pOEShaderMgr->CreateDefaultShader(DST_LINE);
	if (!m_pShader) return false;

	return true;
}

void COENormalRender_Impl::Destroy()
{
	// TODO: 
}

bool COENormalRender_Impl::Render(IOERenderData* pRenderData)
{
	IOEMesh* pMesh = pRenderData->GetMesh();
	if (!pMesh) return false;

	int nNumPiece = pMesh->GetNumPieces();

	CMatrix4x4 matModelToWorld = pRenderData->GetNode()->GetFinalMatrix();

	CMatrix4x4 matWorldToProject = matModelToWorld;
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_VIEW_PROJ);

	CDefaultRenderState DefaultState;

	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = pMesh->GetPiece(i);
		if (!pPiece) continue;

		if (((pPiece->GetVertDeclMask() & COEFmtMesh::VDM_NORMAL) == 0)
			|| ((pPiece->GetVertDeclMask() & COEFmtMesh::VDM_POSITION) == 0)) continue;

		m_vLines.clear();
		m_vIndex.clear();
		CreateNormalLine(m_vLines, m_vIndex, pPiece);

		m_pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);

		g_pOERenderSystem->SetShader(m_pShader);
		g_pOERenderSystem->DrawLineList(&m_vLines[0], (uint)m_vLines.size(), &m_vIndex[0], (uint)m_vIndex.size());
	}

	return true;
}

int COENormalRender_Impl::GetNormalOffset(int nVertDescMask)
{
	int nOffset = 0;
	if (nVertDescMask & COEFmtMesh::VDM_POSITION) nOffset += 3*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD0) nOffset += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD1) nOffset += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD2) nOffset += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD3) nOffset += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD4) nOffset += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD5) nOffset += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD6) nOffset += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD7) nOffset += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_COLOR) nOffset += sizeof(uint);

	return nOffset;
}

int COENormalRender_Impl::GetVertexSize(int nVertDescMask)
{
	int nSize = 0;
	if (nVertDescMask & COEFmtMesh::VDM_POSITION) nSize += 3*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD0) nSize += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD1) nSize += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD2) nSize += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD3) nSize += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD4) nSize += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD5) nSize += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD6) nSize += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TEXCOORD7) nSize += 2*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_COLOR) nSize += sizeof(uint);
	if (nVertDescMask & COEFmtMesh::VDM_NORMAL) nSize += 3*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_TANGENT) nSize += 3*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_BINORMAL) nSize += 3*sizeof(float);
	if (nVertDescMask & COEFmtMesh::VDM_SKELECTON) nSize += (4*sizeof(float)+4*sizeof(uchar));
	return nSize;
}

void COENormalRender_Impl::CreateNormalLine(TV_VERTEX_LINE& vLines, TV_INDEX& vIndex, IOEPiece* pPiece)
{
	int nNormalOffset = GetNormalOffset(pPiece->GetVertDeclMask());
	int nVertexSize = GetVertexSize(pPiece->GetVertDeclMask());
	int nNumVerts = pPiece->GetNumVerts();
	const uchar* pData = (const uchar*)pPiece->GetVerts();

	int nVertexOffset = 0;
	for (int i = 0; i < nNumVerts; ++i)
	{
		const float* pPosition = (const float*)(pData+nVertexOffset);
		const float* pNormal = (const float*)(pData+nVertexOffset+nNormalOffset);

		VERTEX_LINE Verts;
		Verts.x = pPosition[0];
		Verts.y = pPosition[1];
		Verts.z = pPosition[2];
		Verts.nColor = 0xFF00FF00;
		vLines.push_back(Verts);

		Verts.x = pPosition[0]+pNormal[0]*1.0f;
		Verts.y = pPosition[1]+pNormal[1]*1.0f;
		Verts.z = pPosition[2]+pNormal[2]*1.0f;
		vLines.push_back(Verts);

		vIndex.push_back(i*2);
		vIndex.push_back(i*2+1);

		nVertexOffset += nVertexSize;
	}
}
