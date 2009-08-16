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
			matOut = m_vFrame[nFrameCount-1].matTransform;
			return true;
		}

		fTime -= floorf(fTime/m_fTimeLength)*m_fTimeLength;
	}

	if (fTime <= 0.0f)
	{
		if (!bLoop)
		{
			matOut = m_vFrame[0].matTransform;
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
		matOut = m_vFrame[nPrevIndex].matTransform;
		return true;
	}

	//float t = (fTime - m_vFrame[nPrevIndex].fTime) / (m_vFrame[nNextIndex].fTime - m_vFrame[nPrevIndex].fTime);
	//COEMath::MatrixLerp(matOut, m_vFrame[nPrevIndex].matTransform, m_vFrame[nNextIndex].matTransform, t);

	CQuaternion q1;
	CQuaternion q2;
	COEMath::BuildQuaternionFromMatrix(q1, m_vFrame[nPrevIndex].matTransform);
	COEMath::BuildQuaternionFromMatrix(q2, m_vFrame[nNextIndex].matTransform);

	CQuaternion q;
	float t = (fTime - m_vFrame[nPrevIndex].fTime) / (m_vFrame[nNextIndex].fTime - m_vFrame[nPrevIndex].fTime);
	COEMath::QuaternionSlerp(q, q1, q2, t);

	COEMath::BuildMatrixFromQuaternion(matOut, q);
	matOut.m[12] = (1.0f-t)*m_vFrame[nPrevIndex].matTransform.m[12] + t*m_vFrame[nNextIndex].matTransform.m[12];
	matOut.m[13] = (1.0f-t)*m_vFrame[nPrevIndex].matTransform.m[13] + t*m_vFrame[nNextIndex].matTransform.m[13];
	matOut.m[14] = (1.0f-t)*m_vFrame[nPrevIndex].matTransform.m[14] + t*m_vFrame[nNextIndex].matTransform.m[14];
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
	memcpy(m_matLocal.m, Bone.matLocal, sizeof(m_matLocal.m));

	if (Bone.nNumBoneFrame)
	{
		m_vFrame.resize(Bone.nNumBoneFrame);
		pFile->Seek(Bone.nOffBoneFrame);
		pFile->Read(&m_vFrame[0], sizeof(COEFmtMesh::BONE_FRAME)*Bone.nNumBoneFrame);
	}

	return true;
}
