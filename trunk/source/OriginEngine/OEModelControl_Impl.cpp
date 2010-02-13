/*!
 * \file OEModelControl_Impl.cpp
 * \date 12-2-2010 20:47:10
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModelControl_Impl.h"
#include <OEFmtBone.h>

COEModelControl_Impl::COEModelControl_Impl()
:IOEControl(OECT_SKINMESH)
{
	// TODO: 
}

COEModelControl_Impl::~COEModelControl_Impl()
{
	// TODO: 
}

bool COEModelControl_Impl::Update(IOERenderData* pRenderData, float fDetailTime)
{
	COEModelRenderData_Impl* pData = (COEModelRenderData_Impl*)ConvertData(pRenderData);
	if (!pData) return false;

	float fTotalTime = pData->GetTotalTime() + fDetailTime;
	float fAnimLength = pData->GetAnimLength();

	if (fTotalTime > fAnimLength || fTotalTime < 0.0f)
	{
		fTotalTime -= floorf(fTotalTime/fAnimLength)*fAnimLength;
	}

	pData->SetTotalTime(fTotalTime);

	TV_BONE& vBones = pData->GetBones();
	TV_MATRIX& vSkinMatrix = pData->GetSkinMatrix();

	int nNumBones = (int)vBones.size();
	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pBone = vBones[i];

		if (fTotalTime > pBone->GetTimeLength())
		{
			pBone->SlerpMatrix(vSkinMatrix[i], fTotalTime, false);
		}
		else
		{
			pBone->SlerpMatrix(vSkinMatrix[i], fTotalTime, true);
		}

		int nParentID = pBone->GetParentID();
		if (nParentID != COEFmtBone::INVALID_BONE_ID)
		{
			vSkinMatrix[i] *= vSkinMatrix[nParentID];
		}
	}

	for (int i = 0; i < nNumBones; ++i)
	{
		IOEBone* pBone = vBones[i];
		vSkinMatrix[i] = pBone->GetWorldMatrixInv()*vSkinMatrix[i];
	}

	return true;
}

COEModelRenderData_Impl* COEModelControl_Impl::ConvertData(IOERenderData* pRenderData)
{
	if (!pRenderData) return NULL;

	return (COEModelRenderData_Impl*)pRenderData;
}
