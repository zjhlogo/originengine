/*!
 * \file OEMesh_Impl.cpp
 * \date 30-7-2009 16:50:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMesh_Impl.h"

COEMesh_Impl::COEMesh_Impl(const tstring& strFile)
{
	Init();
	m_bOK = Create(strFile);
}

COEMesh_Impl::~COEMesh_Impl()
{
	Destroy();
}

void COEMesh_Impl::Init()
{
	// TODO: 
}

void COEMesh_Impl::Destroy()
{
	g_pOEResMgr->DestroyPieces(m_vPiece);
}

int COEMesh_Impl::GetNumPieces() const
{
	return (int)m_vPiece.size();
}

IOEPiece* COEMesh_Impl::GetPiece(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_vPiece.size()) return NULL;
	return m_vPiece[nIndex];
}

IOEPiece* COEMesh_Impl::FindPiece(const tstring& strName) const
{
	for (TV_PIECE::const_iterator it = m_vPiece.begin(); it != m_vPiece.end(); ++it)
	{
		if ((*it)->GetName() == strName) return (*it);
	}

	return NULL;
}

bool COEMesh_Impl::Create(const tstring& strFile)
{
	bool bOK = g_pOEResMgr->CreatePieces(m_vPiece, strFile);
	if (!bOK) return false;

	return true;
}
