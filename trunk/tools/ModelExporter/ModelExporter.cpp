/*!
 * \file ModelExporter.cpp
 * \date 1-3-2010 21:00:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ModelExporter.h"
#include "DlgModelExporterOption.h"

#include <libOEBase/OEOS.h>
#include <IPathConfigMgr.h>

DWORD WINAPI DummyFunc(LPVOID arg)
{
	return 0;
}

CModelExporter::CModelExporter()
{
	Init();
}

CModelExporter::~CModelExporter()
{
	Destroy();
}

void CModelExporter::Init()
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

	m_pParentWindow = NULL;
	m_pDlgModelExpOption = NULL;
}

void CModelExporter::Destroy()
{
	wxDELETE(m_pDlgModelExpOption);

	if (m_pParentWindow)
	{
		m_pParentWindow->SetHWND(NULL);
		wxDELETE(m_pParentWindow);
	}
}

int CModelExporter::ExtCount()
{
	return 1;
}

const TCHAR* CModelExporter::Ext(int n)
{
	return _T("mesh");
}

const TCHAR* CModelExporter::LongDesc()
{
	return _T("Export to origin engine model file format");
}

const TCHAR* CModelExporter::ShortDesc()
{
	return _T("Origin Engine Model");
}

const TCHAR* CModelExporter::AuthorName()
{
	return _T("zjhlogo");
}

const TCHAR* CModelExporter::CopyrightMessage()
{
	return _T("Copyright (c) 2009, zjhlogo All rights reserved");
}

const TCHAR* CModelExporter::OtherMessage1()
{
	return _T("Other message1");
}

const TCHAR* CModelExporter::OtherMessage2()
{
	return _T("Other message2");
}

unsigned int CModelExporter::Version()
{
	return CURRENT_VERSION;
}

void CModelExporter::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, _T("Origin Engine Model Exporter"), _T("Copyright (c) 2009, zjhlogo All rights reserved"), MB_OK);
}

int CModelExporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts /* = FALSE */, DWORD options /* = 0 */)
{
	Cleanup();
	m_pInterface = i;

	// show the option dialog
	if (!ShowOptionDialog()) return TRUE;

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

	// build material info
	m_pInterface->ProgressStart(_T("Build material information"), TRUE, DummyFunc, NULL);
	BuildMaterialsInfo();

	// save to file
	tstring strFile;
	COEOS::GetFileNameWithoutExt(strFile, name);
	if (m_pDlgModelExpOption->IsExportMesh()) SaveMeshFile(strFile + TS(".mesh"));
	if (m_pDlgModelExpOption->IsExportSkelecton()) SaveSkeletonFile(strFile + TS(".skel"));
	SaveMaterialsFile(strFile + TS(".xml"), strFile + TS(".mesh"), strFile + TS(".skel"));

	pIGame->ReleaseIGame();
	pIGame = NULL;

	m_pInterface->ProgressEnd();
	Cleanup();

	return TRUE;
}

void CModelExporter::Cleanup()
{
	m_vMeshNode.clear();
	m_vBoneNode.clear();
	m_vMeshData.clear();
	m_vBoneInfo.clear();
	m_vBoneInfoMap.clear();
	m_vMaterial.clear();
	m_vBoundingBoxMin.Init(COEMath::FLOAT_MAX, COEMath::FLOAT_MAX, COEMath::FLOAT_MAX);
	m_vBoundingBoxMax.Init(COEMath::FLOAT_MIN, COEMath::FLOAT_MIN, COEMath::FLOAT_MIN);
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

bool CModelExporter::CollectNodes(IGameNode* pGameNode, IGameNode* pParentGameNode /* = NULL */)
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

bool CModelExporter::BuildBonesInfo()
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
		BoneInfo.nParentID = COEFmtSkeleton::INVALID_BONE_ID;
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

bool CModelExporter::BuildMeshsInfo()
{
	int nMeshCount = m_vMeshNode.size();
	for (int i = 0; i < nMeshCount; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/nMeshCount);

		NODE_INFO& NodeInfo = m_vMeshNode[i];
		MESH_DATA MeshData;
		MeshData.strName = NodeInfo.pGameNode->GetName();
		MeshData.matLocal = NodeInfo.pGameNode->GetLocalTM();
		MeshData.pMaterial = NULL;
		MeshData.nMaterialID = -1;

		bool bOK = DumpMesh(MeshData, NodeInfo.pGameNode);
		assert(bOK);
		// TODO: check bOK
		m_vMeshData.push_back(MeshData);
	}

	return true;
}

bool CModelExporter::BuildMaterialsInfo()
{
	for (TV_MESH_DATA::iterator it = m_vMeshData.begin(); it != m_vMeshData.end(); ++it)
	{
		MESH_DATA& MeshData = (*it);
		IGameMaterial* pMaterial = MeshData.pMaterial;
		if (!pMaterial) continue;
		if (FindMaterial(pMaterial)) continue;

		MATERIAL Material;
		Material.pMaterial = pMaterial;
		Material.nID = m_vMaterial.size();
		int nNumTexture = pMaterial->GetNumberOfTextureMaps();
		for (int i = 0; i < nNumTexture; ++i)
		{
			IGameTextureMap* pMap = pMaterial->GetIGameTextureMap(i);
			tstring strMapFile = pMap->GetBitmapFileName();
			Material.vTextureMap.push_back(strMapFile);
		}

		m_vMaterial.push_back(Material);
		MeshData.nMaterialID = Material.nID;
	}

	return true;
}

bool CModelExporter::FindMaterial(IGameMaterial* pMaterial)
{
	for (TV_MATERIAL::iterator it = m_vMaterial.begin(); it != m_vMaterial.end(); ++it)
	{
		MATERIAL& Material = (*it);
		if (pMaterial == Material.pMaterial) return true;
	}

	return false;
}

bool CModelExporter::SaveMeshFile(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	// write header
	COEFmtMesh::FILE_HEADER Header;
	Header.nMagicNumber = COEFmtMesh::MAGIC_NUMBER;
	Header.nVersion = COEFmtMesh::CURRENT_VERSION;

	// 左右手坐标转化
	Header.fBoundingBoxMin[0] = m_vBoundingBoxMin.x;
	Header.fBoundingBoxMin[1] = m_vBoundingBoxMin.z;
	Header.fBoundingBoxMin[2] = m_vBoundingBoxMin.y;
	Header.fBoundingBoxMax[0] = m_vBoundingBoxMax.x;
	Header.fBoundingBoxMax[1] = m_vBoundingBoxMax.z;
	Header.fBoundingBoxMax[2] = m_vBoundingBoxMax.y;

	Header.nNumPieces = (int)m_vMeshData.size();
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
		MESH_DATA& MeshData = m_vMeshData[i];

		std::string strName;
		COEOS::tchar2char(strName, MeshData.strName.c_str());
		strncpy_s(Piece.szName, COEFmtMesh::PIECE_NAME_SIZE, strName.c_str(), _TRUNCATE);

		Piece.nPieceMask = COEFmtMesh::PM_VISIBLE;
		Piece.nVertexDataMask = m_pDlgModelExpOption->GetVertexFlag();
		Piece.nMaterialID = MeshData.nMaterialID;

		// write vertex data offset
		Piece.nNumVerts = (int)MeshData.vVertSlots.size();
		Piece.nOffVerts = pFile->Tell();

		// write vertex data
		int nNumVerts = (int)MeshData.vVertSlots.size();
		for (int j = 0; j < nNumVerts; ++j)
		{
			VERTEX_SLOT& VertSlot = MeshData.vVertSlots[j];

			if (Piece.nVertexDataMask & COEFmtMesh::VDM_POSITION)
			{
				// position 左右手坐标转化
				float pfPos[3] = {VertSlot.pos.x, VertSlot.pos.z, VertSlot.pos.y};
				pFile->Write(pfPos, sizeof(pfPos));
			}

			if (Piece.nVertexDataMask & COEFmtMesh::VDM_TEXCOORD0)
			{
				// uv
				float pfUV[2] = {VertSlot.tex.x, 1.0f - VertSlot.tex.y};
				pFile->Write(pfUV, sizeof(pfUV));
			}

			if (Piece.nVertexDataMask & COEFmtMesh::VDM_NORMAL)
			{
				// normal 左右手坐标转化
				float pfNormal[3] = {VertSlot.normal.x, VertSlot.normal.z, VertSlot.normal.y};
				pFile->Write(pfNormal, sizeof(pfNormal));
			}

			if (Piece.nVertexDataMask & COEFmtMesh::VDM_TANGENT)
			{
				// tangent 左右手坐标转化
				float pfTangent[3] = {VertSlot.tangent.x, VertSlot.tangent.z, VertSlot.tangent.y};
				pFile->Write(pfTangent, sizeof(pfTangent));
			}

			if (Piece.nVertexDataMask & COEFmtMesh::VDM_SKELECTON)
			{
				uchar uchIndex[4] = {0};
				float pfWeight[4] = {0.0f};

				// bone weight, index
				int nSkinCount = (int)VertSlot.Skins.size();
				if (nSkinCount > 4) nSkinCount = 4;
				for (int k = 0; k < nSkinCount; ++k)
				{
					uchIndex[k] = VertSlot.Skins[k].nBoneIndex;
					pfWeight[k] = VertSlot.Skins[k].fWeight;
				}

				pFile->Write(uchIndex, sizeof(uchIndex));
				pFile->Write(pfWeight, sizeof(pfWeight));
			}
		}

		// write index data offset
		Piece.nNumIndis = (int)MeshData.vFaces.size()*3;
		Piece.nOffIndis = pFile->Tell();

		// write index data
		int nNumFaces = (int)MeshData.vFaces.size();
		for (int j = 0; j < nNumFaces; ++j)
		{
			//  左右手坐标转化
			ushort nIndex[3];
			nIndex[0] = (ushort)MeshData.vFaces[j].nVertIndex[0];
			nIndex[1] = (ushort)MeshData.vFaces[j].nVertIndex[2];
			nIndex[2] = (ushort)MeshData.vFaces[j].nVertIndex[1];
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

bool CModelExporter::SaveSkeletonFile(const tstring& strFileName)
{
	IOEFile* pFile = g_pOEFileMgr->OpenFile(strFileName, IOEFile::OFF_WRITE);
	if (!pFile) return false;

	// write header
	COEFmtSkeleton::FILE_HEADER Header;
	Header.nMagicNumber = COEFmtSkeleton::MAGIC_NUMBER;
	Header.nVersion = COEFmtSkeleton::CURRENT_VERSION;
	Header.nNumBones = (int)m_vBoneInfo.size();
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

	// write bone data
	m_pInterface->ProgressStart(_T("Save bone data"), TRUE, DummyFunc, NULL);
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/Header.nNumBones);

		COEFmtSkeleton::BONE& Bone = vBone[i];
		BONE_INFO& BoneInfo = m_vBoneInfo[i];

		// bone list info
		std::string strName;
		COEOS::tchar2char(strName, BoneInfo.strName.c_str());
		strncpy_s(Bone.szName, COEFmtSkeleton::BONE_NAME_SIZE, strName.c_str(), _TRUNCATE);
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

			COEFmtSkeleton::FRAME_ROT Frame;
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

			COEFmtSkeleton::FRAME_POS Frame;
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

			COEFmtSkeleton::FRAME_SCALE Frame;
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
		pFile->Write(&vBone[0], sizeof(COEFmtSkeleton::BONE)*Header.nNumBones);
	}

	SAFE_RELEASE(pFile);
	return true;
}

bool CModelExporter::SaveMaterialsFile(const tstring& strFileName, const tstring& strMeshFile, const tstring& strSkeletonFile)
{
	IOEXmlDocument* pXmlDocument = g_pOEXmlMgr->CreateDocument();
	if (!pXmlDocument) return false;

	IOEXmlNode* pXmlModel = pXmlDocument->InsertRootNode(TS("Model"));
	IOEXmlNode* pXmlRenderData = pXmlModel->InsertChild(TS("RenderData"));

	if (m_pDlgModelExpOption->IsExportMesh())
	{
		IOEXmlNode* pXmlMesh = pXmlRenderData->InsertChild(TS("Mesh"));
		tstring strName;
		COEOS::GetFileNameWithExt(strName, strMeshFile);
		pXmlMesh->SetText(strName);
	}

	if (m_pDlgModelExpOption->IsExportSkelecton())
	{
		IOEXmlNode* pXmlSkeleton = pXmlRenderData->InsertChild(TS("Skeleton"));
		tstring strName;
		COEOS::GetFileNameWithExt(strName, strSkeletonFile);
		pXmlSkeleton->SetText(strName);
	}

	IOEXmlNode* pXmlMaterials = pXmlRenderData->InsertChild(TS("Materials"));
	pXmlMaterials->SetAttribute(TS("count"), (int)m_vMaterial.size());
	for (TV_MATERIAL::iterator it = m_vMaterial.begin(); it != m_vMaterial.end(); ++it)
	{
		MATERIAL& Material = (*it);
		IOEXmlNode* pXmlMaterial = pXmlMaterials->InsertChild(TS("Material"));
		pXmlMaterial->SetAttribute(TS("id"), Material.nID);
		pXmlMaterial->SetAttribute(TS("vertdecl"), m_pDlgModelExpOption->GetVertexFlag());
		pXmlMaterial->SetAttribute(TS("shader"), EMPTY_STRING);

		int nTexIndex = 0;
		for (TV_STRING::iterator ittex = Material.vTextureMap.begin(); ittex != Material.vTextureMap.end(); ++ittex)
		{
			tstring& strMap = (*ittex);
			tstring strName;
			COEOS::GetFileNameWithExt(strName, strMap);

			tstring strKey;
			COEOS::strformat(strKey, TS("texture%d"), nTexIndex);
			pXmlMaterial->SetAttribute(strKey, strName);
			++nTexIndex;
		}
	}

	pXmlDocument->SaveFile(strFileName);
	SAFE_RELEASE(pXmlDocument);

	return true;
}

bool CModelExporter::DumpMesh(MESH_DATA& MeshDataOut, IGameNode* pGameNode)
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
		MeshDataOut.vVertSlots.push_back(EmptySlot);
	}

	// faces
	int nNumFaces = pGameMesh->GetNumberOfFaces();
	if (nNumFaces > 0)
	{
		FaceEx* pFace = pGameMesh->GetFace(0);
		MeshDataOut.pMaterial = pGameMesh->GetMaterialFromFace(pFace);
	}

	for (int i = 0; i < nNumFaces; ++i)
	{
		FaceEx* pFace = pGameMesh->GetFace(i);

		FACE Face;

		for (int j = 0; j < 3; ++j)
		{
			uint nVertIndex = pFace->vert[j];
			uint nTexIndex = pFace->texCoord[j];

			if (MeshDataOut.vVertSlots[nVertIndex].bUsed)
			{
				bool bAddNew = true;
				if (MeshDataOut.vVertSlots[nVertIndex].nTexIndex == nTexIndex)
				{
					bAddNew = false;
				}
				else
				{
					TV_INT& vClone = MeshDataOut.vVertSlots[nVertIndex].vClone;
					for (TV_INT::iterator it = vClone.begin(); it != vClone.end(); ++it)
					{
						if (MeshDataOut.vVertSlots[*it].nTexIndex == nTexIndex)
						{
							bAddNew = false;
							nVertIndex = (*it);
							break;
						}
					}
				}

				MeshDataOut.vVertSlots[nVertIndex].vNormalIndex.insert(pFace->norm[j]);
				int nTangentIndex = pGameMesh->GetFaceVertexTangentBinormal(i, j);
				MeshDataOut.vVertSlots[nVertIndex].vTangentIndex.insert(nTangentIndex);

				if (bAddNew)
				{
					// add new slot
					int nNewVertIndex = MeshDataOut.vVertSlots.size();
					MeshDataOut.vVertSlots.push_back(EmptySlot);
					MeshDataOut.vVertSlots[nNewVertIndex].bUsed = true;
					MeshDataOut.vVertSlots[nNewVertIndex].nVertIndex = nVertIndex;
					MeshDataOut.vVertSlots[nNewVertIndex].nTexIndex = nTexIndex;

					MeshDataOut.vVertSlots[nVertIndex].vClone.push_back(nNewVertIndex);
					nVertIndex = nNewVertIndex;
				}
			}
			else
			{
				MeshDataOut.vVertSlots[nVertIndex].vNormalIndex.insert(pFace->norm[j]);
				int nTangentIndex = pGameMesh->GetFaceVertexTangentBinormal(i, j);
				MeshDataOut.vVertSlots[nVertIndex].vTangentIndex.insert(nTangentIndex);

				// set this slot
				MeshDataOut.vVertSlots[nVertIndex].bUsed = true;
				MeshDataOut.vVertSlots[nVertIndex].nVertIndex = nVertIndex;
				MeshDataOut.vVertSlots[nVertIndex].nTexIndex = nTexIndex;
			}

			Face.nVertIndex[j] = nVertIndex;
		}

		MeshDataOut.vFaces.push_back(Face);
	}

	// setup vertex data
	for (int i = 0; i < nNumVerts; ++i)
	{
		VERTEX_SLOT& LocalSlot = MeshDataOut.vVertSlots[i];

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

	int nNewNumVerts = MeshDataOut.vVertSlots.size();
	for (int i = 0; i < nNewNumVerts; ++i)
	{
		VERTEX_SLOT& LocalSlot = MeshDataOut.vVertSlots[i];

		// get position
		pGameMesh->GetVertex(LocalSlot.nVertIndex, LocalSlot.pos, false);

		if (LocalSlot.pos.x < m_vBoundingBoxMin.x) m_vBoundingBoxMin.x = LocalSlot.pos.x;
		if (LocalSlot.pos.x > m_vBoundingBoxMax.x) m_vBoundingBoxMax.x = LocalSlot.pos.x;

		if (LocalSlot.pos.y < m_vBoundingBoxMin.y) m_vBoundingBoxMin.y = LocalSlot.pos.y;
		if (LocalSlot.pos.y > m_vBoundingBoxMax.y) m_vBoundingBoxMax.y = LocalSlot.pos.y;

		if (LocalSlot.pos.z < m_vBoundingBoxMin.z) m_vBoundingBoxMin.z = LocalSlot.pos.z;
		if (LocalSlot.pos.z > m_vBoundingBoxMax.z) m_vBoundingBoxMax.z = LocalSlot.pos.z;

		// get normal and tangent
		if (i != LocalSlot.nVertIndex)
		{
			LocalSlot.normal = MeshDataOut.vVertSlots[LocalSlot.nVertIndex].normal;
			LocalSlot.tangent = MeshDataOut.vVertSlots[LocalSlot.nVertIndex].tangent;
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

		bool bOK = DumpSkin(MeshDataOut, pGameSkin);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool CModelExporter::DumpSkin(MESH_DATA& MeshDataOut, IGameSkin* pGameSkin)
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
			MeshDataOut.vVertSlots[i].Skins.push_back(Skin);
		}

		SortSkin(MeshDataOut.vVertSlots[i].Skins);
	}

	int nNumAdditionVerts = MeshDataOut.vVertSlots.size();
	for (int i = nNumSkinVerts; i < nNumAdditionVerts; ++i)
	{
		int nOldIndex = MeshDataOut.vVertSlots[i].nVertIndex;
		MeshDataOut.vVertSlots[i].Skins = MeshDataOut.vVertSlots[nOldIndex].Skins;
	}

	return true;
}

bool CModelExporter::DumpController(BONE_INFO& BoneInfo, IGameNode* pGameNode)
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

			COEMath::BuildQuaternionFromEulerXYZ(KeyFrame.qRot, KeyFrame.vRot.x, KeyFrame.vRot.y, KeyFrame.vRot.z);
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

bool CModelExporter::DumpPositionController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpRotationController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpScaleController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpMaxStdPosKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpIndependentPosKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpMaxStdRotKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpEulerRotKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpMaxStdScaleKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpConstraintKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpListKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
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

bool CModelExporter::DumpSampleKey(BONE_INFO& BoneInfo, IGameControl* pGameControl, IGameControlType eType)
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

CModelExporter::KEYFRAME_POS* CModelExporter::GetKeyFrame(TM_KEYFRAME_POS& FramePos, TimeValue time)
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

CModelExporter::KEYFRAME_SCALE* CModelExporter::GetKeyFrame(TM_KEYFRAME_SCALE& FrameScale, TimeValue time)
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

CModelExporter::KEYFRAME_ROT* CModelExporter::GetKeyFrame(TM_KEYFRAME_ROT& FrameRot, TimeValue time)
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

bool CModelExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, KEY_FRAME_MASK eMask, float fValue)
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

bool CModelExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, KEY_FRAME_MASK eMask, const Point3& vValue)
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

bool CModelExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, const Quat& qValue)
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

bool CModelExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, const GMatrix& matValue)
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

void CModelExporter::MaxMat2OeMat(CMatrix4x4& matOut, const GMatrix& matIn)
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

void CModelExporter::OeMat2MaxMat(GMatrix& matOut, const CMatrix4x4& matIn)
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

void CModelExporter::MaxQuat2OeQuat(CQuaternion& qOut, const Quat& qIn)
{
	qOut.x = -qIn.x;
	qOut.y = -qIn.z;
	qOut.z = -qIn.y;
	qOut.w = qIn.w;
}

void CModelExporter::OeQuat2MaxQuat(Quat& qOut, const CQuaternion& qIn)
{
	qOut.x = -qIn.x;
	qOut.y = -qIn.z;
	qOut.z = -qIn.y;
	qOut.w = qIn.w;
}

void CModelExporter::MaxVec2OeVec(CVector3& vOut, const Point3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void CModelExporter::OeVec2MaxVec(Point3& vOut, const CVector3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void CModelExporter::MaxEular2OeEular(CVector3& vOut, const Point3& vIn)
{
	vOut.x = -vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void CModelExporter::OeEular2MaxEular(Point3& vOut, const CVector3& vIn)
{
	vOut.x = -vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void CModelExporter::SortSkin(TV_SKIN& vSkin)
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

bool CModelExporter::ReadConfig()
{
	tstring strFile = GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
	strFile += TS("\\MeshExporterConfig.xml");

	IOEXmlDocument* pXmlDocument = g_pOEXmlMgr->CreateDocument(strFile);
	if (!pXmlDocument) return false;

	IOEXmlNode* pXmlRoot = pXmlDocument->GetRootNode();
	if (!pXmlRoot)
	{
		SAFE_RELEASE(pXmlDocument);
		return false;
	}

	IOEXmlNode* pXmlAnimation = pXmlRoot->FirstChild(TS("Animation"));
	if (!pXmlAnimation)
	{
		SAFE_RELEASE(pXmlDocument);
		return false;
	}

	IOEXmlNode* pXmlOptimizeRotation = pXmlAnimation->FirstChild(TS("OptimizeRotation"));
	IOEXmlNode* pXmlOptimizePosition = pXmlAnimation->FirstChild(TS("OptimizePosition"));
	IOEXmlNode* pXmlOptimizeScale = pXmlAnimation->FirstChild(TS("OptimizeScale"));

	if (!pXmlOptimizeRotation || !pXmlOptimizePosition || !pXmlOptimizeScale)
	{
		SAFE_RELEASE(pXmlDocument);
		return false;
	}

	int nOptimizeRotation = 0;
	int nOptimizePosition = 0;
	int nOptimizeScale = 0;

	pXmlOptimizeRotation->GetAttribute(nOptimizeRotation, TS("enable"));
	if (nOptimizeRotation != 0)
	{
		m_bOptimizeRotation = true;
		pXmlOptimizeRotation->GetText(m_fOptimizeRotation);
	}

	pXmlOptimizePosition->GetAttribute(nOptimizePosition, TS("enable"));
	if (nOptimizePosition != 0)
	{
		m_bOptimizePosition = true;
		pXmlOptimizePosition->GetText(m_fOptimizePosition);
	}

	pXmlOptimizeScale->GetAttribute(nOptimizeScale, TS("enable"));
	if (nOptimizeScale != 0)
	{
		m_bOptimizeScale = true;
		pXmlOptimizeScale->GetText(m_fOptimizeScale);
	}

	SAFE_RELEASE(pXmlDocument);
	return true;
}

bool CModelExporter::SaveConfig()
{
	// TODO: 
	return true;
}

bool CModelExporter::ShowOptionDialog()
{
	if (!m_pDlgModelExpOption)
	{
		m_pParentWindow = new wxWindow();
		m_pParentWindow->SetHWND(m_pInterface->GetMAXHWnd());

		m_pDlgModelExpOption = new CDlgModelExporterOption();
		if (!m_pDlgModelExpOption || !m_pDlgModelExpOption->Initialize(m_pParentWindow))
		{
			wxDELETE(m_pDlgModelExpOption);

			m_pParentWindow->SetHWND(NULL);
			wxDELETE(m_pParentWindow);

			return false;
		}
	}

	wxString strFile = IPathConfigMgr::GetPathConfigMgr()->GetDir(APP_PLUGCFG_DIR);
	strFile += wxT("\\MeshExporter.xml");
	m_pDlgModelExpOption->LoadConfig(strFile);

	int nModalValue = m_pDlgModelExpOption->ShowModal();
	m_pDlgModelExpOption->SaveConfig(strFile);
	if (nModalValue != wxID_OK) return false;

	return true;
}
