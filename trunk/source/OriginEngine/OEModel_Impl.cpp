/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"
#include <IOEMeshMgr.h>
#include <OEFmtMesh.h>

COEModel_Impl::COEModel_Impl(const tstring& strFileName)
{
	Init();
	m_bOK = Create(strFileName);
}

COEModel_Impl::~COEModel_Impl()
{
	Destroy();
}

void COEModel_Impl::Init()
{
	m_pMesh = NULL;
	m_fTotalTime = 0.0f;
}

void COEModel_Impl::Destroy()
{
	// TODO: 
}

void COEModel_Impl::Update(float fDetailTime)
{
	m_fTotalTime += fDetailTime;

	CMatrix4x4 matAnim;
	int nNumBones = m_pMesh->GetNumBones();
	for (int i = 0; i < nNumBones; ++i)
	{
		IOEMeshBone* pMeshBone = m_pMesh->GetBone(i);

		pMeshBone->SlerpMatrix(matAnim, m_fTotalTime);
		const CMatrix4x4& matLocal = pMeshBone->GetLocalMatrix();

		m_vmatSkin[i] = matAnim * matLocal;

		int nParentID = pMeshBone->GetParentID();
		if (nParentID != COEFmtMesh::INVALID_BONE_ID)
		{
			m_vmatSkin[i] *= m_vmatSkin[nParentID];
		}
	}

	for (int i = 0; i < nNumBones; ++i)
	{
		IOEMeshBone* pMeshBone = m_pMesh->GetBone(i);
		m_vmatSkin[i] = pMeshBone->GetWorldMatrixInv() * m_vmatSkin[i];
	}
}

void COEModel_Impl::Render(float fDetailTime)
{
	// TODO: 
}

IOEMesh* COEModel_Impl::GetMesh()
{
	return m_pMesh;
}

int COEModel_Impl::GetNumMatrixPalette()
{
	return m_vmatSkin.size();
}

CMatrix4x4* COEModel_Impl::GetMatrixPalette()
{
	if (m_vmatSkin.empty()) return NULL;
	return &m_vmatSkin[0];
}

bool COEModel_Impl::Create(const tstring& strFileName)
{
	m_pMesh = g_pOEMeshMgr->CreateMeshFromFile(strFileName);
	if (!m_pMesh) return false;

	IOEMeshBone* pRootBone = m_pMesh->GetRootBone();
	if (!pRootBone) return true;

	int nNumBones = m_pMesh->GetNumBones();
	m_vmatSkin.resize(nNumBones);

	return true;
}
