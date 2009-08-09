/*!
 * \file OEMeshPiece_Impl.cpp
 * \date 8-8-2009 21:30:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEMeshPiece_Impl.h"
#include <OEOS.h>
#include <vector>

COEMeshPiece_Impl::COEMeshPiece_Impl(const COEFmtMesh::PIECE& Piece, IOEFile* pFile)
{
	Init();
	m_bOK = Create(Piece, pFile);
}

COEMeshPiece_Impl::~COEMeshPiece_Impl()
{
	Destroy();
}

void COEMeshPiece_Impl::Init()
{
	m_pDecl = NULL;
	m_nVerts = 0;
	m_nIndis = 0;
	m_pVerts = NULL;
	m_pIndis = NULL;
}

void COEMeshPiece_Impl::Destroy()
{
	SAFE_RELEASE(m_pDecl);
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
}

const tstring& COEMeshPiece_Impl::GetName() const
{
	return m_strName;
}

bool COEMeshPiece_Impl::Create(const COEFmtMesh::PIECE& Piece, IOEFile* pFile)
{
	COEOS::char2tchar(m_strName, Piece.szName);

	if (!CreateVertDecl(Piece.nVertexDataMask)) return false;
	int nStrideSize = m_pDecl->GetStrideSize();
	if (nStrideSize <= 0) return false;

	m_nVerts = Piece.nNumVerts;
	m_pVerts = (void*)new uchar[nStrideSize*m_nVerts];
	pFile->Seek(Piece.nOffVerts);
	pFile->Read(m_pVerts, nStrideSize*m_nVerts);

	m_nIndis = Piece.nNumIndis;
	m_pIndis = new ushort[m_nIndis];
	pFile->Seek(Piece.nOffIndis);
	pFile->Read(m_pIndis, sizeof(ushort)*m_nIndis);

	return true;
}

bool COEMeshPiece_Impl::CreateVertDecl(uint nVertexDataMask)
{
	SAFE_RELEASE(m_pDecl);

	std::vector<IOEVertDecl::ELEMENT> vDecl;

	if (nVertexDataMask & COEFmtMesh::VDM_XYZ)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_FLOAT3;
		Element.eUsage = IOEVertDecl::U_POSITION;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	if (nVertexDataMask & COEFmtMesh::VDM_COLOR)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_COLOR;
		Element.eUsage = IOEVertDecl::U_COLOR;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	if (nVertexDataMask & COEFmtMesh::VDM_NXNYNZ)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_FLOAT3;
		Element.eUsage = IOEVertDecl::U_NORMAL;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	if (nVertexDataMask & COEFmtMesh::VDM_UV)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_FLOAT2;
		Element.eUsage = IOEVertDecl::U_TEXCOORD;
		Element.nIndex = 0;
		vDecl.push_back(Element);
	}

	if (nVertexDataMask & COEFmtMesh::VDM_BONE)
	{
		IOEVertDecl::ELEMENT Element;
		Element.eType = IOEVertDecl::T_FLOAT4;
		Element.eUsage = IOEVertDecl::U_TEXCOORD;
		Element.nIndex = 1;
		vDecl.push_back(Element);

		Element.eType = IOEVertDecl::T_FLOAT4;
		Element.eUsage = IOEVertDecl::U_TEXCOORD;
		Element.nIndex = 2;
		vDecl.push_back(Element);
	}

	IOEVertDecl::ELEMENT Element;
	Element.eType = IOEVertDecl::T_UNKNOWN;
	Element.eUsage = IOEVertDecl::U_UNKNOWN;
	Element.nIndex = 0;
	vDecl.push_back(Element);

	m_pDecl = g_pOEDevice->CreateVertDecl(&vDecl[0]);
	if (!m_pDecl) return false;

	return true;
}
