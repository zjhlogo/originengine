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
	IOEAnimData* pAnimData = pRenderData->GetAnimData(TS("MainAnimData"));
	if (!pAnimData) return false;

	float fCurrTime = pAnimData->GetCurrTime() + fDetailTime;
	float fAnimLength = pAnimData->GetAnimLength();

	if (fCurrTime > fAnimLength || fCurrTime < 0.0f)
	{
		fCurrTime -= floorf(fCurrTime/fAnimLength)*fAnimLength;
	}

	pAnimData->SetCurrTime(fCurrTime);

	IOESkeleton* pSkeleton = pAnimData->GetSkeleton();
	CMatrix4x4* pSkinMatrixs = pAnimData->GetSkinMatrixs();

	int nNumBones = pSkeleton->GetBonesCount();
	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pBone = pSkeleton->GetBone(i);

		if (fCurrTime > pBone->GetTimeLength())
		{
			pBone->SlerpMatrix(pSkinMatrixs[i], fCurrTime, false);
		}
		else
		{
			pBone->SlerpMatrix(pSkinMatrixs[i], fCurrTime, true);
		}

		int nParentID = pBone->GetParentID();
		if (nParentID != COEFmtSkeleton::INVALID_BONE_ID)
		{
			pSkinMatrixs[i] *= pSkinMatrixs[nParentID];
		}
	}

	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pBone = pSkeleton->GetBone(i);
		pSkinMatrixs[i] = pBone->GetWorldMatrixInv()*pSkinMatrixs[i];
	}

	return true;
}
