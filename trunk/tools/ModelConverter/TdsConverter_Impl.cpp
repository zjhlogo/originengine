/*!
 * \file TdsConverter_Impl.cpp
 * \date 10-26-2010 21:25:31
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "TdsConverter_Impl.h"
#include <libOEBase/OEOS.h>
#include <OEBase/IOEFileMgr.h>
#include <OEBase/IOEXmlMgr.h>

CTdsConverter_Impl::CTdsConverter_Impl()
{
	Init();
}

CTdsConverter_Impl::~CTdsConverter_Impl()
{
	Destroy();
}

void CTdsConverter_Impl::Init()
{
	m_pMaterials = NULL;
	m_pMeshes = NULL;
	m_pVertices = NULL;
	m_nVertDecl = COEFmtMesh::VDM_POSITION | COEFmtMesh::VDM_TEXCOORD0 | COEFmtMesh::VDM_NORMAL;
}

void CTdsConverter_Impl::Destroy()
{
	ClearData();
}

bool CTdsConverter_Impl::CanConvert(const tstring& strFile)
{
	tstring strExt;
	COEOS::GetFileExt(strExt, strFile);

	COEOS::tolower(strExt, strExt);
	if (strExt == TS("tds")) return true;

	return false;
}

bool CTdsConverter_Impl::DoConvert(const tstring& strFileIn, const tstring& strFileOut)
{
	if (!LoadFromFile(strFileIn)) return false;

	tstring strMeshFile;
	COEOS::GetFileNameWithoutExt(strMeshFile, strFileOut);
	strMeshFile += TS(".mesh");
	if (!SaveToMeshFile(strMeshFile)) return false;

	tstring strXmlFile;
	COEOS::GetFileNameWithoutExt(strXmlFile, strFileOut);
	strXmlFile += TS(".xml");
	if (!SaveToXmlFile(strXmlFile, strMeshFile)) return false;

	return true;
}

bool CTdsConverter_Impl::LoadFromFile(const tstring& strFile)
{
	ClearData();

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile);
	if (!pFile) return false;

	pFile->Read(&m_TdsHeader, sizeof(m_TdsHeader));

	if (m_TdsHeader.dwID != TSG_SIGNATURE)
	{
		SAFE_RELEASE(pFile);
		return false;
	}

	// read the materials
	if (m_TdsHeader.wMaterialCount)
	{
		m_pMaterials = new TSGMaterial[m_TdsHeader.wMaterialCount];
		int nSize = pFile->Read(&m_pMaterials[0], sizeof(TSGMaterial)*m_TdsHeader.wMaterialCount);
		if (nSize != sizeof(TSGMaterial)*m_TdsHeader.wMaterialCount)
		{
			SAFE_RELEASE(pFile);
			return false;
		}
	}

	// read the meshes
	if (m_TdsHeader.wMeshCount)
	{
		m_pMeshes = new TSGMesh[m_TdsHeader.wMeshCount];
		memset(m_pMeshes, 0, sizeof(TSGMesh) * m_TdsHeader.wMeshCount);
		for (int i = 0; i < m_TdsHeader.wMeshCount; i++)
		{
			int nSize = pFile->Read(&m_pMeshes[i], sizeof(TSGMesh) - sizeof(unsigned short *));
			if (nSize != sizeof(TSGMesh) - sizeof(unsigned short *))
			{
				SAFE_RELEASE(pFile);
				return false;
			}
		}

		// read indices
		for (int j = 0; j < m_TdsHeader.wMeshCount; j++)
		{
			m_pMeshes[j].pIndices = new unsigned short[m_pMeshes[j].dwFaceCount * 3];
			int nSize = pFile->Read(m_pMeshes[j].pIndices, sizeof(unsigned short)*m_pMeshes[j].dwFaceCount * 3);
			if (nSize != sizeof(unsigned short)*m_pMeshes[j].dwFaceCount * 3)
			{
				SAFE_RELEASE(pFile);
				return false;
			}
		}
	}

	// read vertices
	if (m_TdsHeader.wVertexCount)
	{
		m_pVertices = new TSGVertex[m_TdsHeader.wVertexCount];
		int nSize = pFile->Read(m_pVertices, sizeof(TSGVertex)*m_TdsHeader.wVertexCount);
		if (nSize != sizeof(TSGVertex)*m_TdsHeader.wVertexCount)
		{
			SAFE_RELEASE(pFile);
			return false;
		}
	}

	SAFE_RELEASE(pFile);

	for (int i = 0; i < m_TdsHeader.wVertexCount; ++i)
	{
		if (m_pVertices[i].vPosition[0] < m_vBoundingBoxMin.x) m_vBoundingBoxMin.x = m_pVertices[i].vPosition[0];
		if (m_pVertices[i].vPosition[0] > m_vBoundingBoxMax.x) m_vBoundingBoxMax.x = m_pVertices[i].vPosition[0];

		if (m_pVertices[i].vPosition[2] < m_vBoundingBoxMin.y) m_vBoundingBoxMin.y = m_pVertices[i].vPosition[2];
		if (m_pVertices[i].vPosition[2] > m_vBoundingBoxMax.y) m_vBoundingBoxMax.y = m_pVertices[i].vPosition[2];

		if (-m_pVertices[i].vPosition[1] < m_vBoundingBoxMin.z) m_vBoundingBoxMin.z = -m_pVertices[i].vPosition[1];
		if (-m_pVertices[i].vPosition[1] > m_vBoundingBoxMax.z) m_vBoundingBoxMax.z = -m_pVertices[i].vPosition[1];
	}

	return true;
}

void CTdsConverter_Impl::ClearData()
{
	SAFE_DELETE_ARRAY(m_pMaterials);
	SAFE_DELETE_ARRAY(m_pVertices);

	if (m_pMeshes)
	{
		for (int i = 0; i < m_TdsHeader.wMeshCount; i++)
		{
			SAFE_DELETE_ARRAY(m_pMeshes[i].pIndices);
		}
	}

	SAFE_DELETE_ARRAY(m_pMeshes);
	memset(&m_TdsHeader, 0, sizeof(m_TdsHeader));

	m_vBoundingBoxMin.Init(COEMath::FLOAT_MAX, COEMath::FLOAT_MAX, COEMath::FLOAT_MAX);
	m_vBoundingBoxMax.Init(COEMath::FLOAT_MIN, COEMath::FLOAT_MIN, COEMath::FLOAT_MIN);
}

bool CTdsConverter_Impl::SaveToMeshFile(const tstring& strFile)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	// write header
	COEFmtMesh::FILE_HEADER Header;
	Header.nMagicNumber = COEFmtMesh::MAGIC_NUMBER;
	Header.nVersion = COEFmtMesh::CURRENT_VERSION;
	Header.fBoundingBoxMin[0] = m_vBoundingBoxMin.x;
	Header.fBoundingBoxMin[1] = m_vBoundingBoxMin.y;
	Header.fBoundingBoxMin[2] = m_vBoundingBoxMin.z;
	Header.fBoundingBoxMax[0] = m_vBoundingBoxMax.x;
	Header.fBoundingBoxMax[1] = m_vBoundingBoxMax.y;
	Header.fBoundingBoxMax[2] = m_vBoundingBoxMax.z;
	Header.nNumPieces = m_TdsHeader.wMeshCount;
	pFile->Write(&Header, sizeof(Header));

	// make room for piece list
	uint nPieceListPos = pFile->Tell();
	std::vector<COEFmtMesh::PIECE> vPiece;
	if (Header.nNumPieces > 0)
	{
		vPiece.resize(Header.nNumPieces);
		memset(&vPiece[0], 0, sizeof(COEFmtMesh::PIECE)*Header.nNumPieces);
		pFile->Write(&vPiece[0], sizeof(COEFmtMesh::PIECE)*Header.nNumPieces);
	}

	// write mesh
	for (int i = 0; i < Header.nNumPieces; ++i)
	{
		strncpy_s(vPiece[i].szName, COEFmtMesh::PIECE_NAME_SIZE, m_pMeshes[i].pName, _TRUNCATE);
		vPiece[i].nPieceMask = COEFmtMesh::PM_VISIBLE;
		vPiece[i].nVertexDataMask = m_nVertDecl;
		vPiece[i].nMaterialID = m_pMeshes[i].wMaterialID;

		TV_USHORT vIndis;
		ushort nVertexCount = RebuildIndis(vIndis, m_pMeshes[i]);
		// calculate vertex data offset
		vPiece[i].nNumVerts = nVertexCount;
		vPiece[i].nOffVerts = pFile->Tell();

		// write vertex data
		for (int j = 0; j < m_TdsHeader.wVertexCount; ++j)
		{
			if (vIndis[j] == m_TdsHeader.wVertexCount) continue;

			TSGVertex& Vertex = m_pVertices[j];

			FILE_VERTEX FileVert;
			memset(&FileVert, 0, sizeof(FileVert));

			// position
			FileVert.x = Vertex.vPosition[0];
			FileVert.y = Vertex.vPosition[2];
			FileVert.z = -Vertex.vPosition[1];

			// uv
			FileVert.u = Vertex.vTexCoord[0];
			FileVert.v = Vertex.vTexCoord[1];

			// normal
			FileVert.nx = Vertex.vNormal[0];
			FileVert.ny = Vertex.vNormal[2];
			FileVert.nz = -Vertex.vNormal[1];

			pFile->Write(&FileVert, sizeof(FileVert));
		}

		// calculate index data offset
		vPiece[i].nNumIndis = (int)m_pMeshes[i].dwFaceCount*3;
		vPiece[i].nOffIndis = pFile->Tell();

		// write index data
		int nNumFaces = (int)m_pMeshes[i].dwFaceCount;
		for (int j = 0; j < nNumFaces; ++j)
		{
			INDEX Index;
			Index.v1 = vIndis[m_pMeshes[i].pIndices[j*3]];
			Index.v2 = vIndis[m_pMeshes[i].pIndices[j*3+1]];
			Index.v3 = vIndis[m_pMeshes[i].pIndices[j*3+2]];

			pFile->Write(&Index, sizeof(Index));
		}
	}

	// write true piece list
	pFile->Seek(nPieceListPos);
	if (Header.nNumPieces > 0)
	{
		pFile->Write(&vPiece[0], sizeof(COEFmtMesh::PIECE)*Header.nNumPieces);
	}

	SAFE_RELEASE(pFile);
	return true;
}

bool CTdsConverter_Impl::SaveToXmlFile(const tstring& strFile, const tstring& strMeshFile)
{
	IOEXmlDocument* pXmlDocument = g_pOEXmlMgr->CreateDocument();
	if (!pXmlDocument) return false;

	IOEXmlNode* pXmlModel = pXmlDocument->InsertRootNode(TS("Model"));
	IOEXmlNode* pXmlRenderData = pXmlModel->InsertChild(TS("RenderData"));

	IOEXmlNode* pXmlMesh = pXmlRenderData->InsertChild(TS("Mesh"));
	tstring strName;
	COEOS::GetFileNameWithExt(strName, strMeshFile);
	pXmlMesh->SetText(strName);

	IOEXmlNode* pXmlMaterials = pXmlRenderData->InsertChild(TS("Materials"));
	pXmlMaterials->SetAttribute(TS("count"), m_TdsHeader.wMaterialCount);
	for (int i = 0; i < m_TdsHeader.wMaterialCount; ++i)
	{
		TSGMaterial& Material = m_pMaterials[i];
		IOEXmlNode* pXmlMaterial = pXmlMaterials->InsertChild(TS("Material"));
		pXmlMaterial->SetAttribute(TS("id"), i);
		pXmlMaterial->SetAttribute(TS("vertdecl"), (int)m_nVertDecl);
		pXmlMaterial->SetAttribute(TS("shader"), EMPTY_STRING);

		tstring strTextureMap;
		COEOS::char2tchar(strTextureMap, Material.pTextureMap);
		if (!strTextureMap.empty()) pXmlMaterial->SetAttribute(TS("texture0"), strTextureMap);

		tstring strReflectionMap;
		COEOS::char2tchar(strReflectionMap, Material.pReflectionMap);
		if (!strReflectionMap.empty()) pXmlMaterial->SetAttribute(TS("texture1"), strReflectionMap);
	}

	pXmlDocument->SaveFile(strFile);
	SAFE_RELEASE(pXmlDocument);

	return true;
}

ushort CTdsConverter_Impl::RebuildIndis(TV_USHORT& IndisOut, const TSGMesh& Mesh)
{
	IndisOut.resize(m_TdsHeader.wVertexCount, m_TdsHeader.wVertexCount);

	for (ushort i = 0; i < Mesh.dwFaceCount; ++i)
	{
		IndisOut[Mesh.pIndices[i*3]] = 0;
		IndisOut[Mesh.pIndices[i*3+1]] = 0;
		IndisOut[Mesh.pIndices[i*3+2]] = 0;
	}

	int nIndis = 0;
	for (ushort i = 0; i < m_TdsHeader.wVertexCount; ++i)
	{
		if (IndisOut[i] == 0) IndisOut[i] = nIndis++;
	}

	return nIndis;
}
