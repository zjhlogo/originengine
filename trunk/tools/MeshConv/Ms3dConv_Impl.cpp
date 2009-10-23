/*!
 * \file Ms3dConv_Impl.cpp
 * \date 10-20-2009 15:21:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "Ms3dConv_Impl.h"

#include <IOEFileMgr.h>
#include <OEMath/OEMath.h>
#include <OEOS.h>

#include <map>
#include <math.h>
#include <assert.h>

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
	m_vVerts.clear();
	m_vIndis.clear();
	m_vBoneInfo.clear();

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
		// 右手到左手坐标转换
		NewVert.z = -arrVertices[i].vertex[2];
		NewVert.u = 0.0f;
		NewVert.v = 0.0f;
		NewVert.nx = 0.0f;
		NewVert.ny = 0.0f;
		NewVert.nz = 0.0f;
		NewVert.nBoneID = arrVertices[i].boneId;

		m_vVerts.push_back(NewVert);
	}

	// number of triangles
	word nNumTriangles = 0;
	pFile->Read(&nNumTriangles, sizeof(nNumTriangles));

	// triangles
	std::vector<ms3d_triangle_t> arrTriangles;
	arrTriangles.resize(nNumTriangles);
	pFile->Read(&arrTriangles[0], sizeof(ms3d_triangle_t)*nNumTriangles);
	for (int i = 0; i < nNumTriangles; ++i)
	{
		// 右手到左手坐标转换
		arrTriangles[i].vertexNormals[0][2] = -arrTriangles[i].vertexNormals[0][2];
		arrTriangles[i].vertexNormals[1][2] = -arrTriangles[i].vertexNormals[1][2];
		arrTriangles[i].vertexNormals[2][2] = -arrTriangles[i].vertexNormals[2][2];
	}

	// groups
	word nNumGroups = 0;
	pFile->Read(&nNumGroups, sizeof(nNumGroups));
	for (int i = 0; i < nNumGroups; ++i)
	{
		byte nFlags = 0;
		char szName[32];
		word nNumTriangles = 0;
		byte nMaterialIndex = 0;

		pFile->Read(&nFlags, sizeof(nFlags));
		pFile->Read(szName, sizeof(szName));
		pFile->Read(&nNumTriangles, sizeof(nNumTriangles));

		for (int j = 0; j < nNumTriangles; ++j)
		{
			word nTriIndex = 0;
			pFile->Read(&nTriIndex, sizeof(nTriIndex));
		}

		pFile->Read(&nMaterialIndex, sizeof(nMaterialIndex));
	}

	// materials
	word nNumMaterials = 0;
	pFile->Read(&nNumMaterials, sizeof(nNumMaterials));
	for (int i = 0; i < nNumMaterials; ++i)
	{
		ms3d_material_t Material;
		pFile->Read(&Material, sizeof(Material));
	}

	// animations
	float fFPS = 0.0f;
	float fCurrTime = 0.0f;
	int nTotalFrames = 0;
	float fTotalTime = 0.0f;

	pFile->Read(&fFPS, sizeof(fFPS));
	pFile->Read(&fCurrTime, sizeof(fCurrTime));
	pFile->Read(&nTotalFrames, sizeof(nTotalFrames));
	fTotalTime = nTotalFrames / fFPS;

	// bones
	word nNumBones = 0;
	pFile->Read(&nNumBones, sizeof(nNumBones));
	m_vBoneInfo.resize(nNumBones);
	for (int i = 0; i < nNumBones; ++i)
	{
		byte nFlags = 0;
		char szName[32];
		char szParentName[32];
		float rRot[3];
		float vPos[3];
		word nNumKeyFramesRot = 0;
		word nNumKeyFramesTrans = 0;

		pFile->Read(&nFlags, sizeof(nFlags));
		pFile->Read(szName, sizeof(szName));
		pFile->Read(szParentName, sizeof(szParentName));
		pFile->Read(rRot, sizeof(rRot));
		pFile->Read(vPos, sizeof(vPos));

		// 右手到左手坐标转换
		rRot[0] = -rRot[0];
		rRot[1] = -rRot[1];
		vPos[2] = -vPos[2];

		pFile->Read(&nNumKeyFramesRot, sizeof(nNumKeyFramesRot));
		pFile->Read(&nNumKeyFramesTrans, sizeof(nNumKeyFramesTrans));

		COEOS::char2tchar(m_vBoneInfo[i].strName, szName);
		COEOS::char2tchar(m_vBoneInfo[i].strParentName, szParentName);
		m_vBoneInfo[i].nIndex = i;
		m_vBoneInfo[i].nParentIndex = COEFmtMesh::INVALID_BONE_ID;
		m_vBoneInfo[i].fTimeLength = fTotalTime;
		Ms3dTrans2OETrans(m_vBoneInfo[i].TransLocal, rRot, vPos);
		m_vBoneInfo[i].nNumBoneFrames = nNumKeyFramesRot;
		m_vBoneInfo[i].vBoneFrames.resize(m_vBoneInfo[i].nNumBoneFrames);

		assert(nNumKeyFramesRot == nNumKeyFramesTrans);

		for (int j = 0; j < nNumKeyFramesRot; ++j)
		{
			ms3d_keyframe_rot_t ms3drot;
			pFile->Read(&ms3drot, sizeof(ms3drot));

			// 右手到左手坐标转换
			ms3drot.rotation[0] = -ms3drot.rotation[0];
			ms3drot.rotation[1] = -ms3drot.rotation[1];

			m_vBoneInfo[i].vBoneFrames[j].fTime = ms3drot.time;
			Ms3dRot2OERot(m_vBoneInfo[i].vBoneFrames[j].BoneTrans, ms3drot.rotation);
		}

		for (int j = 0; j < nNumKeyFramesTrans; ++j)
		{
			ms3d_keyframe_pos_t ms3dpos;
			pFile->Read(&ms3dpos, sizeof(ms3dpos));

			// 右手到左手坐标转换
			ms3dpos.position[2] = -ms3dpos.position[2];

			Ms3dPos2OEPos(m_vBoneInfo[i].vBoneFrames[j].BoneTrans, ms3dpos.position);
			Ms3dScale2OEScale(m_vBoneInfo[i].vBoneFrames[j].BoneTrans);
		}
	}

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

			m_vVerts[i].nx += arrTriangles[nTriaIndex].vertexNormals[nInternalIndex][0];
			m_vVerts[i].ny += arrTriangles[nTriaIndex].vertexNormals[nInternalIndex][1];
			m_vVerts[i].nz += arrTriangles[nTriaIndex].vertexNormals[nInternalIndex][2];
			++nCount;
		}

		if (nCount > 1)
		{
			m_vVerts[i].nx /= nCount;
			m_vVerts[i].ny /= nCount;
			m_vVerts[i].nz /= nCount;
		}

		// normalized
		float fDet = sqrtf(m_vVerts[i].nx*m_vVerts[i].nx+m_vVerts[i].ny*m_vVerts[i].ny+m_vVerts[i].nz*m_vVerts[i].nz);
		m_vVerts[i].nx /= fDet;
		m_vVerts[i].ny /= fDet;
		m_vVerts[i].nz /= fDet;
	}

	// calculate uv
	for (word i = 0; i < nNumVertices; ++i)
	{
		bool bPrevUV = false;
		float fPrevU = 0.0f;
		float fPrevV = 0.0f;

		range = mapVertices.equal_range(i);
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
				m_vVerts[i].u = fPrevU;
				m_vVerts[i].v = fPrevV;
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
					VERTEX NewVert = m_vVerts[i];
					NewVert.u = arrTriangles[nTriaIndex].s[nInternalIndex];
					NewVert.v = arrTriangles[nTriaIndex].t[nInternalIndex];
					word nNewVertIndex = (word)m_vVerts.size();
					m_vVerts.push_back(NewVert);

					arrTriangles[nTriaIndex].vertexIndices[nInternalIndex] = nNewVertIndex;
				}
			}
		}
	}

	// store indices
	for (int i = 0; i < nNumTriangles; ++i)
	{
		INDEX Index;
		Index.v1 = arrTriangles[i].vertexIndices[0];
		Index.v2 = arrTriangles[i].vertexIndices[2];
		Index.v3 = arrTriangles[i].vertexIndices[1];
		m_vIndis.push_back(Index);
	}

	// bones
	SetupParentBoneIndex(m_vBoneInfo);

	return true;
}

bool CMs3dConv_Impl::SaveToFile(const tstring& strFile)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	int nNumMeshes = 1;		// TODO: 
	int nNumBones = m_vBoneInfo.size();

	// write header
	COEFmtMesh::FILE_HEADER Header;
	Header.nMagicNumber = COEFmtMesh::MAGIC_NUMBER;
	Header.nVersion = COEFmtMesh::CURRENT_VERSION;
	Header.nNumPieces = nNumMeshes;
	Header.nNumBones = nNumBones;
	pFile->Write(&Header, sizeof(Header));

	// make room for piece list
	uint nPieceListPos = pFile->Tell();
	std::vector<COEFmtMesh::PIECE> vPiece;
	if (nNumMeshes > 0)
	{
		vPiece.resize(nNumMeshes);
		pFile->Write(&vPiece[0], sizeof(COEFmtMesh::PIECE)*nNumMeshes);
	}

	// make room for bone list
	uint nBoneListPos = pFile->Tell();
	std::vector<COEFmtMesh::BONE> vBone;
	if (nNumBones > 0)
	{
		vBone.resize(nNumBones);
		pFile->Write(&vBone[0], sizeof(COEFmtMesh::BONE)*nNumBones);
	}

	// write mesh
	for (int i = 0; i < nNumMeshes; ++i)
	{
		//std::string strName;
		//COEOS::tchar2char(strName, m_vSkinMesh[i].strName.c_str());
		//strncpy_s(vPiece[i].szName, strName.c_str(), COEFmtMesh::PIECE_NAME_SIZE);
		strncpy_s(vPiece[i].szName, "mesh", COEFmtMesh::PIECE_NAME_SIZE);

		vPiece[i].nPieceMask = COEFmtMesh::PM_VISIBLE;
		vPiece[i].nVertexDataMask = COEFmtMesh::VDM_XYZ | COEFmtMesh::VDM_UV | COEFmtMesh::VDM_BONE;
		vPiece[i].nMaterialID = 0;										// TODO: 

		// calculate vertex data offset
		vPiece[i].nNumVerts = (int)m_vVerts.size();
		vPiece[i].nOffVerts = pFile->Tell();

		// write vertex data
		int nNumVerts = (int)m_vVerts.size();
		for (int j = 0; j < nNumVerts; ++j)
		{
			VERTEX& Vertex = m_vVerts[j];

			// position
			float vPos[3];
			vPos[0] = Vertex.x;
			vPos[1] = Vertex.y;
			vPos[2] = Vertex.z;
			pFile->Write(vPos, sizeof(vPos));

			// uv
			float vUV[2];
			vUV[0] = Vertex.u;
			vUV[1] = Vertex.v;
			pFile->Write(vUV, sizeof(vUV));

			// bone weight, index
			int arrBoneID[4] = {Vertex.nBoneID, 0, 0, 0};
			float arrWeight[4] = {1.0f, 0.0f, 0.0f, 0.0f};
			pFile->Write(arrBoneID, sizeof(arrBoneID));
			pFile->Write(arrWeight, sizeof(arrWeight));

			// TODO: add more data
		}

		// calculate index data offset
		vPiece[i].nNumIndis = (int)m_vIndis.size()*3;
		vPiece[i].nOffIndis = pFile->Tell();

		// write index data
		int nNumFaces = (int)m_vIndis.size();
		pFile->Write(&m_vIndis[0], sizeof(INDEX)*nNumFaces);
	}

	// write bone data
	for (int i = 0; i < nNumBones; ++i)
	{
		// bone list info
		std::string strName;
		COEOS::tchar2char(strName, m_vBoneInfo[i].strName.c_str());
		strncpy_s(vBone[i].szName, strName.c_str(), COEFmtMesh::BONE_NAME_SIZE);
		vBone[i].nParentIndex = m_vBoneInfo[i].nParentIndex;
		vBone[i].fTimeLength = m_vBoneInfo[i].fTimeLength;
		vBone[i].BoneTrans = m_vBoneInfo[i].TransLocal;
		vBone[i].nNumBoneFrame = m_vBoneInfo[i].nNumBoneFrames;
		vBone[i].nOffBoneFrame = pFile->Tell();

		// bone data
		int nNumFrame = m_vBoneInfo[i].nNumBoneFrames;
		for (int j = 0; j < nNumFrame; ++j)
		{
			COEFmtMesh::BONE_FRAME& BoneFrame = m_vBoneInfo[i].vBoneFrames[j];
			pFile->Write(&BoneFrame, sizeof(BoneFrame));
		}
	}

	// write true piece list
	pFile->Seek(nPieceListPos);
	if (nNumMeshes > 0)
	{
		pFile->Write(&vPiece[0], sizeof(COEFmtMesh::PIECE)*nNumMeshes);
	}

	// write true bone list
	pFile->Seek(nBoneListPos);
	if (nNumBones > 0)
	{
		pFile->Write(&vBone[0], sizeof(COEFmtMesh::BONE)*nNumBones);
	}

	SAFE_RELEASE(pFile);
	return true;
}

void CMs3dConv_Impl::Ms3dRot2OERot(COEFmtMesh::BONE_TRANSFORM& TransOut, const float* rRot)
{
	CQuaternion qOut;
	COEMath::BuildQuaternionFromEuler(qOut, rRot[0], rRot[1], rRot[2]);

	TransOut.vRotation[0] = qOut.x;
	TransOut.vRotation[1] = qOut.y;
	TransOut.vRotation[2] = qOut.z;
	TransOut.vRotation[3] = qOut.w;
}

void CMs3dConv_Impl::Ms3dPos2OEPos(COEFmtMesh::BONE_TRANSFORM& TransOut, const float* vPos)
{
	TransOut.vPos[0] = vPos[0];
	TransOut.vPos[1] = vPos[1];
	TransOut.vPos[2] = vPos[2];
}

void CMs3dConv_Impl::Ms3dScale2OEScale(COEFmtMesh::BONE_TRANSFORM& TransOut)
{
	TransOut.vScale[0] = 1.0f;
	TransOut.vScale[1] = 1.0f;
	TransOut.vScale[2] = 1.0f;
}

void CMs3dConv_Impl::Ms3dTrans2OETrans(COEFmtMesh::BONE_TRANSFORM& TransOut, const float* rRot, const float* vPos)
{
	Ms3dRot2OERot(TransOut, rRot);
	Ms3dPos2OEPos(TransOut, vPos);
	Ms3dScale2OEScale(TransOut);
}

int CMs3dConv_Impl::FindParentIndex(const TV_BONE_INFO& vBoneInfo, const tstring& strParentName)
{
	for (TV_BONE_INFO::const_iterator it = vBoneInfo.begin(); it != vBoneInfo.end(); ++it)
	{
		if (it->strName == strParentName) return it->nIndex;
	}

	return COEFmtMesh::INVALID_BONE_ID;
}

void CMs3dConv_Impl::SetupParentBoneIndex(TV_BONE_INFO& vBoneInfo)
{
	for (TV_BONE_INFO::iterator it = vBoneInfo.begin(); it != vBoneInfo.end(); ++it)
	{
		it->nParentIndex = FindParentIndex(vBoneInfo, it->strParentName);
	}
}
