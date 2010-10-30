/*!
 * \file OEAnimData_Impl.cpp
 * \date 10-29-2010 18:50:39
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEAnimData_Impl.h"
#include <OECore/IOEResMgr.h>

COEAnimData_Impl::COEAnimData_Impl(const tstring& strFile)
{
	Init();
	m_bOK = Create(strFile);
}

COEAnimData_Impl::~COEAnimData_Impl()
{
	Destroy();
}

void COEAnimData_Impl::Init()
{
	m_pSkeleton = NULL;
	m_fAnimLength = 0.0f;
	m_fCurrTime = 0.0f;
	m_pSkinMatrixs = NULL;
	m_nNumSkinMatrixs = NULL;
}

void COEAnimData_Impl::Destroy()
{
	SAFE_RELEASE(m_pSkeleton);
	SAFE_DELETE_ARRAY(m_pSkinMatrixs);
}

IOESkeleton* COEAnimData_Impl::GetSkeleton()
{
	return m_pSkeleton;
}

float COEAnimData_Impl::GetAnimLength()
{
	return m_fAnimLength;
}

void COEAnimData_Impl::SetCurrTime(float fCurrTime)
{
	m_fCurrTime = fCurrTime;
}

float COEAnimData_Impl::GetCurrTime()
{
	return m_fCurrTime;
}

CMatrix4x4* COEAnimData_Impl::GetSkinMatrixs()
{
	return m_pSkinMatrixs;
}

int COEAnimData_Impl::GetNumSkinMatrixs()
{
	return m_nNumSkinMatrixs;
}

bool COEAnimData_Impl::Create(const tstring& strFile)
{
	m_pSkeleton = g_pOEResMgr->CreateSkeleton(strFile);
	if (!m_pSkeleton) return false;

	int nBonesCount = m_pSkeleton->GetBonesCount();
	for (int i = 0; i < nBonesCount; ++i)
	{
		IOEBone* pBone = m_pSkeleton->GetBone(i);
		if (m_fAnimLength < pBone->GetTimeLength()) m_fAnimLength = pBone->GetTimeLength();
	}

	m_pSkinMatrixs = new CMatrix4x4[nBonesCount];
	m_nNumSkinMatrixs = nBonesCount;

	return true;
}
