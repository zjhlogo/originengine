/*!
 * \file MeshExporter.cpp
 * \date 1-8-2009 10:36:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshExporter.h"
#include <OEOS.h>

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
	// TODO: 
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

	IGameScene* pIGame = GetIGameInterface();
	IGameConversionManager* pGameConvMgr = GetConversionManager();
	pGameConvMgr->SetCoordSystem(IGameConversionManager::IGAME_D3D);
	pIGame->InitialiseIGame();

	// collect usefull nodes
	int nNodeCount = pIGame->GetTopLevelNodeCount();
	for (int i = 0; i < nNodeCount; ++i)
	{
		IGameNode* pGameNode = pIGame->GetTopLevelNode(i);
		CollectNodes(pGameNode);
	}

	// build bone info
	BuildBonesInfo();

	// build mesh info
	BuildMeshsInfo();

	// save to file
	tstring strFile;
	COEOS::GetFileName(strFile, name);
	strFile += t(".mesh");
	SaveToFile(strFile);

	pIGame->ReleaseIGame();
	pIGame = NULL;

	Cleanup();

	return TRUE;
}

void CMeshExporter::Cleanup()
{
	m_TimeValueSet.clear();
	m_vMeshNode.clear();
	m_vBoneNode.clear();
	m_vSkinMesh.clear();
	m_vBoneInfo.clear();
	m_vBoneInfoMap.clear();
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
	for (TV_NODE_INFO::iterator it = m_vMeshNode.begin(); it != m_vMeshNode.end(); ++it)
	{
		NODE_INFO& NodeInfo = (*it);

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
	for (TV_NODE_INFO::iterator it = m_vBoneNode.begin(); it != m_vBoneNode.end(); ++it)
	{
		NODE_INFO& NodeInfo = (*it);

		BONE_INFO BoneInfo;
		BoneInfo.strName = NodeInfo.pGameNode->GetName();
		BoneInfo.pNode = NodeInfo.pGameNode;
		BoneInfo.pParentNode = NodeInfo.pParentGameNode;
		BoneInfo.nID = (int)m_vBoneInfo.size();
		BoneInfo.nParentID = COEFmtMesh::INVALID_BONE_ID;
		BoneInfo.matLocal = NodeInfo.pGameNode->GetLocalTM();
		bool bOK = DumpController(BoneInfo.vFrameInfo, NodeInfo.pGameNode);
		assert(bOK);
		// TODO: check bOK
		m_vBoneInfo.push_back(BoneInfo);
		m_vBoneInfoMap.insert(std::make_pair(BoneInfo.pNode, BoneInfo.nID));
	}

	for (TV_BONE_INFO::iterator it = m_vBoneInfo.begin(); it != m_vBoneInfo.end(); ++it)
	{
		BONE_INFO& BoneInfo = (*it);
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

bool CMeshExporter::SaveToFile(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	int nNumMeshes = (int)m_vSkinMesh.size();
	int nNumBones = (int)m_vBoneInfo.size();

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
		memset(&vPiece[0], 0, sizeof(COEFmtMesh::PIECE)*nNumMeshes);
		pFile->Write(&vPiece[0], sizeof(COEFmtMesh::PIECE)*nNumMeshes);
	}

	// make room for bone list
	uint nBoneListPos = pFile->Tell();
	std::vector<COEFmtMesh::BONE> vBone;
	if (nNumBones > 0)
	{
		vBone.resize(nNumBones);
		memset(&vBone[0], 0, sizeof(COEFmtMesh::BONE)*nNumBones);
		pFile->Write(&vBone[0], sizeof(COEFmtMesh::BONE)*nNumBones);
	}

	// write mesh
	for (int i = 0; i < nNumMeshes; ++i)
	{
		std::string strName;
		COEOS::tchar2char(strName, m_vSkinMesh[i].strName.c_str());
		strncpy_s(vPiece[i].szName, COEFmtMesh::PIECE_NAME_SIZE, strName.c_str(), _TRUNCATE);

		vPiece[i].nPieceMask = COEFmtMesh::PM_VISIBLE;
		vPiece[i].nVertexDataMask = COEFmtMesh::VDM_XYZ | COEFmtMesh::VDM_UV | COEFmtMesh::VDM_NXNYNZ | COEFmtMesh::VDM_TXTYTZ | COEFmtMesh::VDM_BONE;
		vPiece[i].nMaterialID = 0;					// TODO: 

		// write vertex data offset
		vPiece[i].nNumVerts = (int)m_vSkinMesh[i].vVertSlots.size();
		vPiece[i].nOffVerts = pFile->Tell();

		// write vertex data
		int nNumVerts = (int)m_vSkinMesh[i].vVertSlots.size();
		for (int j = 0; j < nNumVerts; ++j)
		{
			VERTEX_SLOT& VertSlot = m_vSkinMesh[i].vVertSlots[j];

			FILE_VERTEX FileVert;
			memset(&FileVert, 0, sizeof(FileVert));

			// position
			FileVert.x = VertSlot.pos.x;
			FileVert.y = VertSlot.pos.y;
			FileVert.z = VertSlot.pos.z;

			// uv
			FileVert.u = VertSlot.tex.x;
			FileVert.v = VertSlot.tex.y;

			// normal
			FileVert.nx = VertSlot.normal.x;
			FileVert.ny = VertSlot.normal.y;
			FileVert.nz = VertSlot.normal.z;

			// tangent
			FileVert.tx = VertSlot.tangent.x;
			FileVert.ty = VertSlot.tangent.y;
			FileVert.tz = VertSlot.tangent.z;

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
		vPiece[i].nNumIndis = (int)m_vSkinMesh[i].vFaces.size()*3;
		vPiece[i].nOffIndis = pFile->Tell();

		// write index data
		int nNumFaces = (int)m_vSkinMesh[i].vFaces.size();
		for (int j = 0; j < nNumFaces; ++j)
		{
			ushort nIndex[3];
			nIndex[0] = (ushort)m_vSkinMesh[i].vFaces[j].nVertIndex[0];
			nIndex[1] = (ushort)m_vSkinMesh[i].vFaces[j].nVertIndex[1];
			nIndex[2] = (ushort)m_vSkinMesh[i].vFaces[j].nVertIndex[2];
			pFile->Write(nIndex, sizeof(nIndex));
		}
	}

	// write bone data
	for (int i = 0; i < nNumBones; ++i)
	{
		// bone list info
		std::string strName;
		COEOS::tchar2char(strName, m_vBoneInfo[i].strName.c_str());
		strncpy_s(vBone[i].szName, COEFmtMesh::BONE_NAME_SIZE, strName.c_str(), _TRUNCATE);
		vBone[i].nParentIndex = m_vBoneInfo[i].nParentID;
		if (!m_vBoneInfo[i].vFrameInfo.empty())
		{
			TimeValue TimeTick = m_vBoneInfo[i].vFrameInfo.rbegin()->Time;
			vBone[i].fTimeLength = TicksToSec(TimeTick);
		}
		else
		{
			vBone[i].fTimeLength = 0.0f;
		}
		GMatrix2BoneTransform(vBone[i].BoneTrans, m_vBoneInfo[i].matLocal);
		vBone[i].nNumBoneFrame = (int)m_vBoneInfo[i].vFrameInfo.size();
		vBone[i].nOffBoneFrame = pFile->Tell();

		// bone data
		int nNumFrame = (int)m_vBoneInfo[i].vFrameInfo.size();
		for (int j = 0; j < nNumFrame; ++j)
		{
			FRAME_INFO& FrameInfo = m_vBoneInfo[i].vFrameInfo[j];

			COEFmtMesh::BONE_FRAME BoneFrame;
			BoneFrame.fTime = TicksToSec(FrameInfo.Time);
			GMatrix2BoneTransform(BoneFrame.BoneTrans, FrameInfo.matAnim);
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

		pGameMesh->GetVertex(LocalSlot.nVertIndex, LocalSlot.pos, false);

		if (i != LocalSlot.nVertIndex)
		{
			LocalSlot.normal = SkinMeshOut.vVertSlots[LocalSlot.nVertIndex].normal;
			LocalSlot.tangent = SkinMeshOut.vVertSlots[LocalSlot.nVertIndex].tangent;
		}

		pGameMesh->GetTexVertex(LocalSlot.nTexIndex, LocalSlot.tex);
		LocalSlot.tex.y += 1.0f;
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

bool CMeshExporter::DumpController(TV_FRAME_INFO& vFrameInfoOut, IGameNode* pGameNode)
{
	IGameControl* pGameControl = pGameNode->GetIGameControl();

	bool bBiped = false;
	TM_KEY_FRAME KeyFrames;

	// export position controller
	if (pGameControl->IsAnimated(IGAME_POS))
	{
		if (pGameControl->GetControlType(IGAME_POS) == IGameControl::IGAME_BIPED)
		{
			// dump sample keys
			DumpSampleKey(KeyFrames, pGameControl, IGAME_TM);
			bBiped = true;
		}
		else
		{
			DumpPositionController(KeyFrames, pGameControl);
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
				DumpSampleKey(KeyFrames, pGameControl, IGAME_TM);
				bBiped = true;
			}
		}
		else
		{
			DumpRotationController(KeyFrames, pGameControl);
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
				DumpSampleKey(KeyFrames, pGameControl, IGAME_TM);
				bBiped = true;
			}
		}
		else
		{
			DumpScaleController(KeyFrames, pGameControl);
		}
	}

	for (TM_KEY_FRAME::iterator it = KeyFrames.begin(); it != KeyFrames.end(); ++it)
	{
		KEY_FRAME& KeyFrame = it->second;

		FRAME_INFO FrameInfo;
		FrameInfo.Time = KeyFrame.time;

		if (KeyFrame.nMask == KFM_MATRIX)
		{
			CMatrix4x42GMatrix(FrameInfo.matAnim, KeyFrame.matFull);
		}
		else if (KeyFrame.nMask == KFM_QUAT)
		{
			CMatrix4x4 matFinal;
			COEMath::BuildMatrixFromQuaternion(matFinal, KeyFrame.rRot);
			COEMath::SetMatrixScale(matFinal, KeyFrame.vScale);
			COEMath::SetMatrixTranslation(matFinal, KeyFrame.vPos);

			CMatrix4x42GMatrix(FrameInfo.matAnim, matFinal);
		}
		else
		{
			CMatrix4x4 matFinal;
			CQuaternion qRot;

			COEMath::BuildQuaternionFromEuler(qRot, KeyFrame.vRot.x, KeyFrame.vRot.y, KeyFrame.vRot.z);
			COEMath::BuildMatrixFromQuaternion(matFinal, qRot);
			COEMath::SetMatrixScale(matFinal, KeyFrame.vScale);
			COEMath::SetMatrixTranslation(matFinal, KeyFrame.vPos);

			CMatrix4x42GMatrix(FrameInfo.matAnim, matFinal);
		}

		vFrameInfoOut.push_back(FrameInfo);
	}

	//// 
	//IGameKeyTab TMKey;
	//pGameControl->GetFullSampledKeys(TMKey, 1, IGAME_TM, true);

	//TM_KEY_FRAME::const_iterator it = TimeValueSet.begin();
	//int nCount = TMKey.Count();

	//for (int i = 0; i < nCount; ++i)
	//{
	//	if (it == TimeValueSet.end()) break;
	//	if (TMKey[i].t < (*it)) continue;

	//	FRAME_INFO FrameInfo;
	//	FrameInfo.Time = TMKey[i].t;
	//	FrameInfo.matAnim = TMKey[i].sampleKey.gval;
	//	vFrameInfoOut.push_back(FrameInfo);

	//	++it;
	//}

	return true;
}

bool CMeshExporter::DumpPositionController(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_POS);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std pos key
			DumpMaxStdPosKey(KeyFrames, pGameControl);
		}
		break;
	case IGameControl::IGAME_POS_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(KeyFrames, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(KeyFrames, pGameControl);
		}
		break;
	case IGameControl::IGAME_INDEPENDENT_POS:
		{
			// export independent controller
			DumpIndependentPosKey(KeyFrames, pGameControl);
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

bool CMeshExporter::DumpRotationController(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_ROT);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std rot key
			DumpMaxStdRotKey(KeyFrames, pGameControl);
		}
		break;
	case IGameControl::IGAME_EULER:
		{
			// export Euler controller
			DumpEulerRotKey(KeyFrames, pGameControl);
		}
		break;
	case IGameControl::IGAME_ROT_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(KeyFrames, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(KeyFrames, pGameControl);
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

bool CMeshExporter::DumpScaleController(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_SCALE);

	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export scale key
			DumpMaxStdScaleKey(KeyFrames, pGameControl);
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

bool CMeshExporter::DumpMaxStdPosKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, PosKey[i].t, KFM_POS, PosKey[i].bezierKey.pval);
		}
	}
	else if (pGameControl->GetLinearKeys(PosKey, IGAME_POS))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, PosKey[i].t, KFM_POS, PosKey[i].linearKey.pval);
		}
	}

	return true;
}

bool CMeshExporter::DumpIndependentPosKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_X))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, PosKey[i].t, KFM_POS_X, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_X))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, PosKey[i].t, KFM_POS_X, PosKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Y))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, PosKey[i].t, KFM_POS_Y, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Y))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, PosKey[i].t, KFM_POS_Y, PosKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Z))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, PosKey[i].t, KFM_POS_Z, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Z))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, PosKey[i].t, KFM_POS_Z, PosKey[i].linearKey.fval);
		}
	}

	return true;
}

bool CMeshExporter::DumpMaxStdRotKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	IGameKeyTab RotKey;
	if (pGameControl->GetBezierKeys(RotKey, IGAME_ROT))
	{
		// export Bezier Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_QUAT, RotKey[i].bezierKey.qval);
		}
	}
	else if (pGameControl->GetLinearKeys(RotKey, IGAME_ROT))
	{
		// export Linear Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_QUAT, RotKey[i].linearKey.qval);
		}
	}
	else if (pGameControl->GetTCBKeys(RotKey, IGAME_ROT))
	{
		// export TCB Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			Quat qValue = QFromAngAxis(RotKey[i].tcbKey.aval.angle, RotKey[i].tcbKey.aval.axis);
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_QUAT, qValue);
		}
	}

	return true;
}

bool CMeshExporter::DumpEulerRotKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	IGameKeyTab RotKey;

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_X))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_ROT_X, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_X))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_ROT_X, RotKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Y))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_ROT_Y, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Y))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_ROT_Y, RotKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Z))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_ROT_Z, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Z))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, RotKey[i].t, KFM_ROT_Z, RotKey[i].linearKey.fval);
		}
	}

	return true;
}

bool CMeshExporter::DumpMaxStdScaleKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	IGameKeyTab ScaleKey;
	if (pGameControl->GetBezierKeys(ScaleKey, IGAME_SCALE))
	{
		for (int i = 0; i < ScaleKey.Count(); ++i)
		{
			InsertKeyFrame(KeyFrames, ScaleKey[i].t, KFM_SCALE, ScaleKey[i].bezierKey.sval.s);
		}
	}

	return true;
}

bool CMeshExporter::DumpConstraintKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
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

bool CMeshExporter::DumpListKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl)
{
	int nCount = pGameControl->GetNumOfListSubControls(IGAME_POS);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_POS);
		bool bOK = DumpPositionController(KeyFrames, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_ROT);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_ROT);
		bool bOK = DumpRotationController(KeyFrames, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_SCALE);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_SCALE);
		bool bOK = DumpScaleController(KeyFrames, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool CMeshExporter::DumpSampleKey(TM_KEY_FRAME& KeyFrames, IGameControl* pGameControl, IGameControlType eType)
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
					InsertKeyFrame(KeyFrames, Keys[i].t, KFM_MATRIX, Keys[i].sampleKey.gval);
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

CMeshExporter::KEY_FRAME* CMeshExporter::FindKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time)
{
	KEY_FRAME* pKeyFrame = NULL;

	TM_KEY_FRAME::iterator itfound = KeyFrames.find(time);
	if (itfound != KeyFrames.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEY_FRAME EmptyKeyFrame;
		EmptyKeyFrame.time = time;
		EmptyKeyFrame.nMask = KFM_UNKNOWN;
		EmptyKeyFrame.vScale.x = 1.0f;
		EmptyKeyFrame.vScale.y = 1.0f;
		EmptyKeyFrame.vScale.z = 1.0f;
		KeyFrames.insert(std::make_pair(EmptyKeyFrame.time, EmptyKeyFrame));

		itfound = KeyFrames.find(time);
		assert(itfound != KeyFrames.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

bool CMeshExporter::InsertKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time, KEY_FRAME_MASK eMask, float fValue)
{
	KEY_FRAME* pKeyFrame = FindKeyFrame(KeyFrames, time);
	assert(pKeyFrame);

	if (pKeyFrame->nMask & (eMask | KFM_MATRIX))
	{
		assert(false);
		return false;
	}

	switch (eMask)
	{
	case KFM_POS_X:
		pKeyFrame->vPos.x = fValue;
		break;
	case KFM_POS_Y:
		pKeyFrame->vPos.y = fValue;
		break;
	case KFM_POS_Z:
		pKeyFrame->vPos.z = fValue;
		break;
	case KFM_SCALE_X:
		pKeyFrame->vScale.x = fValue;
		break;
	case KFM_SCALE_Y:
		pKeyFrame->vScale.y = fValue;
		break;
	case KFM_SCALE_Z:
		pKeyFrame->vScale.z = fValue;
		break;
	case KFM_ROT_X:
		if (pKeyFrame->nMask & KFM_QUAT) assert(false);
		pKeyFrame->vRot.x = fValue;
		break;
	case KFM_ROT_Y:
		if (pKeyFrame->nMask & KFM_QUAT) assert(false);
		pKeyFrame->vRot.y = fValue;
		break;
	case KFM_ROT_Z:
		if (pKeyFrame->nMask & KFM_QUAT) assert(false);
		pKeyFrame->vRot.z = fValue;
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	}
	pKeyFrame->nMask |= eMask;

	return true;
}

bool CMeshExporter::InsertKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time, KEY_FRAME_MASK eMask, const Point3& vValue)
{
	KEY_FRAME* pKeyFrame = FindKeyFrame(KeyFrames, time);
	assert(pKeyFrame);

	if (pKeyFrame->nMask & (eMask | KFM_MATRIX))
	{
		assert(false);
		return false;
	}

	switch (eMask)
	{
	case KFM_POS:
		pKeyFrame->vPos.x = vValue.x;
		pKeyFrame->vPos.y = vValue.y;
		pKeyFrame->vPos.z = vValue.z;
		break;
	case KFM_SCALE:
		pKeyFrame->vScale.x = vValue.x;
		pKeyFrame->vScale.y = vValue.y;
		pKeyFrame->vScale.z = vValue.z;
		break;
	case KFM_ROT:
		if (pKeyFrame->nMask & KFM_QUAT) assert(false);
		pKeyFrame->vRot.x = vValue.x;
		pKeyFrame->vRot.y = vValue.y;
		pKeyFrame->vRot.z = vValue.z;
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	}
	pKeyFrame->nMask |= eMask;

	return true;
}

bool CMeshExporter::InsertKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time, KEY_FRAME_MASK eMask, const Quat& qValue)
{
	KEY_FRAME* pKeyFrame = FindKeyFrame(KeyFrames, time);
	assert(pKeyFrame);

	if (pKeyFrame->nMask & (eMask | KFM_MATRIX))
	{
		assert(false);
		return false;
	}

	switch (eMask)
	{
	case KFM_QUAT:
		{
			if (pKeyFrame->nMask & KFM_ROT) assert(false);

			Matrix3 matRot;
			qValue.MakeMatrix(matRot);
			GMatrix matFinal(matRot);

			COEFmtMesh::BONE_TRANSFORM BoneTrans;
			GMatrix2BoneTransform(BoneTrans, matFinal);

			pKeyFrame->rRot.x = BoneTrans.vRotation[0];
			pKeyFrame->rRot.y = BoneTrans.vRotation[1];
			pKeyFrame->rRot.z = BoneTrans.vRotation[2];
			pKeyFrame->rRot.w = BoneTrans.vRotation[3];
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	}
	pKeyFrame->nMask |= eMask;

	return true;
}

bool CMeshExporter::InsertKeyFrame(TM_KEY_FRAME& KeyFrames, TimeValue time, KEY_FRAME_MASK eMask, const GMatrix& matValue)
{
	KEY_FRAME* pKeyFrame = FindKeyFrame(KeyFrames, time);
	assert(pKeyFrame);

	if (pKeyFrame->nMask & (eMask | KFM_POS | KFM_SCALE | KFM_ROT | KFM_QUAT))
	{
		assert(false);
		return false;
	}

	switch (eMask)
	{
	case KFM_MATRIX:
		{
			GMatrix2CMatrix4x4(pKeyFrame->matFull, matValue);
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	}
	pKeyFrame->nMask |= eMask;

	return true;
}

void CMeshExporter::GMatrix2CMatrix4x4(CMatrix4x4& matOut, const GMatrix& matIn)
{
	matOut.m[0] = matIn[0][0];
	matOut.m[1] = matIn[0][1];
	matOut.m[2] = matIn[0][2];
	matOut.m[3] = matIn[0][3];

	matOut.m[4] = matIn[1][0];
	matOut.m[5] = matIn[1][1];
	matOut.m[6] = matIn[1][2];
	matOut.m[7] = matIn[1][3];

	matOut.m[8] = matIn[2][0];
	matOut.m[9] = matIn[2][1];
	matOut.m[10] = matIn[2][2];
	matOut.m[11] = matIn[2][3];

	matOut.m[12] = matIn[3][0];
	matOut.m[13] = matIn[3][1];
	matOut.m[14] = matIn[3][2];
	matOut.m[15] = matIn[3][3];
}

void CMeshExporter::CMatrix4x42GMatrix(GMatrix& matOut, const CMatrix4x4& matIn)
{
	matOut[0][0] = matIn.m[0];
	matOut[0][1] = matIn.m[1];
	matOut[0][2] = matIn.m[2];
	matOut[0][3] = matIn.m[3];

	matOut[1][0] = matIn.m[4];
	matOut[1][1] = matIn.m[5];
	matOut[1][2] = matIn.m[6];
	matOut[1][3] = matIn.m[7];

	matOut[2][0] = matIn.m[8];
	matOut[2][1] = matIn.m[9];
	matOut[2][2] = matIn.m[10];
	matOut[2][3] = matIn.m[11];

	matOut[3][0] = matIn.m[12];
	matOut[3][1] = matIn.m[13];
	matOut[3][2] = matIn.m[14];
	matOut[3][3] = matIn.m[15];
}

void CMeshExporter::GMatrix2BoneTransform(COEFmtMesh::BONE_TRANSFORM& BoneTrans, const GMatrix& matTrans)
{
	CMatrix4x4 matRot;
	GMatrix2CMatrix4x4(matRot, matTrans);

	CQuaternion rRot;
	COEMath::BuildQuaternionFromMatrix(rRot, matRot);

	Point3 vScale = matTrans.Scaling();

	BoneTrans.vPos[0] = matTrans[3][0];
	BoneTrans.vPos[1] = matTrans[3][1];
	BoneTrans.vPos[2] = matTrans[3][2];

	BoneTrans.vScale[0] = vScale.x;
	BoneTrans.vScale[1] = vScale.y;
	BoneTrans.vScale[2] = vScale.z;

	BoneTrans.vRotation[0] = rRot.x;
	BoneTrans.vRotation[1] = rRot.y;
	BoneTrans.vRotation[2] = rRot.z;
	BoneTrans.vRotation[3] = rRot.w;
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
