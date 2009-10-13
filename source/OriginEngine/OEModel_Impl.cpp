/*!
 * \file OEModel_Impl.cpp
 * \date 9-8-2009 17:52:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModel_Impl.h"
#include <IOEMeshMgr.h>

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
	m_pRootTracker = NULL;
}

void COEModel_Impl::Destroy()
{
	SAFE_RELEASE(m_pMesh);
	for (TRACKER_MAP::iterator it = m_vTrackerMap.begin(); it != m_vTrackerMap.end(); ++it)
	{
		CAnimationTracker* pTracker = it->second;
		SAFE_DELETE(pTracker);
	}
	m_vTrackerMap.clear();
}

void COEModel_Impl::Update(float fDetailTime)
{
	m_pRootTracker->Update(fDetailTime);
}

void COEModel_Impl::Render(float fDetailTime)
{
	// TODO: 
}

bool COEModel_Impl::Create(const tstring& strFileName)
{
	m_pMesh = g_pOEMeshMgr->CreateMeshFromFile(strFileName);
	if (!m_pMesh) return false;

	IOEMeshBone* pRootBone = m_pMesh->GetRootBone();
	if (!pRootBone) return true;

	int nTotalCount = CalculateBoneCount(pRootBone);
	m_vmatTransformed.resize(nTotalCount);

	m_pRootTracker = LoopCreateTracker(pRootBone, NULL);

	return true;
}

int COEModel_Impl::CalculateBoneCount(IOEMeshBone* pBone)
{
	int nTotalBoneCount = 1;

	int nNumChildren = pBone->GetNumChildren();
	for (int i = 0; i < nNumChildren; ++i)
	{
		IOEMeshBone* pChildBone = pBone->GetChild(i);
		nTotalBoneCount += CalculateBoneCount(pChildBone);
	}

	return nTotalBoneCount;
}

CAnimationTracker* COEModel_Impl::LoopCreateTracker(IOEMeshBone* pBone, CAnimationTracker* pParentTracker /*= NULL*/)
{
	CAnimationTracker* pTracker = CreateTracker(pBone, pParentTracker);
	if (!pTracker) return NULL;

	int nNumChildren = pBone->GetNumChildren();
	for (int i = 0; i < nNumChildren; ++i)
	{
		IOEMeshBone* pChildBone = pBone->GetChild(i);
		CAnimationTracker* pChildTracker = LoopCreateTracker(pChildBone, pTracker);
		if (!pChildTracker) return NULL;
	}

	return pTracker;
}

CAnimationTracker* COEModel_Impl::CreateTracker(IOEMeshBone* pBone, CAnimationTracker* pParentTracker /*= NULL*/)
{
	int nIndex = pBone->GetID();

	CAnimationTracker* pTracker = new CAnimationTracker(pBone, &m_vmatTransformed[nIndex], pParentTracker);
	if (!pTracker || !pTracker->IsOK())
	{
		SAFE_DELETE(pTracker);
		return NULL;
	}

	m_vTrackerMap.insert(std::make_pair(pBone->GetID(), pTracker));
	return pTracker;
}
