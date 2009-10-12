/*!
 * \file MeshExporter.cpp
 * \date 1-8-2009 10:36:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshExporter.h"
#include <OEOS.h>
#include <OEFmtMesh.h>

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
	SaveToFile(name);

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
	for (VNODE_INFO::iterator it = m_vMeshNode.begin(); it != m_vMeshNode.end(); ++it)
	{
		NODE_INFO& NodeInfo = (*it);

		SKIN_MESH SkinMesh;
		SkinMesh.strName = NodeInfo.pGameNode->GetName();
		SkinMesh.matLocal = NodeInfo.pGameNode->GetLocalTM();
		bool bOK = DumpSkinMesh(SkinMesh, NodeInfo.pGameNode);
		// TODO: check bOK
		m_vSkinMesh.push_back(SkinMesh);
	}

	return true;
}

bool CMeshExporter::BuildBonesInfo()
{
	for (VNODE_INFO::iterator it = m_vBoneNode.begin(); it != m_vBoneNode.end(); ++it)
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

	for (VBONE_INFO::iterator it = m_vBoneInfo.begin(); it != m_vBoneInfo.end(); ++it)
	{
		BONE_INFO& BoneInfo = (*it);
		BONE_INFO_MAP::iterator itfound = m_vBoneInfoMap.find(BoneInfo.nParentNodeID);
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

	int nNumMesh = (int)m_vSkinMesh.size();
	int nNumBone = (int)m_vBoneInfo.size();

	uint nSizeOfHeader = sizeof(COEFmtMesh::FILE_HEADER);
	uint nSizeOfMeshs = sizeof(COEFmtMesh::PIECE)*nNumMesh;
	uint nSizeOfBones = sizeof(COEFmtMesh::BONE)*nNumBone;

	uint nCurrPos = nSizeOfHeader+nSizeOfMeshs+nSizeOfBones;

	// calculate verts offset
	VFILE_BLOCK vVertsBlock;
	uint nElementSize = sizeof(float)*3+sizeof(int)*4+sizeof(float)*4;
	for (int i = 0; i < nNumMesh; ++i)
	{
		FILE_BLOCK FileBlock;
		FileBlock.nOffset = nCurrPos;
		FileBlock.nSize = nElementSize*(uint)m_vSkinMesh[i].vVerts.size();
		vVertsBlock.push_back(FileBlock);

		nCurrPos += FileBlock.nSize;
	}

	// calculate indis offset
	VFILE_BLOCK vIndisBlock;
	nElementSize = sizeof(ushort)*3;
	for (int i = 0; i < nNumMesh; ++i)
	{
		FILE_BLOCK FileBlock;
		FileBlock.nOffset = nCurrPos;
		FileBlock.nSize = nElementSize*(uint)m_vSkinMesh[i].vFaces.size();
		vIndisBlock.push_back(FileBlock);

		nCurrPos += FileBlock.nSize;
	}

	// calculate bones offset
	VFILE_BLOCK vBoneBlock;
	nElementSize = sizeof(float)+sizeof(float)*16;
	for (int i = 0; i < nNumBone; ++i)
	{
		FILE_BLOCK FileBlock;
		FileBlock.nOffset = nCurrPos;
		FileBlock.nSize = nElementSize*(uint)m_vBoneInfo[i].vFrameInfo.size();
		vBoneBlock.push_back(FileBlock);

		nCurrPos += FileBlock.nSize;
	}

	// write header
	COEFmtMesh::FILE_HEADER Header;
	Header.nMagicNumber = COEFmtMesh::MAGIC_NUMBER;
	Header.nVersion = COEFmtMesh::CURRENT_VERSION;
	Header.nNumPieces = nNumMesh;
	Header.nNumBones = nNumBone;
	pFile->Write(&Header, sizeof(Header));

	// write piece list
	for (int i = 0; i < nNumMesh; ++i)
	{
		COEFmtMesh::PIECE Piece;

		std::string strName;
		COEOS::tchar2char(strName, m_vSkinMesh[i].strName.c_str());
		strncpy_s(Piece.szName, strName.c_str(), COEFmtMesh::PIECE_NAME_SIZE);

		Piece.nPieceMask = COEFmtMesh::PM_VISIBLE;
		Piece.nVertexDataMask = COEFmtMesh::VDM_XYZ | COEFmtMesh::VDM_BONE;
		Piece.nMaterialID = 0;														// TODO: 
		Piece.nNumVerts = (int)m_vSkinMesh[i].vVerts.size();
		Piece.nOffVerts = vVertsBlock[i].nOffset;
		Piece.nNumIndis = (int)m_vSkinMesh[i].vFaces.size()*3;
		Piece.nOffIndis = vIndisBlock[i].nOffset;

		pFile->Write(&Piece, sizeof(Piece));
	}

	// write bone list
	for (int i = 0; i < nNumBone; ++i)
	{
		COEFmtMesh::BONE Bone;

		std::string strName;
		COEOS::tchar2char(strName, m_vBoneInfo[i].strName.c_str());
		strncpy_s(Bone.szName, strName.c_str(), COEFmtMesh::BONE_NAME_SIZE);

		Bone.nParentIndex = m_vBoneInfo[i].nParentID;

		if (!m_vBoneInfo[i].vFrameInfo.empty())
		{
			TimeValue TimeTick = m_vBoneInfo[i].vFrameInfo.rbegin()->Time;
			Bone.fTimeLength = TicksToSec(TimeTick);
		}
		else
		{
			Bone.fTimeLength = 0.0f;
		}

		memcpy(Bone.matLocal, m_vBoneInfo[i].matLocal.GetAddr(), sizeof(float)*16);

		Bone.nNumBoneFrame = (int)m_vBoneInfo[i].vFrameInfo.size();
		Bone.nOffBoneFrame = vBoneBlock[i].nOffset;
		pFile->Write(&Bone, sizeof(Bone));
	}

	// write vertex data
	for (int i = 0; i < nNumMesh; ++i)
	{
		int nNumVerts = (int)m_vSkinMesh[i].vVerts.size();
		for (int j = 0; j < nNumVerts; ++j)
		{
			VERTEX& Vertex = m_vSkinMesh[i].vVerts[j];
			float fPos[3];
			fPos[0] = Vertex.pos.x;
			fPos[1] = Vertex.pos.y;
			fPos[2] = Vertex.pos.z;
			pFile->Write(fPos, sizeof(fPos));

			// TODO: add more data

			int arrBoneID[4] = {0};
			float arrWeight[4] = {0.0f};

			int nSkinCount = (int)Vertex.Skins.size();
			if (nSkinCount > 4) nSkinCount = 4;
			for (int k = 0; k < nSkinCount; ++k)
			{
				arrBoneID[k] = Vertex.Skins[k].nBoneIndex;
				arrWeight[k] = Vertex.Skins[k].fWeight;
			}
			pFile->Write(arrBoneID, sizeof(int)*4);
			pFile->Write(arrWeight, sizeof(float)*4);
		}
	}

	// write index data
	for (int i = 0; i < nNumMesh; ++i)
	{
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
	for (int i = 0; i < nNumBone; ++i)
	{
		int nNumFrame = (int)m_vBoneInfo[i].vFrameInfo.size();
		for (int j = 0; j < nNumFrame; ++j)
		{
			FRAME_INFO& FrameInfo = m_vBoneInfo[i].vFrameInfo[j];
			float fTime = TicksToSec(FrameInfo.Time);
			pFile->Write(&fTime, sizeof(float));
			pFile->Write(FrameInfo.matAnim.GetAddr(), sizeof(float)*16);
		}
	}

	SAFE_RELEASE(pFile);
	return true;
}

bool CMeshExporter::DumpSkinMesh(SKIN_MESH& SkinMeshOut, IGameNode* pGameNode)
{
	IGameObject* pGameObject = pGameNode->GetIGameObject();
	if (pGameObject->GetIGameType() != IGameObject::IGAME_MESH) return false;

	IGameMesh* pGameMesh = (IGameMesh*)pGameObject;
	pGameMesh->SetCreateOptimizedNormalList();
	if (!pGameMesh->InitializeData()) return false;

	// vertices
	int nNumVerts = pGameMesh->GetNumberOfVerts();
	for(int i = 0; i < nNumVerts; ++i)
	{
		VERTEX Vertex;
		pGameMesh->GetVertex(i, Vertex.pos, true);
		SkinMeshOut.vVerts.push_back(Vertex);
	}

	// faces
	int nNumFaces = pGameMesh->GetNumberOfFaces();
	for (int i = 0; i < nNumFaces; ++i)
	{
		FaceEx* pFace = pGameMesh->GetFace(i);
		FACE Face;
		Face.nVertIndex[0] = pFace->vert[0];
		Face.nVertIndex[1] = pFace->vert[1];
		Face.nVertIndex[2] = pFace->vert[2];
		SkinMeshOut.vFaces.push_back(Face);
	}

	// skins
	int nNumModifier = pGameObject->GetNumModifiers();
	for (int i = 0; i < nNumModifier; ++i)
	{
		IGameModifier* pGameModifier = pGameObject->GetIGameModifier(i);
		if (!pGameModifier->IsSkin()) continue;
		IGameSkin* pGameSkin = (IGameSkin*)pGameModifier;

		bool bOK = DumpSkinVerts(SkinMeshOut, pGameSkin);
		// TODO: check bOK
	}

	return true;
}

bool CMeshExporter::DumpSkinVerts(SKIN_MESH& SkinMeshOut, IGameSkin* pGameSkin)
{
	int nNumSkinVerts = pGameSkin->GetNumOfSkinnedVerts();
	assert(nNumSkinVerts == SkinMeshOut.vVerts.size());

	for (int i = 0; i < nNumSkinVerts; ++i)
	{
		int nNumBone = pGameSkin->GetNumberOfBones(i);
		for (int j = 0; j < nNumBone; ++j)
		{
			int nNodeID = pGameSkin->GetBoneID(i, j);
			BONE_INFO_MAP::iterator it = m_vBoneInfoMap.find(nNodeID);
			if (it == m_vBoneInfoMap.end()) continue;

			SKIN Skin;
			Skin.nBoneIndex = it->second;
			Skin.fWeight = pGameSkin->GetWeight(i, j);
			SkinMeshOut.vVerts[i].Skins.push_back(Skin);
		}
	}

	return true;
}

bool CMeshExporter::DumpController(VFRAME_INFO& vFrameInfoOut, IGameNode* pGameNode)
{
	IGameControl* pGameControl = pGameNode->GetIGameControl();

	TIME_VALUE_SET TimeValueSet;

	// export position controller
	if (pGameControl->IsAnimated(IGAME_POS)) DumpPositionController(TimeValueSet, pGameControl);

	// export rotation controller
	if (pGameControl->IsAnimated(IGAME_ROT)) DumpRotationController(TimeValueSet, pGameControl);

	// export scale controller
	if (pGameControl->IsAnimated(IGAME_SCALE)) DumpScaleController(TimeValueSet, pGameControl);

	// 
	IGameKeyTab TMKey;
	pGameControl->GetFullSampledKeys(TMKey, 1, IGAME_TM, true);

	TIME_VALUE_SET::const_iterator it = TimeValueSet.begin();
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

bool CMeshExporter::DumpPositionController(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpRotationController(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpScaleController(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpMaxStdPosKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpBipedPosKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpIndependentPosKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpMaxStdRotKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpBipedRotKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpEulerRotKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpMaxStdScaleKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpBipedScaleKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpConstraintKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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

bool CMeshExporter::DumpListKey(TIME_VALUE_SET& TimeSetOut, IGameControl* pGameControl)
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
