/*!
 * \file OESkinMeshControl_Impl.cpp
 * \date 13-2-2010 19:56:26
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkinMeshControl_Impl.h"
#include <OECore/OEFmtSkeleton.h>

COESkinMeshControl_Impl::COESkinMeshControl_Impl()
{
	// TODO: 
}

COESkinMeshControl_Impl::~COESkinMeshControl_Impl()
{
	// TODO: 
}

bool COESkinMeshControl_Impl::Update(IOERenderData* pRenderData, float fDetailTime)
{
	float fTotalTime = pRenderData->GetTotalTime() + fDetailTime;
	float fAnimLength = pRenderData->GetAnimLength();

	if (fTotalTime > fAnimLength || fTotalTime < 0.0f)
	{
		fTotalTime -= floorf(fTotalTime/fAnimLength)*fAnimLength;
	}

	pRenderData->SetTotalTime(fTotalTime);

	IOESkeleton* pSkeleton = pRenderData->GetSkeleton();
	TV_MATRIX4X4& vSkinMatrix = pRenderData->GetSkinMatrix();

	int nNumBones = (int)pSkeleton->GetBonesCount();
	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pBone = pSkeleton->GetBone(i);

		if (fTotalTime > pBone->GetTimeLength())
		{
			pBone->SlerpMatrix(vSkinMatrix[i], fTotalTime, false);
		}
		else
		{
			pBone->SlerpMatrix(vSkinMatrix[i], fTotalTime, true);
		}

		int nParentID = pBone->GetParentID();
		if (nParentID != COEFmtSkeleton::INVALID_BONE_ID)
		{
			vSkinMatrix[i] *= vSkinMatrix[nParentID];
		}
	}

	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pBone = pSkeleton->GetBone(i);
		vSkinMatrix[i] = pBone->GetWorldMatrixInv()*vSkinMatrix[i];
	}

	return true;
}
