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
	m_nID = INVALID_BONE_ID;
	m_nParentID = INVALID_BONE_ID;
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
