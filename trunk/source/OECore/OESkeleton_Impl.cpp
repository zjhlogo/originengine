/*!
 * \file OESkeleton_Impl.cpp
 * \date 1-3-2010 19:20:56
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkeleton_Impl.h"
#include "OEBone_Impl.h"

#include <OEBase/IOEFileMgr.h>
#include <OECore/OEFmtSkeleton.h>

COESkeleton_Impl::COESkeleton_Impl(const tstring& strFile)
:IOESkeleton(TS("COESkeleton_Impl"))
{
	Init();
	m_bOK = CreateBones(strFile);
}

COESkeleton_Impl::~COESkeleton_Impl()
{
	Destroy();
}

void COESkeleton_Impl::Init()
{
	// TODO: 
}

void COESkeleton_Impl::Destroy()
{
	DestroyBones();
}

int COESkeleton_Impl::GetBonesCount()
{
	return (int)m_vBones.size();
}

IOEBone* COESkeleton_Impl::GetBone(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)m_vBones.size()) return NULL;
	return m_vBones[nIndex];
}

bool COESkeleton_Impl::CreateBones(const tstring& strFile)
{
	DestroyBones();

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile);
	if (!pFile) return false;

	COEFmtSkeleton::FILE_HEADER Header;
	pFile->Read(&Header, sizeof(Header));

	if (Header.nMagicNumber != COEFmtSkeleton::MAGIC_NUMBER
		|| Header.nVersion != COEFmtSkeleton::CURRENT_VERSION)
	{
		SAFE_RELEASE(pFile);
		return false;
	}

	// read bone info
	std::vector<COEFmtSkeleton::BONE> vBones;
	if (Header.nNumBones > 0)
	{
		vBones.resize(Header.nNumBones);
		pFile->Read(&vBones[0], sizeof(COEFmtSkeleton::BONE)*Header.nNumBones);
	}

	// create bones
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		COEBone_Impl* pBone = new COEBone_Impl(vBones[i], i, pFile);
		if (!pBone || !pBone->IsOK())
		{
			SAFE_RELEASE(pFile);
			DestroyBones();
			return false;
		}
		m_vBones.push_back(pBone);
	}

	SAFE_RELEASE(pFile);

	// build bone matrix
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		int nParentID = m_vBones[i]->GetParentID();
		if (nParentID != COEFmtSkeleton::INVALID_BONE_ID)
		{
			COEBone_Impl* pCurrBone = (COEBone_Impl*)m_vBones[i];
			COEBone_Impl* pParentBone = (COEBone_Impl*)m_vBones[nParentID];

			pCurrBone->SetWorldMatrix(pCurrBone->GetLocalMatrix() * pParentBone->GetWorldMatrix());
		}
		else
		{
			COEBone_Impl* pCurrBone = (COEBone_Impl*)m_vBones[i];
			pCurrBone->SetWorldMatrix(pCurrBone->GetLocalMatrix());
		}
	}

	return true;
}

void COESkeleton_Impl::DestroyBones()
{
	for (TV_BONE::iterator it = m_vBones.begin(); it != m_vBones.end(); ++it)
	{
		IOEBone* pBone = (*it);
		SAFE_DELETE(pBone);
	}
	m_vBones.clear();
}
