/*!
 * \file OEPiece_Impl.cpp
 * \date 10-23-2009 14:01:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEPiece_Impl.h"

#include <OECore/IOEDevice.h>
#include <libOEBase/OEOS.h>

#include <vector>

COEPiece_Impl::COEPiece_Impl(const COEFmtMesh::PIECE& Piece, IOEFile* pFile)
:IOEPiece(TS("COEPiece_Impl"))
{
	Init();
	m_bOK = Create(Piece, pFile);
}

COEPiece_Impl::~COEPiece_Impl()
{
	Destroy();
}

void COEPiece_Impl::Init()
{
	m_nMaterialID = 0;
	m_nVertDecl = 0;

	m_nVerts = 0;
	m_nIndis = 0;
	m_pVerts = NULL;
	m_pIndis = NULL;
}

void COEPiece_Impl::Destroy()
{
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
}

const tstring& COEPiece_Impl::GetName() const
{
	return m_strName;
}

int COEPiece_Impl::GetMaterialID() const
{
	return m_nMaterialID;
}

int COEPiece_Impl::GetVertDeclMask() const
{
	return m_nVertDecl;
}

int COEPiece_Impl::GetNumVerts() const
{
	return m_nVerts;
}

int COEPiece_Impl::GetNumIndis() const
{
	return m_nIndis;
}

void* COEPiece_Impl::GetVerts() const
{
	return m_pVerts;
}

ushort* COEPiece_Impl::GetIndis() const
{
	return m_pIndis;
}

bool COEPiece_Impl::Create(const COEFmtMesh::PIECE& Piece, IOEFile* pFile)
{
	COEOS::char2tchar(m_strName, Piece.szName);

	m_nMaterialID = Piece.nMaterialID;
	m_nVertDecl = Piece.nVertexDataMask;

	m_nVerts = Piece.nNumVerts;
	m_pVerts = (void*)new uchar[Piece.nOffIndis - Piece.nOffVerts];
	pFile->Seek(Piece.nOffVerts);
	pFile->Read(m_pVerts, Piece.nOffIndis - Piece.nOffVerts);

	m_nIndis = Piece.nNumIndis;
	m_pIndis = new ushort[m_nIndis];
	pFile->Seek(Piece.nOffIndis);
	pFile->Read(m_pIndis, sizeof(ushort)*m_nIndis);

	return true;
}
