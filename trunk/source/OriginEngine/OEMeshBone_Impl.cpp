/*!
 * \file OEMeshBone_Impl.cpp
 * \date 8-8-2009 22:04:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMeshBone_Impl.h"
#include <OEOS.h>

COEMeshBone_Impl::COEMeshBone_Impl(const COEFmtMesh::BONE& Bone, int nID, IOEFile* pFile)
{
	Init();
	m_bOK = Create(Bone, nID, pFile);
}

COEMeshBone_Impl::~COEMeshBone_Impl()
{
	Destroy();
}

void COEMeshBone_Impl::Init()
{
	m_nID = COEFmtMesh::INVALID_BONE_ID;
	m_nParentID = COEFmtMesh::INVALID_BONE_ID;
	m_fTimeLength = 0.0f;
}

void COEMeshBone_Impl::Destroy()
{
	m_vFrame.clear();
}

const tstring& COEMeshBone_Impl::GetName() const
{
	return m_strName;
}

int COEMeshBone_Impl::GetID() const
{
	return m_nID;
}

int COEMeshBone_Impl::GetParentID() const
{
	return m_nParentID;
}

float COEMeshBone_Impl::GetTimeLength() const
{
	return m_fTimeLength;
}

const CMatrix4x4& COEMeshBone_Impl::GetLocalMatrix() const
{
	return m_matLocal;
}

const CMatrix4x4& COEMeshBone_Impl::GetWorldMatrix() const
{
	return m_matWorld;
}

const CMatrix4x4& COEMeshBone_Impl::GetWorldMatrixInv() const
{
	return m_matWorldInv;
}

int COEMeshBone_Impl::GetFrameCount() const
{
	return (int)m_vFrame.size();
}

const IOEMeshBone::BONE_FRAME* COEMeshBone_Impl::GetFrame(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_vFrame.size()) return NULL;
	return &m_vFrame[nIndex];
}

bool COEMeshBone_Impl::SlerpMatrix(CMatrix4x4& matOut, float fTime, bool bLoop /*= true*/)
{
	int nFrameCount = (int)m_vFrame.size();
	if (nFrameCount <= 0)
	{
		matOut = m_matLocal;
		return true;
	}

	if (fTime >= m_fTimeLength)
	{
		if (!bLoop)
		{
			COEMath::BuildMatrixFromQuaternion(matOut, m_vFrame[nFrameCount-1].vRotation);
			COEMath::SetMatrixTranslation(matOut, m_vFrame[nFrameCount-1].vPos);
			COEMath::SetMatrixScale(matOut, m_vFrame[nFrameCount-1].vScale);
			return true;
		}

		fTime -= floorf(fTime/m_fTimeLength)*m_fTimeLength;
	}

	if (fTime <= 0.0f)
	{
		if (!bLoop)
		{
			COEMath::BuildMatrixFromQuaternion(matOut, m_vFrame[0].vRotation);
			COEMath::SetMatrixTranslation(matOut, m_vFrame[0].vPos);
			COEMath::SetMatrixScale(matOut, m_vFrame[0].vScale);
			return true;
		}

		fTime -= floorf(fTime/m_fTimeLength)*m_fTimeLength;
	}

	int nPrevIndex = 0;
	int nNextIndex = 0;

	for (int i = 0; i < nFrameCount-1; ++i)
	{
		if (fTime >= m_vFrame[i].fTime && fTime < m_vFrame[i+1].fTime)
		{
			nPrevIndex = i;
			nNextIndex = i+1;
			break;
		}
	}

	if (nPrevIndex == nNextIndex)
	{
		COEMath::BuildMatrixFromQuaternion(matOut, m_vFrame[nPrevIndex].vRotation);
		COEMath::SetMatrixTranslation(matOut, m_vFrame[nPrevIndex].vPos);
		COEMath::SetMatrixScale(matOut, m_vFrame[nPrevIndex].vScale);
		return true;
	}

	CQuaternion rRot;
	float t = (fTime - m_vFrame[nPrevIndex].fTime) / (m_vFrame[nNextIndex].fTime - m_vFrame[nPrevIndex].fTime);
	COEMath::QuaternionSlerp(rRot, m_vFrame[nPrevIndex].vRotation, m_vFrame[nNextIndex].vRotation, t);

	CVector3 vPos;
	COEMath::VectorLerp(vPos, m_vFrame[nPrevIndex].vPos, m_vFrame[nNextIndex].vPos, t);

	CVector3 vScale;
	COEMath::VectorLerp(vScale, m_vFrame[nPrevIndex].vScale, m_vFrame[nNextIndex].vScale, t);

	COEMath::BuildMatrixFromQuaternion(matOut, rRot);
	COEMath::SetMatrixTranslation(matOut, vPos);
	COEMath::SetMatrixScale(matOut, vScale);

	return true;
}

void COEMeshBone_Impl::SetWorldMatrix(const CMatrix4x4& matWorld)
{
	m_matWorld = matWorld;
	m_matWorldInv = m_matWorld.Inverse();
}

bool COEMeshBone_Impl::Create(const COEFmtMesh::BONE& Bone, int nID, IOEFile* pFile)
{
	COEOS::char2tchar(m_strName, Bone.szName);
	m_nID = nID;
	m_nParentID = Bone.nParentIndex;
	m_fTimeLength = Bone.fTimeLength;

	CVector3 vPos;
	CVector3 vScale;
	CQuaternion rRot;

	vPos.x = Bone.BoneTrans.vPos[0];
	vPos.y = Bone.BoneTrans.vPos[1];
	vPos.z = Bone.BoneTrans.vPos[2];

	vScale.x = Bone.BoneTrans.vScale[0];
	vScale.y = Bone.BoneTrans.vScale[1];
	vScale.z = Bone.BoneTrans.vScale[2];

	rRot.x = Bone.BoneTrans.vRotation[0];
	rRot.y = Bone.BoneTrans.vRotation[1];
	rRot.z = Bone.BoneTrans.vRotation[2];
	rRot.w = Bone.BoneTrans.vRotation[3];

	COEMath::BuildMatrixFromQuaternion(m_matLocal, rRot);
	COEMath::SetMatrixTranslation(m_matLocal, vPos);
	COEMath::SetMatrixScale(m_matLocal, vScale);

	pFile->Seek(Bone.nOffBoneFrame);
	m_vFrame.clear();
	for (int i = 0; i < Bone.nNumBoneFrame; ++i)
	{
		COEFmtMesh::BONE_FRAME BoneFrame;
		pFile->Read(&BoneFrame, sizeof(BoneFrame));

		BONE_FRAME InlBoneFrame;
		InlBoneFrame.fTime = BoneFrame.fTime;

		InlBoneFrame.vPos.x = BoneFrame.BoneTrans.vPos[0];
		InlBoneFrame.vPos.y = BoneFrame.BoneTrans.vPos[1];
		InlBoneFrame.vPos.z = BoneFrame.BoneTrans.vPos[2];

		InlBoneFrame.vScale.x = BoneFrame.BoneTrans.vScale[0];
		InlBoneFrame.vScale.y = BoneFrame.BoneTrans.vScale[1];
		InlBoneFrame.vScale.z = BoneFrame.BoneTrans.vScale[2];

		InlBoneFrame.vRotation.x = BoneFrame.BoneTrans.vRotation[0];
		InlBoneFrame.vRotation.y = BoneFrame.BoneTrans.vRotation[1];
		InlBoneFrame.vRotation.z = BoneFrame.BoneTrans.vRotation[2];
		InlBoneFrame.vRotation.w = BoneFrame.BoneTrans.vRotation[3];

		m_vFrame.push_back(InlBoneFrame);
	}

	return true;
}
