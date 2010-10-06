/*!
 * \file OESkeletonRender_Impl.cpp
 * \date 1-3-2010 19:23:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkeletonRender_Impl.h"

#include <OECore/IOEShaderMgr.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/OERenderSystemUtil.h>
#include <OECore/OEFmtSkeleton.h>
#include <assert.h>

COESkeletonRender_Impl::COESkeletonRender_Impl()
{
	m_bOK = Init();
}

COESkeletonRender_Impl::~COESkeletonRender_Impl()
{
	Destroy();
}

bool COESkeletonRender_Impl::Init()
{
	m_pShader = g_pOEShaderMgr->CreateDefaultShader(DST_LINE);
	if (!m_pShader) return false;

	return true;
}

void COESkeletonRender_Impl::Destroy()
{
	SAFE_RELEASE(m_pShader);
}

bool COESkeletonRender_Impl::Render(IOERenderData* pRenderData)
{
	COESkinMeshRenderData_Impl* pData = ConvertData(pRenderData);
	if (!pData) return false;

	m_vVerts.clear();
	m_vIndis.clear();

	IOESkeleton* pSkeleton = pData->GetSkeleton();
	const TV_MATRIX4X4& vmatSkins = pData->GetSkinMatrix();

	int nNumBones = pSkeleton->GetBonesCount();
	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pBone = pSkeleton->GetBone(i);
		BuildBoneVerts(m_vVerts, m_vIndis, pSkeleton, vmatSkins, pBone->GetID(), pBone->GetParentID());
	}

	CDefaultRenderState DefaultState;

	CMatrix4x4 matWorldViewProj;
	pRenderData->GetNode()->GetTransform(matWorldViewProj);
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_WORLD_VIEW_PROJ);

	m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawLineList(&m_vVerts[0], m_vVerts.size(), &m_vIndis[0], m_vIndis.size());

	return true;
}

COESkinMeshRenderData_Impl* COESkeletonRender_Impl::ConvertData(IOERenderData* pRenderData)
{
	if (!pRenderData) return NULL;

	if (!pRenderData->GetRtti()->IsType(TS("COESkinMeshRenderData_Impl"))) return NULL;

	return (COESkinMeshRenderData_Impl*)pRenderData;
}

bool COESkeletonRender_Impl::BuildBoneVerts(TV_VERTEX_LINE& vVertsOut, TV_VERTEX_INDEX& vIndisOut, IOESkeleton* pSkeleton, const TV_MATRIX4X4& vmatSkins, int nBoneID, int nParentBoneID)
{
	if (nParentBoneID == COEFmtSkeleton::INVALID_BONE_ID) return false;

	IOEBone* pBone = pSkeleton->GetBone(nBoneID);
	assert(pBone);
	CMatrix4x4 matSkin = pBone->GetWorldMatrix() * vmatSkins[nBoneID];
	VERTEX_LINE Vertex;
	Vertex.x = matSkin.m[12];
	Vertex.y = matSkin.m[13];
	Vertex.z = matSkin.m[14];
	Vertex.nColor = 0xFFFFFFFF;

	int nSelfIndex = vVertsOut.size();
	vVertsOut.push_back(Vertex);
	vIndisOut.push_back(nSelfIndex);

	IOEBone* pBoneParent = pSkeleton->GetBone(nParentBoneID);
	assert(pBoneParent);
	matSkin = pBoneParent->GetWorldMatrix() * vmatSkins[nParentBoneID];
	Vertex.x = matSkin.m[12];
	Vertex.y = matSkin.m[13];
	Vertex.z = matSkin.m[14];
	Vertex.nColor = 0xFFFFFFFF;

	int nParentIndex = vVertsOut.size();
	vVertsOut.push_back(Vertex);
	vIndisOut.push_back(nParentIndex);

	return true;
}
