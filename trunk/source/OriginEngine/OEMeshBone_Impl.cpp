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
	m_pParent = NULL;
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

IOEMeshBone* COEMeshBone_Impl::GetParent() const
{
	return m_pParent;
}

float COEMeshBone_Impl::GetTimeLength() const
{
	return m_fTimeLength;
}

const CMatrix4x4& COEMeshBone_Impl::GetLocalMatrix() const
{
	return m_matLocal;
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

int COEMeshBone_Impl::GetNumChildren() const
{
	return (int)m_vChildren.size();
}

IOEMeshBone* COEMeshBone_Impl::GetChild(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_vChildren.size()) return NULL;
	return m_vChildren[nIndex];
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

int COEMeshBone_Impl::GetParentID() const
{
	return m_nParentID;
}

void COEMeshBone_Impl::SetParent(COEMeshBone_Impl* pParent)
{
	if (m_pParent == pParent) return;

	if (m_pParent) m_pParent->RemoveChild(this);
	m_pParent = pParent;
	if (m_pParent) m_pParent->AddChild(this);
}

void COEMeshBone_Impl::AddChild(COEMeshBone_Impl* pChild)
{
	m_vChildren.push_back(pChild);
}

void COEMeshBone_Impl::RemoveChild(COEMeshBone_Impl* pChild)
{
	for (VMESH_BONE::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		if (pChild == (*it))
		{
			m_vChildren.erase(it);
			return;
		}
	}
}

bool COEMeshBone_Impl::Create(const COEFmtMesh::BONE& Bone, int nID, IOEFile* pFile)
{
	COEOS::char2tchar(m_strName, Bone.szName);
	m_nID = nID;
	m_nParentID = Bone.nParentIndex;
	m_fTimeLength = Bone.fTimeLength;

	m_vPos.x = Bone.BoneTrans.vPos[0];
	m_vPos.y = Bone.BoneTrans.vPos[1];
	m_vPos.z = Bone.BoneTrans.vPos[2];

	m_vScale.x = Bone.BoneTrans.vScale[0];
	m_vScale.y = Bone.BoneTrans.vScale[1];
	m_vScale.z = Bone.BoneTrans.vScale[2];

	m_vRot.x = Bone.BoneTrans.vRotation[0];
	m_vRot.y = Bone.BoneTrans.vRotation[1];
	m_vRot.z = Bone.BoneTrans.vRotation[2];
	m_vRot.w = Bone.BoneTrans.vRotation[3];

	COEMath::BuildMatrixFromQuaternion(m_matLocal, m_vRot);
	COEMath::SetMatrixTranslation(m_matLocal, m_vPos);
	COEMath::SetMatrixScale(m_matLocal, m_vScale);

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
