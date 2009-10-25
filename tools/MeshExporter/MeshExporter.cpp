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
		BoneInfo.nNodeID = NodeInfo.pGameNode->GetNodeID();
		BoneInfo.nParentNodeID = 0;
		if (NodeInfo.pParentGameNode) BoneInfo.nParentNodeID = NodeInfo.pParentGameNode->GetNodeID();
		BoneInfo.nID = (int)m_vBoneInfo.size();
		BoneInfo.nParentID = COEFmtMesh::INVALID_BONE_ID;
		BoneInfo.matLocal = NodeInfo.pGameNode->GetLocalTM();
		bool bOK = DumpController(BoneInfo.vFrameInfo, NodeInfo.pGameNode);
		// TODO: check bOK
		m_vBoneInfo.push_back(BoneInfo);
		m_vBoneInfoMap.insert(std::make_pair(BoneInfo.nNodeID, BoneInfo.nID));
	}

	for (TV_BONE_INFO::iterator it = m_vBoneInfo.begin(); it != m_vBoneInfo.end(); ++it)
	{
		BONE_INFO& BoneInfo = (*it);
		TM_BONE_INFO::iterator itfound = m_vBoneInfoMap.find(BoneInfo.nParentNodeID);
		if (itfound != m_vBoneInfoMap.end())
		{
			BoneInfo.nParentID = itfound->second;
		}
		else
		{
			// TODO: 
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
		vPiece[i].nVertexDataMask = COEFmtMesh::VDM_XYZ | COEFmtMesh::VDM_UV | COEFmtMesh::VDM_BONE;
		vPiece[i].nMaterialID = 0;										// TODO: 

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
	if (pGameObject->GetIGameType() != IGameObject::IGAME_MESH) return false;

	IGameMesh* pGameMesh = (IGameMesh*)pGameObject;
	pGameMesh->SetCreateOptimizedNormalList();
	if (!pGameMesh->InitializeData()) return false;

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
	int nNewNumVerts = SkinMeshOut.vVertSlots.size();
	for (int i = 0; i < nNewNumVerts; ++i)
	{
		VERTEX_SLOT& LocalSlot = SkinMeshOut.vVertSlots[i];

		pGameMesh->GetVertex(LocalSlot.nVertIndex, LocalSlot.pos, false);
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
			int nNodeID = pGameSkin->GetBoneID(i, j);
			TM_BONE_INFO::iterator it = m_vBoneInfoMap.find(nNodeID);
			if (it == m_vBoneInfoMap.end()) continue;

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

	TS_TIME_VALUE TimeValueSet;

	// export position controller
	if (pGameControl->IsAnimated(IGAME_POS)) DumpPositionController(TimeValueSet, pGameControl);

	// export rotation controller
	if (pGameControl->IsAnimated(IGAME_ROT)) DumpRotationController(TimeValueSet, pGameControl);

	// export scale controller
	if (pGameControl->IsAnimated(IGAME_SCALE)) DumpScaleController(TimeValueSet, pGameControl);

	// 
	IGameKeyTab TMKey;
	pGameControl->GetFullSampledKeys(TMKey, 1, IGAME_TM, true);

	TS_TIME_VALUE::const_iterator it = TimeValueSet.begin();
	int nCount = TMKey.Count();

	for (int i = 0; i < nCount; ++i)
	{
		if (it == TimeValueSet.end()) break;
		if (TMKey[i].t < (*it)) continue;

		FRAME_INFO FrameInfo;
		FrameInfo.Time = TMKey[i].t;
		FrameInfo.matAnim = TMKey[i].sampleKey.gval;
		vFrameInfoOut.push_back(FrameInfo);

		++it;
	}

	return true;
}

bool CMeshExporter::DumpPositionController(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_POS);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std pos key
			DumpMaxStdPosKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_BIPED:
		{
			// export biped pos key
			DumpBipedPosKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_POS_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_INDEPENDENT_POS:
		{
			// export independent controller
			DumpIndependentPosKey(TimeSetOut, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::DumpRotationController(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_ROT);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std rot key
			DumpMaxStdRotKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_BIPED:
		{
			// export biped rot key
			DumpBipedRotKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_EULER:
		{
			// export Euler controller
			DumpEulerRotKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_ROT_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(TimeSetOut, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::DumpScaleController(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_SCALE);

	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export scale key
			DumpMaxStdScaleKey(TimeSetOut, pGameControl);
		}
		break;
	case IGameControl::IGAME_BIPED:
		{
			// export scale key
			DumpBipedScaleKey(TimeSetOut, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			return false;
		}
		break;
	}

	return true;
}

bool CMeshExporter::DumpMaxStdPosKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(PosKey[i].t);
			// time = PosKey[i].t;
			// position = PosKey[i].bezierKey.pval;
		}
	}
	else if (pGameControl->GetLinearKeys(PosKey, IGAME_POS))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(PosKey[i].t);
			// time = PosKey[i].t;
			// position = PosKey[i].linearKey.pval;
		}
	}

	return true;
}

bool CMeshExporter::DumpBipedPosKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	Control* pControl = pGameControl->GetMaxControl(IGAME_POS);

	int nNumKey = pControl->NumKeys();
	for (int i = 0; i < nNumKey; ++i)
	{
		TimeValue KeyTime = pControl->GetKeyTime(i);
		TimeSetOut.insert(KeyTime);
	}

	return true;
}

bool CMeshExporter::DumpIndependentPosKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_X))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(PosKey[i].t);
			// time = PosKey[i].t;
			// position = PosKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_X))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(PosKey[i].t);
			// time = PosKey[i].t;
			// position = PosKey[i].linearKey.fval;
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Y))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(PosKey[i].t);
			// time = PosKey[i].t;
			// position = PosKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Y))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(PosKey[i].t);
			// time = PosKey[i].t;
			// position = PosKey[i].linearKey.fval;
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Z))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(PosKey[i].t);
			// time = PosKey[i].t;
			// position = PosKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Z))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(PosKey[i].t);
			// time = PosKey[i].t;
			// position = PosKey[i].linearKey.fval;
		}
	}

	return true;
}

bool CMeshExporter::DumpMaxStdRotKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	IGameKeyTab RotKey;
	if (pGameControl->GetBezierKeys(RotKey, IGAME_ROT))
	{
		// export Bezier Keys
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// Quat = RotKey[i].bezierKey.qval;
		}
	}
	else if (pGameControl->GetLinearKeys(RotKey, IGAME_ROT))
	{
		// export Linear Keys
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// Quat = RotKey[i].linearKey.qval;
		}
	}
	else if (pGameControl->GetTCBKeys(RotKey, IGAME_ROT))
	{
		// export TCB Keys
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// AngAxis = RotKey[i].tcbKey.aval;
		}
	}

	return true;
}

bool CMeshExporter::DumpBipedRotKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	Control* pControl = pGameControl->GetMaxControl(IGAME_ROT);

	int nNumKey = pControl->NumKeys();
	for (int i = 0; i < nNumKey; ++i)
	{
		TimeValue KeyTime = pControl->GetKeyTime(i);
		TimeSetOut.insert(KeyTime);
	}

	return true;
}

bool CMeshExporter::DumpEulerRotKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	IGameKeyTab RotKey;

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_X))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// RotKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_X))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// RotKey[i].linearKey.fval;
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Y))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// RotKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Y))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// RotKey[i].linearKey.fval;
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Z))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// RotKey[i].bezierKey.fval;
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Z))
	{
		int nCount = RotKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(RotKey[i].t);
			// time = RotKey[i].t;
			// RotKey[i].linearKey.fval;
		}
	}

	return true;
}

bool CMeshExporter::DumpMaxStdScaleKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	IGameKeyTab ScaleKey;
	if (pGameControl->GetBezierKeys(ScaleKey, IGAME_SCALE))
	{
		int nCount = ScaleKey.Count();

		for (int i = 0; i < nCount; ++i)
		{
			TimeSetOut.insert(ScaleKey[i].t);
			// time = ScaleKey[i].t;
			// ScaleKey[i].bezierKey.sval;
		}
	}

	return true;
}

bool CMeshExporter::DumpBipedScaleKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	Control* pControl = pGameControl->GetMaxControl(IGAME_SCALE);

	int nNumKey = pControl->NumKeys();
	for (int i = 0; i < nNumKey; ++i)
	{
		TimeValue KeyTime = pControl->GetKeyTime(i);
		TimeSetOut.insert(KeyTime);
	}

	return true;
}

bool CMeshExporter::DumpConstraintKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
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

bool CMeshExporter::DumpListKey(TS_TIME_VALUE& TimeSetOut, IGameControl* pGameControl)
{
	int nCount = pGameControl->GetNumOfListSubControls(IGAME_POS);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_POS);
		bool bOK = DumpPositionController(TimeSetOut, pSubGameControl);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_ROT);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_ROT);
		bool bOK = DumpRotationController(TimeSetOut, pSubGameControl);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_SCALE);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_SCALE);
		bool bOK = DumpScaleController(TimeSetOut, pSubGameControl);
		// TODO: check bOK
	}

	return true;
}

void CMeshExporter::GMatrix2BoneTransform(COEFmtMesh::BONE_TRANSFORM& BoneTrans, const GMatrix& matTrans)
{
	CMatrix4x4 matRot;
	matRot.m[0] = matTrans[0][0];
	matRot.m[1] = matTrans[0][1];
	matRot.m[2] = matTrans[0][2];
	matRot.m[3] = matTrans[0][3];

	matRot.m[4] = matTrans[1][0];
	matRot.m[5] = matTrans[1][1];
	matRot.m[6] = matTrans[1][2];
	matRot.m[7] = matTrans[1][3];

	matRot.m[8] = matTrans[2][0];
	matRot.m[9] = matTrans[2][1];
	matRot.m[10] = matTrans[2][2];
	matRot.m[11] = matTrans[2][3];

	matRot.m[12] = matTrans[3][0];
	matRot.m[13] = matTrans[3][1];
	matRot.m[14] = matTrans[3][2];
	matRot.m[15] = matTrans[3][3];

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
