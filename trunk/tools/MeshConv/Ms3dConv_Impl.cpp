/*!
 * \file Ms3dConv_Impl.cpp
 * \date 10-20-2009 15:21:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "Ms3dConv_Impl.h"
#include "FmtMs3d.h"

#include <IOEFileMgr.h>
#include <OEFmtMesh.h>

#include <map>
#include <math.h>

CMs3dConv_Impl::CMs3dConv_Impl()
{
	Init();
}

CMs3dConv_Impl::~CMs3dConv_Impl()
{
	Destroy();
}

void CMs3dConv_Impl::Init()
{
	// TODO: 
}

void CMs3dConv_Impl::Destroy()
{
	// TODO: 
}

bool CMs3dConv_Impl::DoConvert(const tstring& strFileIn, const tstring& strFileOut)
{
	if (!LoadFromFile(strFileIn)) return false;
	if (!SaveToFile(strFileOut)) return false;
	return true;
}

bool CMs3dConv_Impl::CanConvert(const tstring& strFileExt)
{
	if (strFileExt == t("ms3d")) return true;
	return false;
}

bool CMs3dConv_Impl::LoadFromFile(const tstring& strFile)
{
	m_vVertices.clear();
	m_vIndices.clear();

	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile);
	if (!pFile) return false;

	ms3d_header_t header;
	pFile->Read(&header, sizeof(header));

	if (strncmp(header.id, "MS3D000000", 10) != 0
		|| header.version != 4)
	{
		SAFE_RELEASE(pFile);
		return false;
	}

	// number of vertices
	word nNumVertices = 0;
	pFile->Read(&nNumVertices, sizeof(nNumVertices));

	// vertices
	std::vector<ms3d_vertex_t> arrVertices;
	arrVertices.resize(nNumVertices);
	pFile->Read(&arrVertices[0], sizeof(ms3d_vertex_t)*nNumVertices);

	for (word i = 0; i < nNumVertices; ++i)
	{
		VERTEX NewVert;
		NewVert.x = arrVertices[i].vertex[0];
		NewVert.y = arrVertices[i].vertex[1];
		NewVert.z = arrVertices[i].vertex[2];
		NewVert.u = 0.0f;
		NewVert.v = 0.0f;
		NewVert.nx = 0.0f;
		NewVert.ny = 0.0f;
		NewVert.nz = 0.0f;
		//NewVert.nBoneID = arrVertices[i].boneId;

		m_vVertices.push_back(NewVert);
	}

	// number of triangles
	word nNumTriangles = 0;
	pFile->Read(&nNumTriangles, sizeof(nNumTriangles));

	// triangles
	std::vector<ms3d_triangle_t> arrTriangles;
	arrTriangles.resize(nNumTriangles);
	pFile->Read(&arrTriangles[0], sizeof(ms3d_triangle_t)*nNumTriangles);

	SAFE_RELEASE(pFile);

	// create vertices and triangles map
	std::multimap<word, word> mapVertices;
	for (word i = 0; i < nNumTriangles; ++i)
	{
		word nVertIndex1 = arrTriangles[i].vertexIndices[0];
		word nVertIndex2 = arrTriangles[i].vertexIndices[1];
		word nVertIndex3 = arrTriangles[i].vertexIndices[2];

		mapVertices.insert(std::make_pair(nVertIndex1, i));
		mapVertices.insert(std::make_pair(nVertIndex2, i));
		mapVertices.insert(std::make_pair(nVertIndex3, i));
	}

	std::pair<std::multimap<word, word>::iterator, std::multimap<word, word>::iterator> range;

	// calculate normals
	for (word i = 0; i < nNumVertices; ++i)
	{
		range = mapVertices.equal_range(i);

		int nCount = 0;
		for (std::multimap<word, word>::iterator it = range.first; it != range.second; ++it)
		{
			word nTriaIndex = it->second;
			word nInternalIndex = 0;
			if (i == arrTriangles[nTriaIndex].vertexIndices[0])
			{
				nInternalIndex = 0;
			}
			else if (i == arrTriangles[nTriaIndex].vertexIndices[1])
			{
				nInternalIndex = 1;
			}
			else if (i == arrTriangles[nTriaIndex].vertexIndices[2])
			{
				nInternalIndex = 2;
			}

			m_vVertices[i].nx += arrTriangles[nTriaIndex].vertexNormals[nInternalIndex][0];
			m_vVertices[i].ny += arrTriangles[nTriaIndex].vertexNormals[nInternalIndex][1];
			m_vVertices[i].nz += arrTriangles[nTriaIndex].vertexNormals[nInternalIndex][2];
			++nCount;
		}

		if (nCount > 1)
		{
			m_vVertices[i].nx /= nCount;
			m_vVertices[i].ny /= nCount;
			m_vVertices[i].nz /= nCount;
		}

		// normalized
		float fDet = sqrtf(m_vVertices[i].nx*m_vVertices[i].nx+m_vVertices[i].ny*m_vVertices[i].ny+m_vVertices[i].nz*m_vVertices[i].nz);
		m_vVertices[i].nx /= fDet;
		m_vVertices[i].ny /= fDet;
		m_vVertices[i].nz /= fDet;
	}

	// calculate uv
	for (word i = 0; i < nNumVertices; ++i)
	{
		range = mapVertices.equal_range(i);
		bool bPrevUV = false;
		float fPrevU = 0.0f;
		float fPrevV = 0.0f;

		for (std::multimap<word, word>::iterator it = range.first; it != range.second; ++it)
		{
			word nTriaIndex = it->second;
			word nInternalIndex = 0;
			if (i == arrTriangles[nTriaIndex].vertexIndices[0])
			{
				nInternalIndex = 0;
			}
			else if (i == arrTriangles[nTriaIndex].vertexIndices[1])
			{
				nInternalIndex = 1;
			}
			else if (i == arrTriangles[nTriaIndex].vertexIndices[2])
			{
				nInternalIndex = 2;
			}

			if (!bPrevUV)
			{
				fPrevU = arrTriangles[nTriaIndex].s[nInternalIndex];
				fPrevV = arrTriangles[nTriaIndex].t[nInternalIndex];
				fPrevU -= floorf(fPrevU);
				fPrevV -= floorf(fPrevV);

				bPrevUV = true;
				m_vVertices[i].u = fPrevU;
				m_vVertices[i].v = fPrevV;
			}
			else
			{
				float fCurrU = arrTriangles[nTriaIndex].s[nInternalIndex];
				float fCurrV = arrTriangles[nTriaIndex].t[nInternalIndex];
				float fDetailU = fabsf(fPrevU - fCurrU);
				float fDetailV = fabsf(fPrevV - fCurrV);

				fDetailU -= floorf(fDetailU);
				fDetailU -= floorf(fDetailV);

				if ( fDetailU > 0.01f
					|| fDetailV > 0.01f)
				{
					VERTEX NewVert = m_vVertices[i];
					NewVert.u = arrTriangles[nTriaIndex].s[nInternalIndex];
					NewVert.v = arrTriangles[nTriaIndex].t[nInternalIndex];
					word nNewVertIndex = (word)m_vVertices.size();
					m_vVertices.push_back(NewVert);

					arrTriangles[nTriaIndex].vertexIndices[nInternalIndex] = nNewVertIndex;
				}
			}
		}
	}

	// store indices
	for (int i = 0; i < nNumTriangles; ++i)
	{
		m_vIndices.push_back(arrTriangles[i].vertexIndices[0]);
		m_vIndices.push_back(arrTriangles[i].vertexIndices[1]);
		m_vIndices.push_back(arrTriangles[i].vertexIndices[2]);
	}

	return true;
}

bool CMs3dConv_Impl::SaveToFile(const tstring& strFile)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	int nNumVertices = (int)m_vVertices.size();
	pFile->Write(&nNumVertices, sizeof(nNumVertices));
	pFile->Write(&m_vVertices[0], sizeof(VERTEX)*nNumVertices);

	int nNumIndices = (int)m_vIndices.size();
	pFile->Write(&nNumIndices, sizeof(nNumIndices));
	pFile->Write(&m_vIndices[0], sizeof(ushort)*nNumIndices);

	SAFE_RELEASE(pFile);
	return true;
}
