/*!
 * \file AnimationTracker.cpp
 * \date 14-8-2009 20:20:17
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "AnimationTracker.h"

CAnimationTracker::CAnimationTracker(IOEMeshBone* pBone, CMatrix4x4* pmatTransformed, CAnimationTracker* pParentTracker /* = NULL */)
{
	Init();
	m_bOK = Create(pBone, pmatTransformed, pParentTracker);
}

CAnimationTracker::~CAnimationTracker()
{
	Destroy();
}

void CAnimationTracker::Init()
{
	m_pBone = NULL;
	m_pmatTransformed = NULL;
	m_pParentTracker = NULL;
}

void CAnimationTracker::Destroy()
{
	// TODO: 
}

void CAnimationTracker::Update(float fDetailTime)
{
	// update self
	if (m_pParentTracker)
	{
		CMatrix4x4 matLocal;
		m_pBone->SlerpMatrix(matLocal, fDetailTime, true);
		(*m_pmatTransformed) = matLocal * m_pParentTracker->GetTransformedMatrix();
	}
	else
	{
		m_pBone->SlerpMatrix(*m_pmatTransformed, fDetailTime, true);
	}

	// update child
	for (VANIM_TRACKER::iterator it = m_vChildTracker.begin(); it != m_vChildTracker.end(); ++it)
	{
		(*it)->Update(fDetailTime);
	}
}

const CMatrix4x4& CAnimationTracker::GetTransformedMatrix() const
{
	return (*m_pmatTransformed);
}

bool CAnimationTracker::Create(IOEMeshBone* pBone, CMatrix4x4* pmatTransformed, CAnimationTracker* pParentTracker /* = NULL */)
{
	if (!pBone) return false;

	m_pBone = pBone;
	m_pmatTransformed = pmatTransformed;
	m_pParentTracker = pParentTracker;
	if (m_pParentTracker) m_pParentTracker->AddChild(this);

	return true;
}

void CAnimationTracker::AddChild(CAnimationTracker* pChild)
{
	m_vChildTracker.push_back(this);
}
