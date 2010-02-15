/*!
 * \file OESkelectonRender_Impl.cpp
 * \date 14-2-2010 8:56:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkelectonRender_Impl.h"
#include "OEBaseTypeEx.h"
#include <IOEShaderMgr.h>
#include <IOERenderSystem.h>
#include <OEFmtBone.h>

COESkelectonRender_Impl::COESkelectonRender_Impl()
:IOERender(OERT_SKELECTON)
{
	Init();
}

COESkelectonRender_Impl::~COESkelectonRender_Impl()
{
	Destroy();
}

void COESkelectonRender_Impl::Init()
{
	m_pShader = NULL;
}

void COESkelectonRender_Impl::Destroy()
{
	SAFE_RELEASE(m_pShader);
}

bool COESkelectonRender_Impl::Render(IOERenderData* pRenderData)
{
	static const VERT_DECL_ELEMENT s_Decl[] =
	{
		VDT_FLOAT3, VDU_POSITION, 0,
		VDT_COLOR, VDU_COLOR, 0,
		VDT_UNKNOWN, VDU_UNKNOWN, 0,
	};

	COESkinMeshRenderData_Impl* pData = ConvertData(pRenderData);
	if (!pData) return false;

	if (!m_pShader)
	{
		m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("line.fx"));
		if (!m_pShader) return false;
	}

	m_vVerts.clear();
	m_vIndis.clear();

	const TV_BONE& vBones = pData->GetBones();
	const TV_MATRIX& vmatSkins = pData->GetSkinMatrix();

	for (TV_BONE::const_iterator it = vBones.begin(); it != vBones.end(); ++it)
	{
		IOEBone* pBone = (*it);
		BuildBoneVerts(m_vVerts, m_vIndis, vBones, vmatSkins, pBone->GetID(), pBone->GetParentID());
	}

	CMatrix4x4 matWorldViewProj;
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_WORLD_VIEW_PROJ);
	m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
	g_pOERenderSystem->SetShader(m_pShader);

	g_pOERenderSystem->DrawLineList(&m_vVerts[0], m_vVerts.size(), &m_vIndis[0], m_vIndis.size());

	return true;
}

COESkinMeshRenderData_Impl* COESkelectonRender_Impl::ConvertData(IOERenderData* pRenderData)
{
	if (!pRenderData) return NULL;

	if (pRenderData->GetType() != OERDT_SKINMESH) return NULL;

	return (COESkinMeshRenderData_Impl*)pRenderData;
}

bool COESkelectonRender_Impl::BuildBoneVerts(TV_VERTEX& vVertsOut, TV_SHORT& vIndisOut, const TV_BONE& vBones, const TV_MATRIX& vmatSkins, int nBoneID, int nParentBoneID)
{
	if (nParentBoneID == COEFmtBone::INVALID_BONE_ID) return false;

	CMatrix4x4 matSkin = vBones[nBoneID]->GetWorldMatrix() * vmatSkins[nBoneID];
	VERTEX Vertex;
	Vertex.x = matSkin.m[12];
	Vertex.y = matSkin.m[13];
	Vertex.z = matSkin.m[14];
	Vertex.color = 0xFFFFFFFF;

	int nSelfIndex = vVertsOut.size();
	vVertsOut.push_back(Vertex);
	vIndisOut.push_back(nSelfIndex);

	matSkin = vBones[nParentBoneID]->GetWorldMatrix() * vmatSkins[nParentBoneID];
	Vertex.x = matSkin.m[12];
	Vertex.y = matSkin.m[13];
	Vertex.z = matSkin.m[14];
	Vertex.color = 0xFFFFFFFF;

	int nParentIndex = vVertsOut.size();
	vVertsOut.push_back(Vertex);
	vIndisOut.push_back(nParentIndex);

	return true;
}
