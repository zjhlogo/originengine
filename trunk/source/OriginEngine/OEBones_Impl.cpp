/*!
 * \file OEBones_Impl.cpp
 * \date 1-3-2010 14:54:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEBones_Impl.h"
#include "OEBone_Impl.h"

#include <IOEFileMgr.h>
#include <OEFmtBone.h>

COEBones_Impl::COEBones_Impl(const tstring& strFile)
{
	Init();
	m_bOK = CreateBones(strFile);
}

COEBones_Impl::~COEBones_Impl()
{
	Destroy();
}

void COEBones_Impl::Init()
{
	// TODO: 
}

void COEBones_Impl::Destroy()
{
	DestroyBones();
}

int COEBones_Impl::GetBonesCount()
{
	return (int)m_vBones.size();
}

IOEBone* COEBones_Impl::GetBone(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)m_vBones.size()) return NULL;
	return m_vBones[nIndex];
}

bool COEBones_Impl::CreateBones(const tstring& strFile)
{
	DestroyBones();

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile);
	if (!pFile) return false;

	COEFmtBone::FILE_HEADER Header;
	pFile->Read(&Header, sizeof(Header));

	if (Header.nMagicNumber != COEFmtBone::MAGIC_NUMBER
		|| Header.nVersion != COEFmtBone::CURRENT_VERSION)
	{
		SAFE_RELEASE(pFile);
		return false;
	}

	// read bone info
	std::vector<COEFmtBone::BONE> vBones;
	if (Header.nNumBones > 0)
	{
		vBones.resize(Header.nNumBones);
		pFile->Read(&vBones[0], sizeof(COEFmtBone::BONE)*Header.nNumBones);
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
		if (nParentID != COEFmtBone::INVALID_BONE_ID)
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

void COEBones_Impl::DestroyBones()
{
	for (TV_BONE::iterator it = m_vBones.begin(); it != m_vBones.end(); ++it)
	{
		IOEBone* pBone = (*it);
		SAFE_DELETE(pBone);
	}
	m_vBones.clear();
}
