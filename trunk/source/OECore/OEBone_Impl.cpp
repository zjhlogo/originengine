/*!
 * \file OEBone_Impl.cpp
 * \date 10-23-2009 13:56:31
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEBone_Impl.h"
#include <libOEBase/OEOS.h>

COEBone_Impl::COEBone_Impl(const COEFmtSkeleton::BONE& Bone, int nID, IOEFile* pFile)
:IOEBone(TS("COEBone_Impl"))
{
	Init();
	m_bOK = Create(Bone, nID, pFile);
}

COEBone_Impl::~COEBone_Impl()
{
	Destroy();
}

void COEBone_Impl::Init()
{
	m_nID = COEFmtSkeleton::INVALID_BONE_ID;
	m_nParentID = COEFmtSkeleton::INVALID_BONE_ID;
	m_fTimeLength = 0.0f;

	m_vKeyFrameRot.clear();
	m_vKeyFramePos.clear();
	m_vKeyFrameScale.clear();
}

void COEBone_Impl::Destroy()
{
	m_vKeyFrameRot.clear();
	m_vKeyFramePos.clear();
	m_vKeyFrameScale.clear();
}

const tstring& COEBone_Impl::GetName() const
{
	return m_strName;
}

int COEBone_Impl::GetID() const
{
	return m_nID;
}

int COEBone_Impl::GetParentID() const
{
	return m_nParentID;
}

float COEBone_Impl::GetTimeLength() const
{
	return m_fTimeLength;
}

const CMatrix4x4& COEBone_Impl::GetLocalMatrix() const
{
	return m_matLocal;
}

const CMatrix4x4& COEBone_Impl::GetWorldMatrix() const
{
	return m_matWorld;
}

const CMatrix4x4& COEBone_Impl::GetWorldMatrixInv() const
{
	return m_matWorldInv;
}

bool COEBone_Impl::SlerpMatrix(CMatrix4x4& matOut, float fTime, bool bLoop /*= true*/)
{
	CQuaternion qRot;
	CVector3 vPos;
	//CVector3 vScale;

	if (!SlerpRot(qRot, fTime)) qRot = m_qLocalRot;
	if (!LerpPos(vPos, fTime)) vPos = m_vLocalPos;
	//LerpScale(vScale, fTime);

	COEMath::BuildMatrixFromQuaternion(matOut, qRot);
	COEMath::SetMatrixTranslation(matOut, vPos);
	//COEMath::SetMatrixScale(matOut, vScale);

	return true;
}

void COEBone_Impl::SetWorldMatrix(const CMatrix4x4& matWorld)
{
	m_matWorld = matWorld;
	m_matWorldInv = m_matWorld.Inverse();
}

bool COEBone_Impl::Create(const COEFmtSkeleton::BONE& Bone, int nID, IOEFile* pFile)
{
	COEOS::char2tchar(m_strName, Bone.szName);
	m_nID = nID;
	m_nParentID = Bone.nParentIndex;
	m_fTimeLength = Bone.fTimeLength;

	memcpy(m_matLocal.m, Bone.matLocal, sizeof(float)*16);
	COEMath::BuildQuaternionFromMatrix(m_qLocalRot, m_matLocal);
	COEMath::GetMatrixTranslation(m_vLocalPos, m_matLocal);
	COEMath::GetMatrixScale(m_vLocalScale, m_matLocal);

	pFile->Seek(Bone.nOffFrameRot);
	for (int i = 0; i < Bone.nNumFrameRot; ++i)
	{
		COEFmtSkeleton::FRAME_ROT KeyFrame;
		pFile->Read(&KeyFrame, sizeof(KeyFrame));

		KEYFRAME_ROT KeyFrameRot;
		KeyFrameRot.fTime = KeyFrame.fTime;
		KeyFrameRot.qRot.Init(KeyFrame.qRot[0], KeyFrame.qRot[1], KeyFrame.qRot[2], KeyFrame.qRot[3]);

		m_vKeyFrameRot.push_back(KeyFrameRot);
	}

	pFile->Seek(Bone.nOffFramePos);
	for (int i = 0; i < Bone.nNumFramePos; ++i)
	{
		COEFmtSkeleton::FRAME_POS KeyFrame;
		pFile->Read(&KeyFrame, sizeof(KeyFrame));

		KEYFRAME_POS KeyFramePos;
		KeyFramePos.fTime = KeyFrame.fTime;
		KeyFramePos.vPos.Init(KeyFrame.vPos[0], KeyFrame.vPos[1], KeyFrame.vPos[2]);

		m_vKeyFramePos.push_back(KeyFramePos);
	}

	pFile->Seek(Bone.nOffFrameScale);
	for (int i = 0; i < Bone.nNumFrameScale; ++i)
	{
		COEFmtSkeleton::FRAME_SCALE KeyFrame;
		pFile->Read(&KeyFrame, sizeof(KeyFrame));

		KEYFRAME_SCALE KeyFrameScale;
		KeyFrameScale.fTime = KeyFrame.fTime;
		KeyFrameScale.vScale.Init(KeyFrame.vScale[0], KeyFrame.vScale[1], KeyFrame.vScale[2]);

		m_vKeyFrameScale.push_back(KeyFrameScale);
	}

	return true;
}

bool COEBone_Impl::SlerpRot(CQuaternion& qRotOut, float fTime)
{
	int nFrameCount = (int)m_vKeyFrameRot.size();
	if (nFrameCount <= 0) return false;

	int nPrevIndex = 0;
	int nNextIndex = 0;
	for (int i = 0; i < nFrameCount; ++i)
	{
		if (m_vKeyFrameRot[i].fTime > fTime)
		{
			nPrevIndex = i-1;
			nNextIndex = i;
			break;
		}
	}

	if (nPrevIndex < 0)
	{
		qRotOut = m_vKeyFrameRot[0].qRot;
		return true;
	}

	if (nPrevIndex == nNextIndex)
	{
		qRotOut = m_vKeyFrameRot[nFrameCount-1].qRot;
		return true;
	}

	float fDetail = (fTime - m_vKeyFrameRot[nPrevIndex].fTime) / (m_vKeyFrameRot[nNextIndex].fTime - m_vKeyFrameRot[nPrevIndex].fTime);
	COEMath::QuaternionSlerp(qRotOut, m_vKeyFrameRot[nPrevIndex].qRot, m_vKeyFrameRot[nNextIndex].qRot, fDetail);

	return true;
}

bool COEBone_Impl::LerpPos(CVector3& vPosOut, float fTime)
{
	int nFrameCount = (int)m_vKeyFramePos.size();
	if (nFrameCount <= 0) return false;

	int nPrevIndex = 0;
	int nNextIndex = 0;
	for (int i = 0; i < nFrameCount; ++i)
	{
		if (m_vKeyFramePos[i].fTime > fTime)
		{
			nPrevIndex = i-1;
			nNextIndex = i;
			break;
		}
	}

	if (nPrevIndex < 0)
	{
		vPosOut = m_vKeyFramePos[0].vPos;
		return true;
	}

	if (nPrevIndex == nNextIndex)
	{
		vPosOut = m_vKeyFramePos[nFrameCount-1].vPos;
		return true;
	}

	float fDetail = (fTime - m_vKeyFramePos[nPrevIndex].fTime) / (m_vKeyFramePos[nNextIndex].fTime - m_vKeyFramePos[nPrevIndex].fTime);
	COEMath::VectorLerp(vPosOut, m_vKeyFramePos[nPrevIndex].vPos, m_vKeyFramePos[nNextIndex].vPos, fDetail);

	return true;
}

bool COEBone_Impl::LerpScale(CVector3& vScaleOut, float fTime)
{
	int nFrameCount = (int)m_vKeyFrameScale.size();
	if (nFrameCount <= 0) return false;

	int nPrevIndex = 0;
	int nNextIndex = 0;
	for (int i = 0; i < nFrameCount; ++i)
	{
		if (m_vKeyFrameScale[i].fTime > fTime)
		{
			nPrevIndex = i-1;
			nNextIndex = i;
			break;
		}
	}

	if (nPrevIndex < 0)
	{
		vScaleOut = m_vKeyFrameScale[0].vScale;
		return true;
	}

	if (nPrevIndex == nNextIndex)
	{
		vScaleOut = m_vKeyFrameScale[nFrameCount-1].vScale;
		return true;
	}

	float fDetail = (fTime - m_vKeyFrameScale[nPrevIndex].fTime) / (m_vKeyFrameScale[nNextIndex].fTime - m_vKeyFrameScale[nPrevIndex].fTime);
	COEMath::VectorLerp(vScaleOut, m_vKeyFrameScale[nPrevIndex].vScale, m_vKeyFrameScale[nNextIndex].vScale, fDetail);

	return true;
}
