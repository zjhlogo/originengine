/*!
 * \file MeshExporter.cpp
 * \date 1-8-2009 10:36:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshExporter.h"
#include <OEInterfaces.h>
#include <OEOS.h>

DWORD WINAPI DummyFunc(LPVOID arg)
{
	return 0;
}

CMeshExporter::CMeshExporter()
{
	Init();
}

CMeshExporter::~CMeshExporter()
{
	Destroy();
}

void CMeshExporter::Init()
{
	m_pInterface = NULL;

	m_bExportMesh = true;
	m_bExportBone = true;

	m_bOptimizeRotation = false;
	m_fOptimizeRotation = 0.0f;

	m_bOptimizePosition = false;
	m_fOptimizePosition = 0.0f;

	m_bOptimizeScale = false;
	m_fOptimizeScale = 0.0f;
}

void CMeshExporter::Destroy()
{
	// TODO: 
}

int CMeshExporter::ExtCount()
{
	return 1;
}

const TCHAR* CMeshExporter::Ext(int n)
{
	return _T("mesh");
}

const TCHAR* CMeshExporter::LongDesc()
{
	return _T("Export to origin engine mesh file format");
}

const TCHAR* CMeshExporter::ShortDesc()
{
	return _T("Origin Engine Mesh");
}

const TCHAR* CMeshExporter::AuthorName()
{
	return _T("zjhlogo");
}

const TCHAR* CMeshExporter::CopyrightMessage()
{
	return _T("Copyright (c) 2009, zjhlogo All rights reserved");
}

const TCHAR* CMeshExporter::OtherMessage1()
{
	return _T("Other message1");
}

const TCHAR* CMeshExporter::OtherMessage2()
{
	return _T("Other message2");
}

unsigned int CMeshExporter::Version()
{
	return CURRENT_VERSION;
}

void CMeshExporter::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, _T("Origin Engine Mesh Exporter"), _T("Copyright (c) 2009, zjhlogo All rights reserved"), MB_OK);
}

int CMeshExporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts /* = FALSE */, DWORD options /* = 0 */)
{
	Cleanup();
	m_pInterface = i;

	// initialize
	m_pInterface->ProgressStart(_T("Initialize IGame interfaces"), TRUE, DummyFunc, NULL);
	IGameScene* pIGame = GetIGameInterface();
	IGameConversionManager* pGameConvMgr = GetConversionManager();
	//pGameConvMgr->SetCoordSystem(IGameConversionManager::IGAME_MAX);
	pIGame->InitialiseIGame();

	// collect usefull nodes
	m_pInterface->ProgressStart(_T("Collection nodes"), TRUE, DummyFunc, NULL);
	int nNodeCount = pIGame->GetTopLevelNodeCount();
	for (int i = 0; i < nNodeCount; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/nNodeCount);
		IGameNode* pGameNode = pIGame->GetTopLevelNode(i);
		CollectNodes(pGameNode);
	}

	// build bone info
	m_pInterface->ProgressStart(_T("Build animation information"), TRUE, DummyFunc, NULL);
	BuildBonesInfo();

	// build mesh info
	m_pInterface->ProgressStart(_T("Build mesh information"), TRUE, DummyFunc, NULL);
	BuildMeshsInfo();

	// save to file
	tstring strFile;
	COEOS::GetFileName(strFile, name);
	SaveMeshFile(strFile + t(".mesh"));
	SaveBoneFile(strFile + t(".bone"));

	pIGame->ReleaseIGame();
	pIGame = NULL;

	m_pInterface->ProgressEnd();
	Cleanup();

	return TRUE;
}

void CMeshExporter::Cleanup()
{
	m_vMeshNode.clear();
	m_vBoneNode.clear();
	m_vSkinMesh.clear();
	m_vBoneInfo.clear();
	m_vBoneInfoMap.clear();
	m_pInterface = NULL;

	m_bExportMesh = true;
	m_bExportBone = true;

	m_bOptimizeRotation = false;
	m_fOptimizeRotation = 0.0f;

	m_bOptimizePosition = false;
	m_fOptimizePosition = 0.0f;

	m_bOptimizeScale = false;
	m_fOptimizeScale = 0.0f;
}

bool CMeshExporter::CollectNodes(IGameNode* pGameNode, IGameNode* pParentGameNode /* = NULL */)
{
	if (!pGameNode) return false;

	IGameObject* pGameObject = pGameNode->GetIGameObject();
	switch (pGameObject->GetIGameType())
	{
	case IGameObject::IGAME_MESH:
		{
			NODE_INFO NodeInfo;
			NodeInfo.pGameNode = pGameNode;
			NodeInfo.pParentGameNode = pParentGameNode;
			m_vMeshNode.push_back(NodeInfo);
		}
		break;
	case IGameObject::IGAME_BONE:
		{
			NODE_INFO NodeInfo;
			NodeInfo.pGameNode = pGameNode;
			NodeInfo.pParentGameNode = pParentGameNode;
			m_vBoneNode.push_back(NodeInfo);
		}
		break;
	}

	// export child
	int nChildCount = pGameNode->GetChildCount();
	for (int i = 0; i < nChildCount; ++i)
	{
		bool bOK = CollectNodes(pGameNode->GetNodeChild(i), pGameNode);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool CMeshExporter::BuildMeshsInfo()
{
	int nMeshCount = m_vMeshNode.size();
	for (int i = 0; i < nMeshCount; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/nMeshCount);

		NODE_INFO& NodeInfo = m_vMeshNode[i];
		SKIN_MESH SkinMesh;
		SkinMesh.strName = NodeInfo.pGameNode->GetName();
		SkinMesh.matLocal = NodeInfo.pGameNode->GetLocalTM();
		bool bOK = DumpMesh(SkinMesh, NodeInfo.pGameNode);
		assert(bOK);
		// TODO: check bOK
		m_vSkinMesh.push_back(SkinMesh);
	}

	return true;
}

bool CMeshExporter::BuildBonesInfo()
{
	int nBoneCount = m_vBoneNode.size();
	for (int i = 0; i < nBoneCount; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/nBoneCount);

		NODE_INFO& NodeInfo = m_vBoneNode[i];

		BONE_INFO BoneInfo;
		BoneInfo.strName = NodeInfo.pGameNode->GetName();
		BoneInfo.pNode = NodeInfo.pGameNode;
		BoneInfo.pParentNode = NodeInfo.pParentGameNode;
		BoneInfo.nID = (int)m_vBoneInfo.size();
		BoneInfo.nParentID = COEFmtBone::INVALID_BONE_ID;
		MaxMat2OeMat(BoneInfo.matLocal, NodeInfo.pGameNode->GetLocalTM());
		bool bOK = DumpController(BoneInfo, NodeInfo.pGameNode);
		assert(bOK);
		// TODO: check bOK
		m_vBoneInfo.push_back(BoneInfo);
		m_vBoneInfoMap.insert(std::make_pair(BoneInfo.pNode, BoneInfo.nID));
	}

	int nBoneInfoCount = m_vBoneInfo.size();
	for (int i = 0; i < nBoneInfoCount; ++i)
	{
		BONE_INFO& BoneInfo = m_vBoneInfo[i];
		if (BoneInfo.pParentNode == NULL) continue;

		TM_BONE_INFO::iterator itfound = m_vBoneInfoMap.find(BoneInfo.pParentNode);
		if (itfound != m_vBoneInfoMap.end())
		{
			BoneInfo.nParentID = itfound->second;
		}
		else
		{
			assert(false);
			tstring strNodeName = BoneInfo.pNode->GetName();
		}
	}

	return true;
}

bool CMeshExporter::SaveMeshFile(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	// write header
	COEFmtMesh::FILE_HEADER Header;
	Header.nMagicNumber = COEFmtMesh::MAGIC_NUMBER;
	Header.nVersion = COEFmtMesh::CURRENT_VERSION;
	Header.nNumPieces = (int)m_vSkinMesh.size();
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
	m_pInterface->ProgressStart(_T("Save mesh data"), TRUE, DummyFunc, NULL);
	for (int i = 0; i < Header.nNumPieces; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/Header.nNumPieces);

		COEFmtMesh::PIECE& Piece = vPiece[i];
		SKIN_MESH& SkinMesh = m_vSkinMesh[i];

		std::string strName;
		COEOS::tchar2char(strName, SkinMesh.strName.c_str());
		strncpy_s(Piece.szName, COEFmtMesh::PIECE_NAME_SIZE, strName.c_str(), _TRUNCATE);

		Piece.nPieceMask = COEFmtMesh::PM_VISIBLE;
		Piece.nVertexDataMask = COEFmtMesh::VDM_XYZ | COEFmtMesh::VDM_UV | COEFmtMesh::VDM_NXNYNZ | COEFmtMesh::VDM_TXTYTZ | COEFmtMesh::VDM_BONE;
		Piece.nMaterialID = 0;					// TODO: 

		// write vertex data offset
		Piece.nNumVerts = (int)SkinMesh.vVertSlots.size();
		Piece.nOffVerts = pFile->Tell();

		// write vertex data
		int nNumVerts = (int)SkinMesh.vVertSlots.size();
		for (int j = 0; j < nNumVerts; ++j)
		{
			VERTEX_SLOT& VertSlot = SkinMesh.vVertSlots[j];

			FILE_VERTEX FileVert;
			memset(&FileVert, 0, sizeof(FileVert));

			// position 左右手坐标转化
			FileVert.x = VertSlot.pos.x;
			FileVert.y = VertSlot.pos.z;
			FileVert.z = VertSlot.pos.y;

			// uv
			FileVert.u = VertSlot.tex.x;
			FileVert.v = 1.0f - VertSlot.tex.y;

			// normal 左右手坐标转化
			FileVert.nx = VertSlot.normal.x;
			FileVert.ny = VertSlot.normal.z;
			FileVert.nz = VertSlot.normal.y;

			// tangent 左右手坐标转化
			FileVert.tx = VertSlot.tangent.x;
			FileVert.ty = VertSlot.tangent.z;
			FileVert.tz = VertSlot.tangent.y;

			// bone weight, index
			int nSkinCount = (int)VertSlot.Skins.size();
			if (nSkinCount > 4) nSkinCount = 4;
			for (int k = 0; k < nSkinCount; ++k)
			{
				FileVert.nBoneIndex[k] = VertSlot.Skins[k].nBoneIndex;
				FileVert.fWeight[k] = VertSlot.Skins[k].fWeight;
			}

			pFile->Write(&FileVert, sizeof(FileVert));
		}

		// write index data offset
		Piece.nNumIndis = (int)SkinMesh.vFaces.size()*3;
		Piece.nOffIndis = pFile->Tell();

		// write index data
		int nNumFaces = (int)SkinMesh.vFaces.size();
		for (int j = 0; j < nNumFaces; ++j)
		{
			//  左右手坐标转化
			ushort nIndex[3];
			nIndex[0] = (ushort)SkinMesh.vFaces[j].nVertIndex[0];
			nIndex[1] = (ushort)SkinMesh.vFaces[j].nVertIndex[2];
			nIndex[2] = (ushort)SkinMesh.vFaces[j].nVertIndex[1];
			pFile->Write(nIndex, sizeof(nIndex));
		}
	}

	// really write piece list
	pFile->Seek(nPieceListPos);
	if (Header.nNumPieces > 0)
	{
		pFile->Write(&vPiece[0], sizeof(COEFmtMesh::PIECE)*Header.nNumPieces);
	}

	SAFE_RELEASE(pFile);
	return true;
}

bool CMeshExporter::SaveBoneFile(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	// write header
	COEFmtBone::FILE_HEADER Header;
	Header.nMagicNumber = COEFmtBone::MAGIC_NUMBER;
	Header.nVersion = COEFmtBone::CURRENT_VERSION;
	Header.nNumBones = (int)m_vBoneInfo.size();
	pFile->Write(&Header, sizeof(Header));

	// make room for bone list
	uint nBoneListPos = pFile->Tell();
	std::vector<COEFmtBone::BONE> vBone;
	if (Header.nNumBones > 0)
	{
		vBone.resize(Header.nNumBones);
		memset(&vBone[0], 0, sizeof(COEFmtBone::BONE)*Header.nNumBones);
		pFile->Write(&vBone[0], sizeof(COEFmtBone::BONE)*Header.nNumBones);
	}

	// write bone data
	m_pInterface->ProgressStart(_T("Save bone data"), TRUE, DummyFunc, NULL);
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/Header.nNumBones);

		COEFmtBone::BONE& Bone = vBone[i];
		BONE_INFO& BoneInfo = m_vBoneInfo[i];

		// bone list info
		std::string strName;
		COEOS::tchar2char(strName, BoneInfo.strName.c_str());
		strncpy_s(Bone.szName, COEFmtBone::BONE_NAME_SIZE, strName.c_str(), _TRUNCATE);
		Bone.nParentIndex = BoneInfo.nParentID;
		memcpy(Bone.matLocal, BoneInfo.matLocal.m, sizeof(float)*16);

		Bone.fTimeLength = 0.0f;
		Bone.nNumFrameRot = (int)BoneInfo.FrameRot.size();
		Bone.nNumFramePos = (int)BoneInfo.FramePos.size();
		Bone.nNumFrameScale = (int)BoneInfo.FrameScale.size();

		// frame rot
		Bone.nOffFrameRot = pFile->Tell();
		for (TM_KEYFRAME_ROT::iterator it = BoneInfo.FrameRot.begin(); it != BoneInfo.FrameRot.end(); ++it)
		{
			KEYFRAME_ROT& FrameRot = it->second;

			COEFmtBone::FRAME_ROT Frame;
			Frame.fTime = TicksToSec(FrameRot.time);
			Frame.qRot[0] = FrameRot.qRot.x;
			Frame.qRot[1] = FrameRot.qRot.y;
			Frame.qRot[2] = FrameRot.qRot.z;
			Frame.qRot[3] = FrameRot.qRot.w;
			pFile->Write(&Frame, sizeof(Frame));

			if (Bone.fTimeLength < Frame.fTime) Bone.fTimeLength = Frame.fTime;
		}

		// frame pos
		Bone.nOffFramePos = pFile->Tell();
		for (TM_KEYFRAME_POS::iterator it = BoneInfo.FramePos.begin(); it != BoneInfo.FramePos.end(); ++it)
		{
			KEYFRAME_POS& FramePos = it->second;

			COEFmtBone::FRAME_POS Frame;
			Frame.fTime = TicksToSec(FramePos.time);
			Frame.vPos[0] = FramePos.vPos.x;
			Frame.vPos[1] = FramePos.vPos.y;
			Frame.vPos[2] = FramePos.vPos.z;
			pFile->Write(&Frame, sizeof(Frame));

			if (Bone.fTimeLength < Frame.fTime) Bone.fTimeLength = Frame.fTime;
		}

		// frame scale
		Bone.nOffFrameScale = pFile->Tell();
		for (TM_KEYFRAME_SCALE::iterator it = BoneInfo.FrameScale.begin(); it != BoneInfo.FrameScale.end(); ++it)
		{
			KEYFRAME_SCALE& FrameScale = it->second;

			COEFmtBone::FRAME_SCALE Frame;
			Frame.fTime = TicksToSec(FrameScale.time);
			Frame.vScale[0] = FrameScale.vScale.x;
			Frame.vScale[1] = FrameScale.vScale.y;
			Frame.vScale[2] = FrameScale.vScale.z;
			pFile->Write(&Frame, sizeof(Frame));

			if (Bone.fTimeLength < Frame.fTime) Bone.fTimeLength = Frame.fTime;
		}
	}

	// really write bone list
	pFile->Seek(nBoneListPos);
	if (Header.nNumBones > 0)
	{
		pFile->Write(&vBone[0], sizeof(COEFmtBone::BONE)*Header.nNumBones);
	}

	SAFE_RELEASE(pFile);
	return true;
}

bool CMeshExporter::DumpMesh(SKIN_MESH& SkinMeshOut, IGameNode* pGameNode)
{
	IGameObject* pGameObject = pGameNode->GetIGameObject();
	if (pGameObject->GetIGameType() != IGameObject::IGAME_MESH)
	{
		assert(false);
		return false;
	}

	IGameMesh* pGameMesh = (IGameMesh*)pGameObject;

	if (!pGameMesh->InitializeData())
	{
		assert(false);
		return false;
	}

	// init vertex slots
	int nNumVerts = pGameMesh->GetNumberOfVerts();
	VERTEX_SLOT EmptySlot;
	EmptySlot.bUsed = false;
	EmptySlot.nVertIndex = 0;
	EmptySlot.nTexIndex = 0;
	EmptySlot.vClone.clear();
	EmptySlot.pos.Set(0.0f, 0.0f, 0.0f);
	EmptySlot.tex.Set(0.0f, 0.0f);
	EmptySlot.Skins.clear();

	// make room for verts
	for (int i = 0; i < nNumVerts; ++i)
	{
		SkinMeshOut.vVertSlots.push_back(EmptySlot);
	}

	// faces
	int nNumFaces = pGameMesh->GetNumberOfFaces();
	for (int i = 0; i < nNumFaces; ++i)
	{
		FaceEx* pFace = pGameMesh->GetFace(i);

		FACE Face;

		for (int j = 0; j < 3; ++j)
		{
			uint nVertIndex = pFace->vert[j];
			uint nTexIndex = pFace->texCoord[j];

			if (SkinMeshOut.vVertSlots[nVertIndex].bUsed)
			{
				bool bAddNew = true;
				if (SkinMeshOut.vVertSlots[nVertIndex].nTexIndex == nTexIndex)
				{
					bAddNew = false;
				}
				else
				{
					TV_INT& vClone = SkinMeshOut.vVertSlots[nVertIndex].vClone;
					for (TV_INT::iterator it = vClone.begin(); it != vClone.end(); ++it)
					{
						if (SkinMeshOut.vVertSlots[*it].nTexIndex == nTexIndex)
						{
							bAddNew = false;
							nVertIndex = (*it);
							break;
						}
					}
				}

				SkinMeshOut.vVertSlots[nVertIndex].vNormalIndex.insert(pFace->norm[j]);
				int nTangentIndex = pGameMesh->GetFaceVertexTangentBinormal(i, j);
				SkinMeshOut.vVertSlots[nVertIndex].vTangentIndex.insert(nTangentIndex);

				if (bAddNew)
				{
					// add new slot
					int nNewVertIndex = SkinMeshOut.vVertSlots.size();
					SkinMeshOut.vVertSlots.push_back(EmptySlot);
					SkinMeshOut.vVertSlots[nNewVertIndex].bUsed = true;
					SkinMeshOut.vVertSlots[nNewVertIndex].nVertIndex = nVertIndex;
					SkinMeshOut.vVertSlots[nNewVertIndex].nTexIndex = nTexIndex;

					SkinMeshOut.vVertSlots[nVertIndex].vClone.push_back(nNewVertIndex);
					nVertIndex = nNewVertIndex;
				}
			}
			else
			{
				SkinMeshOut.vVertSlots[nVertIndex].vNormalIndex.insert(pFace->norm[j]);
				int nTangentIndex = pGameMesh->GetFaceVertexTangentBinormal(i, j);
				SkinMeshOut.vVertSlots[nVertIndex].vTangentIndex.insert(nTangentIndex);

				// set this slot
				SkinMeshOut.vVertSlots[nVertIndex].bUsed = true;
				SkinMeshOut.vVertSlots[nVertIndex].nVertIndex = nVertIndex;
				SkinMeshOut.vVertSlots[nVertIndex].nTexIndex = nTexIndex;
			}

			Face.nVertIndex[j] = nVertIndex;
		}

		SkinMeshOut.vFaces.push_back(Face);
	}

	// setup vertex data
	for (int i = 0; i < nNumVerts; ++i)
	{
		VERTEX_SLOT& LocalSlot = SkinMeshOut.vVertSlots[i];

		int nNumNormal = 0;
		Point3 vNormal(0.0f, 0.0f, 0.0f);
		for (TS_INT::iterator it = LocalSlot.vNormalIndex.begin(); it != LocalSlot.vNormalIndex.end(); ++it)
		{
			vNormal += pGameMesh->GetNormal(*it, false);
			++nNumNormal;
		}

		assert(nNumNormal);
		LocalSlot.normal = vNormal.Normalize();

		int nNumTangent = 0;
		Point3 vTangent(0.0f, 0.0f, 0.0f);
		for (TS_INT::iterator it = LocalSlot.vTangentIndex.begin(); it != LocalSlot.vTangentIndex.end(); ++it)
		{
			vTangent += pGameMesh->GetTangent(*it);
			++nNumTangent;
		}

		assert(nNumTangent);
		LocalSlot.tangent = vTangent.Normalize();
	}

	int nNewNumVerts = SkinMeshOut.vVertSlots.size();
	for (int i = 0; i < nNewNumVerts; ++i)
	{
		VERTEX_SLOT& LocalSlot = SkinMeshOut.vVertSlots[i];

		// get position
		pGameMesh->GetVertex(LocalSlot.nVertIndex, LocalSlot.pos, false);

		// get normal and tangent
		if (i != LocalSlot.nVertIndex)
		{
			LocalSlot.normal = SkinMeshOut.vVertSlots[LocalSlot.nVertIndex].normal;
			LocalSlot.tangent = SkinMeshOut.vVertSlots[LocalSlot.nVertIndex].tangent;
		}

		// get uv
		pGameMesh->GetTexVertex(LocalSlot.nTexIndex, LocalSlot.tex);
	}

	// skins
	int nNumModifier = pGameObject->GetNumModifiers();
	for (int i = 0; i < nNumModifier; ++i)
	{
		IGameModifier* pGameModifier = pGameObject->GetIGameModifier(i);
		if (!pGameModifier->IsSkin()) continue;
		IGameSkin* pGameSkin = (IGameSkin*)pGameModifier;

		bool bOK = DumpSkin(SkinMeshOut, pGameSkin);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool CMeshExporter::DumpSkin(SKIN_MESH& SkinMeshOut, IGameSkin* pGameSkin)
{
	int nNumSkinVerts = pGameSkin->GetNumOfSkinnedVerts();

	for (int i = 0; i < nNumSkinVerts; ++i)
	{
		int nNumBone = pGameSkin->GetNumberOfBones(i);
		for (int j = 0; j < nNumBone; ++j)
		{
			IGameNode* pBoneNode = pGameSkin->GetIGameBone(i, j);
			TM_BONE_INFO::iterator it = m_vBoneInfoMap.find(pBoneNode);
			if (it == m_vBoneInfoMap.end())
			{
				assert(false);
				continue;
			}

			SKIN Skin;
			Skin.nBoneIndex = it->second;
			Skin.fWeight = pGameSkin->GetWeight(i, j);
			SkinMeshOut.vVertSlots[i].Skins.push_back(Skin);
		}

		SortSkin(SkinMeshOut.vVertSlots[i].Skins);
	}

	int nNumAdditionVerts = SkinMeshOut.vVertSlots.size();
	for (int i = nNumSkinVerts; i < nNumAdditionVerts; ++i)
	{
		int nOldIndex = SkinMeshOut.vVertSlots[i].nVertIndex;
		SkinMeshOut.vVertSlots[i].Skins = SkinMeshOut.vVertSlots[nOldIndex].Skins;
	}

	return true;
}

bool CMeshExporter::DumpController(BONE_INFO& BoneInfo, IGameNode* pGameNode)
{
	IGameControl* pGameControl = pGameNode->GetIGameControl();

	bool bBiped = false;

	// export position controller
	if (pGameControl->IsAnimated(IGAME_POS))
	{
		if (pGameControl->GetControlType(IGAME_POS) == IGameControl::IGAME_BIPED)
		{
			// dump sample keys
			DumpSampleKey(BoneInfo, pGameControl, IGAME_TM);
			bBiped = true;
		}
		else
		{
			DumpPositionController(BoneInfo, pGameControl);
		}
	}

	// export rotation controller
	if (pGameControl->IsAnimated(IGAME_ROT))
	{
		if (pGameControl->GetControlType(IGAME_ROT) == IGameControl::IGAME_BIPED)
		{
			if (!bBiped)
			{
				// dump sample keys
				DumpSampleKey(BoneInfo, pGameControl, IGAME_TM);
				bBiped = true;
			}
		}
		else
		{
			DumpRotationController(BoneInfo, pGameControl);
		}
	}

	// export scale controller
	if (pGameControl->IsAnimated(IGAME_SCALE))
	{
		if (pGameControl->GetControlType(IGAME_SCALE) == IGameControl::IGAME_BIPED)
		{
			if (!bBiped)
			{
				// dump sample keys
				DumpSampleKey(BoneInfo, pGameControl, IGAME_TM);
				bBiped = true;
			}
		}
		else
		{
			DumpScaleController(BoneInfo, pGameControl);
		}
	}

	for (TM_KEYFRAME_ROT::iterator it = BoneInfo.FrameRot.begin(); it != BoneInfo.FrameRot.end(); ++it)
	{
		KEYFRAME_ROT& KeyFrame = it->second;

		if (KeyFrame.nMask & KFM_ROT)
		{
			if (KeyFrame.nMask & KFM_QUAT)
			{
				// TODO: logout
				assert(false);
			}

			COEMath::BuildQuaternionFromEuler(KeyFrame.qRot, KeyFrame.vRot.x, KeyFrame.vRot.y, KeyFrame.vRot.z);
		}
		else if (KeyFrame.nMask & KFM_QUAT)
		{
			if (KeyFrame.nMask & KFM_ROT)
			{
				// TODO: logout
				assert(false);
			}
		}
	}

	return true;
}

bool CMeshExporter::DumpPositionController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_POS);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std pos key
			DumpMaxStdPosKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_POS_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_INDEPENDENT_POS:
		{
			// export independent controller
			DumpIndependentPosKey(BoneInfo, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::DumpRotationController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_ROT);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std rot key
			DumpMaxStdRotKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_EULER:
		{
			// export Euler controller
			DumpEulerRotKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_ROT_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(BoneInfo, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::DumpScaleController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_SCALE);

	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export scale key
			DumpMaxStdScaleKey(BoneInfo, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::DumpMaxStdPosKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS, PosKey[i].bezierKey.pval);
		}
	}
	else if (pGameControl->GetLinearKeys(PosKey, IGAME_POS))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS, PosKey[i].linearKey.pval);
		}
	}

	return true;
}

bool CMeshExporter::DumpIndependentPosKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_X))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_X, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_X))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_X, PosKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Y))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_Y, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Y))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_Y, PosKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Z))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_Z, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Z))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_Z, PosKey[i].linearKey.fval);
		}
	}

	return true;
}

bool CMeshExporter::DumpMaxStdRotKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab RotKey;
	if (pGameControl->GetBezierKeys(RotKey, IGAME_ROT))
	{
		// export Bezier Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, RotKey[i].bezierKey.qval);
		}
	}
	else if (pGameControl->GetLinearKeys(RotKey, IGAME_ROT))
	{
		// export Linear Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, RotKey[i].linearKey.qval);
		}
	}
	else if (pGameControl->GetTCBKeys(RotKey, IGAME_ROT))
	{
		// export TCB Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			Quat qValue = QFromAngAxis(RotKey[i].tcbKey.aval.angle, RotKey[i].tcbKey.aval.axis);
			InsertKeyFrame(BoneInfo, RotKey[i].t, qValue);
		}
	}

	return true;
}

bool CMeshExporter::DumpEulerRotKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab RotKey;

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_X))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_X, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_X))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_X, RotKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Y))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_Y, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Y))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_Y, RotKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Z))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_Z, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Z))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_Z, RotKey[i].linearKey.fval);
		}
	}

	return true;
}

bool CMeshExporter::DumpMaxStdScaleKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab ScaleKey;
	if (pGameControl->GetBezierKeys(ScaleKey, IGAME_SCALE))
	{
		for (int i = 0; i < ScaleKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, ScaleKey[i].t, KFM_SCALE, ScaleKey[i].bezierKey.sval.s);
		}
	}

	return true;
}

bool CMeshExporter::DumpConstraintKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	// TODO: 
	assert(false);

	//IGameConstraint* pGameConstraint = pGameControl->GetConstraint(IGAME_POS);

	//int nConstraintCount = pGameConstraint->NumberOfConstraintNodes();
	//for(int i = 0; i < nConstraintCount; ++i)
	//{
	//	float fWeight = pGameConstraint->GetConstraintWeight(i);
	//	int nNodeID = pGameConstraint->GetConstraintNodes(i)->GetNodeID();

	//	// TODO: export constraint
	//}

	//IPropertyContainer* pPropertyContainer = pGameConstraint->GetIPropertyContainer();
	//int nPropertyCount = pPropertyContainer->GetNumberOfProperties();

	//for(int i = 0; i < nPropertyCount; ++i)
	//{
	//	// TODO: dump property
	//}

	return true;
}

bool CMeshExporter::DumpListKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	int nCount = pGameControl->GetNumOfListSubControls(IGAME_POS);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_POS);
		bool bOK = DumpPositionController(BoneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_ROT);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_ROT);
		bool bOK = DumpRotationController(BoneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_SCALE);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_SCALE);
		bool bOK = DumpScaleController(BoneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool CMeshExporter::DumpSampleKey(BONE_INFO& BoneInfo, IGameControl* pGameControl, IGameControlType eType)
{
	IGameKeyTab Keys;
	if (pGameControl->GetFullSampledKeys(Keys, 1, eType, true))
	{
		for (int i = 0; i < Keys.Count(); ++i)
		{
			switch (eType)
			{
			case IGAME_TM:
				{
					InsertKeyFrame(BoneInfo, Keys[i].t, Keys[i].sampleKey.gval);
				}
				break;
			default:
				{
					assert(false);
				}
				break;
			}
		}

		return true;
	}

	return false;
}

CMeshExporter::KEYFRAME_POS* CMeshExporter::GetKeyFrame(TM_KEYFRAME_POS& FramePos, TimeValue time)
{
	KEYFRAME_POS* pKeyFrame = NULL;

	TM_KEYFRAME_POS::iterator itfound = FramePos.find(time);
	if (itfound != FramePos.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_POS EmptyKeyFrame;
		EmptyKeyFrame.time = time;
		EmptyKeyFrame.nMask = KFM_UNKNOWN;
		FramePos.insert(std::make_pair(EmptyKeyFrame.time, EmptyKeyFrame));

		itfound = FramePos.find(time);
		assert(itfound != FramePos.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

CMeshExporter::KEYFRAME_SCALE* CMeshExporter::GetKeyFrame(TM_KEYFRAME_SCALE& FrameScale, TimeValue time)
{
	KEYFRAME_SCALE* pKeyFrame = NULL;

	TM_KEYFRAME_SCALE::iterator itfound = FrameScale.find(time);
	if (itfound != FrameScale.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_SCALE EmptyKeyFrame;
		EmptyKeyFrame.time = time;
		EmptyKeyFrame.nMask = KFM_UNKNOWN;
		EmptyKeyFrame.vScale.Init(1.0f, 1.0f, 1.0f);
		FrameScale.insert(std::make_pair(EmptyKeyFrame.time, EmptyKeyFrame));

		itfound = FrameScale.find(time);
		assert(itfound != FrameScale.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

CMeshExporter::KEYFRAME_ROT* CMeshExporter::GetKeyFrame(TM_KEYFRAME_ROT& FrameRot, TimeValue time)
{
	KEYFRAME_ROT* pKeyFrame = NULL;

	TM_KEYFRAME_ROT::iterator itfound = FrameRot.find(time);
	if (itfound != FrameRot.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_ROT EmptyKeyFrame;
		EmptyKeyFrame.time = time;
		EmptyKeyFrame.nMask = KFM_UNKNOWN;
		FrameRot.insert(std::make_pair(EmptyKeyFrame.time, EmptyKeyFrame));

		itfound = FrameRot.find(time);
		assert(itfound != FrameRot.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

bool CMeshExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, KEY_FRAME_MASK eMask, float fValue)
{
	switch (eMask)
	{
	case KFM_POS_X:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(BoneInfo.FramePos, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vPos.x = fValue;			// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_POS_Y:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(BoneInfo.FramePos, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vPos.z = fValue;			// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_POS_Z:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(BoneInfo.FramePos, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vPos.y = fValue;			// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_SCALE_X:
		{
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(BoneInfo.FrameScale, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vScale.x = fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_SCALE_Y:
		{
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(BoneInfo.FrameScale, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vScale.z = fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_SCALE_Z:
		{
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(BoneInfo.FrameScale, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vScale.y = fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_ROT_X:
		{
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.FrameRot, time);
			if (pKeyFrame->nMask & (eMask | KFM_QUAT))
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vRot.x = -fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_ROT_Y:
		{
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.FrameRot, time);
			if (pKeyFrame->nMask & (eMask | KFM_QUAT))
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vRot.z = -fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_ROT_Z:
		{
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.FrameRot, time);
			if (pKeyFrame->nMask & (eMask | KFM_QUAT))
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vRot.y = -fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, KEY_FRAME_MASK eMask, const Point3& vValue)
{
	switch (eMask)
	{
	case KFM_POS:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(BoneInfo.FramePos, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			MaxVec2OeVec(pKeyFrame->vPos, vValue);
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_SCALE:
		{
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(BoneInfo.FrameScale, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			MaxVec2OeVec(pKeyFrame->vScale, vValue);
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_ROT:
		{
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.FrameRot, time);
			if (pKeyFrame->nMask & (eMask | KFM_QUAT))
			{
				// TODO: logout
				assert(false);
			}
			MaxEular2OeEular(pKeyFrame->vRot, vValue);
			pKeyFrame->nMask |= eMask;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, const Quat& qValue)
{
	KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.FrameRot, time);
	if (pKeyFrame->nMask & KFM_ROT)
	{
		// TODO: logout
		assert(false);
	}

	MaxQuat2OeQuat(pKeyFrame->qRot, qValue);
	pKeyFrame->nMask |= KFM_QUAT;

	return true;
}

bool CMeshExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, const GMatrix& matValue)
{
	KEYFRAME_POS* pKeyFramePos = GetKeyFrame(BoneInfo.FramePos, time);
	if (pKeyFramePos->nMask & KFM_POS)
	{
		// TODO: logout
		assert(false);
	}

	KEYFRAME_SCALE* pKeyFrameScale = GetKeyFrame(BoneInfo.FrameScale, time);
	if (pKeyFrameScale->nMask & KFM_SCALE)
	{
		// TODO: logout
		assert(false);
	}

	KEYFRAME_ROT* pKeyFrameRot = GetKeyFrame(BoneInfo.FrameRot, time);
	if (pKeyFrameRot->nMask & (KFM_ROT | KFM_QUAT))
	{
		// TODO: logout
		assert(false);
	}

	CMatrix4x4 matTrans;
	MaxMat2OeMat(matTrans, matValue);

	COEMath::GetMatrixTranslation(pKeyFramePos->vPos, matTrans);
	COEMath::GetMatrixScale(pKeyFrameScale->vScale, matTrans);
	COEMath::BuildQuaternionFromMatrix(pKeyFrameRot->qRot, matTrans);
	pKeyFrameScale->nMask |= (KFM_SCALE | KFM_POS | KFM_QUAT);

	return true;
}

void CMeshExporter::MaxMat2OeMat(CMatrix4x4& matOut, const GMatrix& matIn)
{
	matOut.m[0] = matIn[0][0];
	matOut.m[1] = matIn[0][2];
	matOut.m[2] = matIn[0][1];
	matOut.m[3] = 0.0f;

	matOut.m[4] = matIn[2][0];
	matOut.m[5] = matIn[2][2];
	matOut.m[6] = matIn[2][1];
	matOut.m[7] = 0.0f;

	matOut.m[8] = matIn[1][0];
	matOut.m[9] = matIn[1][2];
	matOut.m[10] = matIn[1][1];
	matOut.m[11] = 0.0f;

	matOut.m[12] = matIn[3][0];
	matOut.m[13] = matIn[3][2];
	matOut.m[14] = matIn[3][1];
	matOut.m[15] = 1.0f;
}

void CMeshExporter::OeMat2MaxMat(GMatrix& matOut, const CMatrix4x4& matIn)
{
	matOut[0][0] = matIn.m[0];
	matOut[0][1] = matIn.m[2];
	matOut[0][2] = matIn.m[1];
	matOut[0][3] = 0.0f;

	matOut[1][0] = matIn.m[8];
	matOut[1][1] = matIn.m[10];
	matOut[1][2] = matIn.m[9];
	matOut[1][3] = 0.0f;

	matOut[2][0] = matIn.m[4];
	matOut[2][1] = matIn.m[6];
	matOut[2][2] = matIn.m[5];
	matOut[2][3] = 0.0f;

	matOut[3][0] = matIn.m[12];
	matOut[3][1] = matIn.m[14];
	matOut[3][2] = matIn.m[13];
	matOut[3][3] = 1.0f;
}

void CMeshExporter::MaxQuat2OeQuat(CQuaternion& qOut, const Quat& qIn)
{
	qOut.x = -qIn.x;
	qOut.y = -qIn.z;
	qOut.z = -qIn.y;
	qOut.w = qIn.w;
}

void CMeshExporter::OeQuat2MaxQuat(Quat& qOut, const CQuaternion& qIn)
{
	qOut.x = -qIn.x;
	qOut.y = -qIn.z;
	qOut.z = -qIn.y;
	qOut.w = qIn.w;
}

void CMeshExporter::MaxVec2OeVec(CVector3& vOut, const Point3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void CMeshExporter::OeVec2MaxVec(Point3& vOut, const CVector3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void CMeshExporter::MaxEular2OeEular(CVector3& vOut, const Point3& vIn)
{
	vOut.x = -vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void CMeshExporter::OeEular2MaxEular(Point3& vOut, const CVector3& vIn)
{
	vOut.x = -vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void CMeshExporter::SortSkin(TV_SKIN& vSkin)
{
	int nNumSkins = vSkin.size();

	for (int i = 0; i < nNumSkins-1; ++i)
	{
		int nChoose = i;
		for (int j = i+1; j < nNumSkins; ++j)
		{
			if (vSkin[j].fWeight > vSkin[nChoose].fWeight) nChoose = j;
		}

		if (nChoose != i)
		{
			SKIN Skin = vSkin[i];
			vSkin[i] = vSkin[nChoose];
			vSkin[nChoose] = Skin;
		}
	}
}

bool CMeshExporter::ReadConfig()
{
	tstring strFile = GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
	strFile += t("\\MeshExporterConfig.xml");

	IOEXmlDocument* pXmlDocument = g_pOEXmlMgr->CreateDocument(strFile);
	if (!pXmlDocument) return false;

	IOEXmlNode* pXmlRoot = pXmlDocument->GetRootNode();
	if (!pXmlRoot)
	{
		SAFE_RELEASE(pXmlDocument);
		return false;
	}

	IOEXmlNode* pXmlAnimation = pXmlRoot->FirstChild(t("Animation"));
	if (!pXmlAnimation)
	{
		SAFE_RELEASE(pXmlDocument);
		return false;
	}

	IOEXmlNode* pXmlOptimizeRotation = pXmlAnimation->FirstChild(t("OptimizeRotation"));
	IOEXmlNode* pXmlOptimizePosition = pXmlAnimation->FirstChild(t("OptimizePosition"));
	IOEXmlNode* pXmlOptimizeScale = pXmlAnimation->FirstChild(t("OptimizeScale"));

	if (!pXmlOptimizeRotation || !pXmlOptimizePosition || !pXmlOptimizeScale)
	{
		SAFE_RELEASE(pXmlDocument);
		return false;
	}

	int nOptimizeRotation = 0;
	int nOptimizePosition = 0;
	int nOptimizeScale = 0;

	pXmlOptimizeRotation->GetAttribute(nOptimizeRotation, t("enable"));
	if (nOptimizeRotation != 0)
	{
		m_bOptimizeRotation = true;
		pXmlOptimizeRotation->GetText(m_fOptimizeRotation);
	}

	pXmlOptimizePosition->GetAttribute(nOptimizePosition, t("enable"));
	if (nOptimizePosition != 0)
	{
		m_bOptimizePosition = true;
		pXmlOptimizePosition->GetText(m_fOptimizePosition);
	}

	pXmlOptimizeScale->GetAttribute(nOptimizeScale, t("enable"));
	if (nOptimizeScale != 0)
	{
		m_bOptimizeScale = true;
		pXmlOptimizeScale->GetText(m_fOptimizeScale);
	}

	SAFE_RELEASE(pXmlDocument);
	return true;
}

bool CMeshExporter::SaveConfig()
{
	// TODO: 
	return true;
}
