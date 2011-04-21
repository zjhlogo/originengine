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
#include <OECore/IOENode.h>
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
	IOEAnimData* pAnimData = pRenderData->GetAnimData(TS("MainAnimData"));
	if (!pAnimData) return false;

	IOESkeleton* pSkeleton = pAnimData->GetSkeleton();
	if (!pSkeleton) return false;

	m_vVerts.clear();
	m_vIndis.clear();

	const CMatrix4x4* pSkinMatrixs = pAnimData->GetSkinMatrixs();

	int nNumBones = pSkeleton->GetBonesCount();
	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pBone = pSkeleton->GetBone(i);
		BuildBoneVerts(m_vVerts, m_vIndis, pSkeleton, pSkinMatrixs, pBone->GetID(), pBone->GetParentID());
	}

	CDefaultRenderState DefaultState;

	IOENode* pAttachedNode = (IOENode*)pRenderData->GetObject(TS("AttachedNode"));
	if (!pAttachedNode) return false;

	CMatrix4x4 matWorldToProject = pAttachedNode->GetFinalMatrix();
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_WORLD_VIEW_PROJ);

	m_pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);
	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawLineList(&m_vVerts[0], m_vVerts.size(), &m_vIndis[0], m_vIndis.size());

	return true;
}

bool COESkeletonRender_Impl::BuildBoneVerts(TV_VERTEX_LINE& vVertsOut, TV_VERTEX_INDEX& vIndisOut, IOESkeleton* pSkeleton, const CMatrix4x4* pSkinMatrixs, int nBoneID, int nParentBoneID)
{
	if (nParentBoneID == COEFmtSkeleton::INVALID_BONE_ID) return false;

	IOEBone* pBone = pSkeleton->GetBone(nBoneID);
	assert(pBone);
	CMatrix4x4 matSkin = pBone->GetWorldMatrix() * pSkinMatrixs[nBoneID];
	VERTEX_LINE Vertex;
	Vertex.x = matSkin.m[12];
	Vertex.y = matSkin.m[13];
	Vertex.z = matSkin.m[14];
	Vertex.nColor = 0xFF808080;

	int nSelfIndex = vVertsOut.size();
	vVertsOut.push_back(Vertex);
	vIndisOut.push_back(nSelfIndex);

	IOEBone* pBoneParent = pSkeleton->GetBone(nParentBoneID);
	assert(pBoneParent);
	matSkin = pBoneParent->GetWorldMatrix() * pSkinMatrixs[nParentBoneID];
	Vertex.x = matSkin.m[12];
	Vertex.y = matSkin.m[13];
	Vertex.z = matSkin.m[14];
	Vertex.nColor = 0xFF808080;

	int nParentIndex = vVertsOut.size();
	vVertsOut.push_back(Vertex);
	vIndisOut.push_back(nParentIndex);

	return true;
}
