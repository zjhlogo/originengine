/*!
 * \file Ms3dConverter_Impl.cpp
 * \date 1-3-2010 21:20:26
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "Ms3dConverter_Impl.h"

#include <OEBase/IOEFileMgr.h>
#include <libOEMath/OEMath.h>
#include <libOEBase/OEOS.h>

#include <map>
#include <math.h>
#include <assert.h>

CMs3dConverter_Impl::CMs3dConverter_Impl()
{
	Init();
}

CMs3dConverter_Impl::~CMs3dConverter_Impl()
{
	Destroy();
}

void CMs3dConverter_Impl::Init()
{
	// TODO: 
}

void CMs3dConverter_Impl::Destroy()
{
	// TODO: 
}

bool CMs3dConverter_Impl::DoConvert(const tstring& strFileIn, const tstring& strFileOut)
{
	if (!LoadFromFile(strFileIn)) return false;

	tstring strMeshFile;
	COEOS::GetFileNameWithoutExt(strMeshFile, strFileOut);
	strMeshFile += TS(".mesh");
	if (!SaveToMeshFile(strMeshFile)) return false;

	tstring strSkeletonFile;
	COEOS::GetFileNameWithoutExt(strSkeletonFile, strFileOut);
	strSkeletonFile += TS(".skel");
	if (!SaveToSkeletonFile(strSkeletonFile)) return false;

	return true;
}

bool CMs3dConverter_Impl::CanConvert(const tstring& strFile)
{
	tstring strExt;
	COEOS::GetFileExt(strExt, strFile);

	COEOS::tolower(strExt, strExt);
	if (strExt == TS("ms3d")) return true;

	return false;
}

bool CMs3dConverter_Impl::LoadFromFile(const tstring& strFile)
{
	m_vVerts.clear();
	m_vIndis.clear();
	m_vBoneInfo.clear();
	m_vBoundingBoxMin.Init(COEMath::FLOAT_MAX, COEMath::FLOAT_MAX, COEMath::FLOAT_MAX);
	m_vBoundingBoxMax.Init(COEMath::FLOAT_MIN, COEMath::FLOAT_MIN, COEMath::FLOAT_MIN);

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

		if (NewVert.x < m_vBoundingBoxMin.x) m_vBoundingBoxMin.x = NewVert.x;
		if (NewVert.x > m_vBoundingBoxMax.x) m_vBoundingBoxMax.x = NewVert.x;

		if (NewVert.y < m_vBoundingBoxMin.y) m_vBoundingBoxMin.y = NewVert.y;
		if (NewVert.y > m_vBoundingBoxMax.y) m_vBoundingBoxMax.y = NewVert.y;

		if (NewVert.z < m_vBoundingBoxMin.z) m_vBoundingBoxMin.z = NewVert.z;
		if (NewVert.z > m_vBoundingBoxMax.z) m_vBoundingBoxMax.z = NewVert.z;

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
		m_vBoneInfo[i].nParentIndex = COEFmtSkeleton::INVALID_BONE_ID;
		m_vBoneInfo[i].fTimeLength = fTotalTime;

		Ms3dTrans2OETrans(m_vBoneInfo[i].matLocal, rRot, vPos);

		for (int j = 0; j < nNumKeyFramesRot; ++j)
		{
			ms3d_keyframe_rot_t ms3drot;
			pFile->Read(&ms3drot, sizeof(ms3drot));

			// 右手到左手坐标转换
			ms3drot.rotation[0] = -ms3drot.rotation[0];
			ms3drot.rotation[1] = -ms3drot.rotation[1];

			COEFmtSkeleton::FRAME_ROT FrameRot;
			FrameRot.fTime = ms3drot.time;
			CQuaternion qRot;
			Ms3dRot2OERot(qRot, ms3drot.rotation);
			FrameRot.qRot[0] = qRot.x;
			FrameRot.qRot[1] = qRot.y;
			FrameRot.qRot[2] = qRot.z;
			FrameRot.qRot[3] = qRot.w;
			m_vBoneInfo[i].vFrameRot.push_back(FrameRot);
		}

		for (int j = 0; j < nNumKeyFramesTrans; ++j)
		{
			ms3d_keyframe_pos_t ms3dpos;
			pFile->Read(&ms3dpos, sizeof(ms3dpos));

			// 右手到左手坐标转换
			ms3dpos.position[2] = -ms3dpos.position[2];

			COEFmtSkeleton::FRAME_POS FramePos;
			FramePos.fTime = ms3dpos.time;
			CVector3 vPos;
			Ms3dPos2OEPos(vPos, ms3dpos.position);
			FramePos.vPos[0] = vPos.x;
			FramePos.vPos[1] = vPos.y;
			FramePos.vPos[2] = vPos.z;
			m_vBoneInfo[i].vFramePos.push_back(FramePos);
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

bool CMs3dConverter_Impl::SaveToMeshFile(const tstring& strFile)
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
	Header.nNumPieces = 1;									// TODO: 
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
		strncpy_s(vPiece[i].szName, COEFmtMesh::PIECE_NAME_SIZE, "ms3d_mesh", _TRUNCATE);
		vPiece[i].nPieceMask = COEFmtMesh::PM_VISIBLE;
		vPiece[i].nVertexDataMask = COEFmtMesh::VDM_POSITION | COEFmtMesh::VDM_TEXCOORD0 | COEFmtMesh::VDM_SKELECTON;
		vPiece[i].nMaterialID = 0;										// TODO: 

		// calculate vertex data offset
		vPiece[i].nNumVerts = (int)m_vVerts.size();
		vPiece[i].nOffVerts = pFile->Tell();

		// write vertex data
		int nNumVerts = (int)m_vVerts.size();
		for (int j = 0; j < nNumVerts; ++j)
		{
			VERTEX& Vertex = m_vVerts[j];

			FILE_VERTEX FileVert;
			memset(&FileVert, 0, sizeof(FileVert));

			// position
			FileVert.x = Vertex.x;
			FileVert.y = Vertex.y;
			FileVert.z = Vertex.z;

			// uv
			FileVert.u = Vertex.u;
			FileVert.v = Vertex.v;

			// bone weight, index
			FileVert.nBoneIndex[0] = Vertex.nBoneID;
			FileVert.fWeight[0] = 1.0f;

			pFile->Write(&FileVert, sizeof(FileVert));
		}

		// calculate index data offset
		vPiece[i].nNumIndis = (int)m_vIndis.size()*3;
		vPiece[i].nOffIndis = pFile->Tell();

		// write index data
		int nNumFaces = (int)m_vIndis.size();
		pFile->Write(&m_vIndis[0], sizeof(INDEX)*nNumFaces);
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

bool CMs3dConverter_Impl::SaveToSkeletonFile(const tstring& strFile)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFile, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	// write header
	COEFmtSkeleton::FILE_HEADER Header;
	Header.nMagicNumber = COEFmtSkeleton::MAGIC_NUMBER;
	Header.nVersion = COEFmtSkeleton::CURRENT_VERSION;
	Header.nNumBones = m_vBoneInfo.size();
	pFile->Write(&Header, sizeof(Header));

	// make room for bone list
	uint nBoneListPos = pFile->Tell();
	std::vector<COEFmtSkeleton::BONE> vBone;
	if (Header.nNumBones > 0)
	{
		vBone.resize(Header.nNumBones);
		memset(&vBone[0], 0, sizeof(COEFmtSkeleton::BONE)*Header.nNumBones);
		pFile->Write(&vBone[0], sizeof(COEFmtSkeleton::BONE)*Header.nNumBones);
	}

	// write bone
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		COEFmtSkeleton::BONE& Bone = vBone[i];
		BONE_INFO& BoneInfo = m_vBoneInfo[i];

		// bone list info
		std::string strName;
		COEOS::tchar2char(strName, BoneInfo.strName.c_str());
		strncpy_s(Bone.szName, COEFmtSkeleton::BONE_NAME_SIZE, strName.c_str(), _TRUNCATE);

		Bone.nParentIndex = BoneInfo.nParentIndex;
		memcpy(Bone.matLocal, BoneInfo.matLocal.m, sizeof(float)*16);

		Bone.fTimeLength = 0.0f;
		Bone.nNumFrameRot = (int)BoneInfo.vFrameRot.size();
		Bone.nNumFramePos = (int)BoneInfo.vFramePos.size();
		Bone.nNumFrameScale = (int)BoneInfo.vFrameScale.size();

		// frame rot
		Bone.nOffFrameRot = pFile->Tell();
		for (TV_FRAME_ROT::iterator it = BoneInfo.vFrameRot.begin(); it != BoneInfo.vFrameRot.end(); ++it)
		{
			COEFmtSkeleton::FRAME_ROT& FrameRot = (*it);
			pFile->Write(&FrameRot, sizeof(FrameRot));

			if (Bone.fTimeLength < FrameRot.fTime) Bone.fTimeLength = FrameRot.fTime;
		}

		// frame pos
		Bone.nOffFramePos = pFile->Tell();
		for (TV_FRAME_POS::iterator it = BoneInfo.vFramePos.begin(); it != BoneInfo.vFramePos.end(); ++it)
		{
			COEFmtSkeleton::FRAME_POS& FramePos = (*it);
			pFile->Write(&FramePos, sizeof(FramePos));

			if (Bone.fTimeLength < FramePos.fTime) Bone.fTimeLength = FramePos.fTime;
		}

		// frame scale
		Bone.nOffFrameScale = pFile->Tell();
		for (TV_FRAME_SCALE::iterator it = BoneInfo.vFrameScale.begin(); it != BoneInfo.vFrameScale.end(); ++it)
		{
			COEFmtSkeleton::FRAME_SCALE& FrameScale = (*it);
			pFile->Write(&FrameScale, sizeof(FrameScale));

			if (Bone.fTimeLength < FrameScale.fTime) Bone.fTimeLength = FrameScale.fTime;
		}
	}

	// really write bone list
	pFile->Seek(nBoneListPos);
	if (Header.nNumBones > 0)
	{
		pFile->Write(&vBone[0], sizeof(COEFmtSkeleton::BONE)*Header.nNumBones);
	}

	SAFE_RELEASE(pFile);
	return true;
}

void CMs3dConverter_Impl::Ms3dRot2OERot(CQuaternion& qOut, const float* rRot)
{
	COEMath::BuildQuaternionFromEulerXYZ(qOut, rRot[0], rRot[1], rRot[2]);
}

void CMs3dConverter_Impl::Ms3dPos2OEPos(CVector3& vOut, const float* vPos)
{
	vOut.x = vPos[0];
	vOut.y = vPos[1];
	vOut.z = vPos[2];
}

void CMs3dConverter_Impl::Ms3dScale2OEScale(CVector3& vScale)
{
	vScale.x = 1.0f;
	vScale.y = 1.0f;
	vScale.z = 1.0f;
}

void CMs3dConverter_Impl::Ms3dTrans2OETrans(CMatrix4x4& matOut, const float* rRot, const float* vPos)
{
	CQuaternion qOERot;
	Ms3dRot2OERot(qOERot, rRot);

	CVector3 vOEPos;
	Ms3dPos2OEPos(vOEPos, vPos);

	CVector3 vOEScale;
	Ms3dScale2OEScale(vOEScale);

	COEMath::BuildMatrixFromQuaternion(matOut, qOERot);
	COEMath::SetMatrixTranslation(matOut, vOEPos);
	COEMath::SetMatrixScale(matOut, vOEScale);
}

int CMs3dConverter_Impl::FindParentIndex(const TV_BONE_INFO& vBoneInfo, const tstring& strParentName)
{
	for (TV_BONE_INFO::const_iterator it = vBoneInfo.begin(); it != vBoneInfo.end(); ++it)
	{
		if (it->strName == strParentName) return it->nIndex;
	}

	return COEFmtSkeleton::INVALID_BONE_ID;
}

void CMs3dConverter_Impl::SetupParentBoneIndex(TV_BONE_INFO& vBoneInfo)
{
	for (TV_BONE_INFO::iterator it = vBoneInfo.begin(); it != vBoneInfo.end(); ++it)
	{
		it->nParentIndex = FindParentIndex(vBoneInfo, it->strParentName);
	}
}
