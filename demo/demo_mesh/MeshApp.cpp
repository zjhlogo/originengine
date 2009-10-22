/*!
 * \file MeshApp.cpp
 * \date 31-7-2009 20:46:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshApp.h"
#include <assert.h>

CMeshApp::CMeshApp()
{
	Init();
}

CMeshApp::~CMeshApp()
{
	Destroy();
}

void CMeshApp::Init()
{
	m_pCamera = NULL;
	m_pSimpleShape = NULL;

	m_pDecl = NULL;
	m_pDecl2 = NULL;
	m_pMesh = NULL;

	m_pShader = NULL;
	m_pTexBase = NULL;

	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
}

void CMeshApp::Destroy()
{
	// TODO: 
}

bool CMeshApp::Initialize()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
		IOEVertDecl::T_COLOR, IOEVertDecl::U_COLOR, 0,
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	static const IOEVertDecl::ELEMENT s_Decl2[] =
	{
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
		IOEVertDecl::T_FLOAT2, IOEVertDecl::U_TEXCOORD, 0,
		IOEVertDecl::T_UBYTE4, IOEVertDecl::U_BLENDINDICES, 0,
		IOEVertDecl::T_FLOAT4, IOEVertDecl::U_BLENDWEIGHT, 0,
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	if (!m_pDecl) return false;

	m_pDecl2 = g_pOEDevice->CreateVertDecl(s_Decl2);
	if (!m_pDecl2) return false;

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl2, t("mesh.fx"));
	if (!m_pShader) return false;

	m_pTexBase = g_pOETextureMgr->CreateTextureFromFile(t("grid16.png"));
	if (!m_pTexBase) return false;

	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape || !m_pSimpleShape->Initialize()) return false;

	m_pMesh = g_pOEMeshMgr->CreateMeshFromFile(t("Zombie.mesh"));
	if (!m_pMesh) return false;

	g_pOERenderer->SetCullMode(IOERenderer::CMT_NONE);
	BindInitializePose(m_pMesh);

	return true;
}

void CMeshApp::Terminate()
{
	SAFE_RELEASE(m_pTexBase);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pDecl2);
	SAFE_RELEASE(m_pDecl);
	SAFE_RELEASE(m_pMesh);
	SAFE_DELETE(m_pSimpleShape);
	SAFE_DELETE(m_pCamera);
}

void CMeshApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderer->SetTransform(IOERenderer::TT_VIEW, m_pCamera->GetViewMatrix());
}

void CMeshApp::Render(float fDetailTime)
{
	//static VERTEX s_Verts[4] = 
	//{
	//	{0.0f, 0.0f, 0.0f, 0xFF00FF00},
	//	{0.0f, 10.0f, 0.0f, 0xFF00FF00},
	//	{10.0f, 10.0f, 0.0f, 0xFF00FF00},
	//	{10.0f, 20.0f, 0.0f, 0xFF00FF00},
	//};

	//static ushort s_Indis[4] = {0, 1, 2, 3};

	static float s_fTotalTime = 0.0f;

	s_fTotalTime += fDetailTime;
	CVector3 vLightPos;
	vLightPos.x = cos(s_fTotalTime)*10.0f;
	vLightPos.y = 5.0f;
	vLightPos.z = sin(s_fTotalTime)*10.0f;

	//m_pSimpleShape->DrawCube(g_pOERenderer, vLightPos);

	//RebuildBoneVerts(s_fTotalTime);

	//g_pOERenderer->SetFillMode(IOERenderer::FM_WIREFRAME);
	//g_pOERenderer->SetVertDecl(m_pDecl);
	////g_pOERenderer->DrawLineStrip(s_Verts, 4, s_Indis, 4);
	//g_pOERenderer->DrawLineStrip(&m_vVerts[0], (uint)m_vVerts.size(), &m_vIndis[0], (uint)m_vIndis.size());

	CMatrix4x4 matWorldViewProj;
	g_pOERenderer->GetTransform(matWorldViewProj, IOERenderer::TT_WORLD_VIEW_PROJ);
	m_pShader->SetMatrix(t("g_matWorldViewProj"), matWorldViewProj);
	m_pShader->SetTexture(t("g_texBase"), m_pTexBase);
	//m_pShader->SetMatrixArray(t("g_matBoneMatrix"), &m_matBones[0], m_matBones.size());
	g_pOERenderer->SetShader(m_pShader);

	IOEMeshPiece* pPiece = m_pMesh->GetPiece(0);
	g_pOERenderer->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
}

void CMeshApp::OnLButtonDown(int x, int y)
{
	m_bLButtonDown = true;
}

void CMeshApp::OnLButtonUp(int x, int y)
{
	m_bLButtonDown = false;
}

void CMeshApp::OnMouseMove(int dx, int dy)
{
	if (!m_bLButtonDown) return;
	m_nMouseDetailX = dx;
	m_nMouseDetailY = dy;
}

void CMeshApp::OnKeyUp(int nKeyCode)
{
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = false;
}

void CMeshApp::OnKeyDown(int nKeyCode)
{
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = true;
}

bool CMeshApp::UpdateMovement(float fDetailTime)
{
	static const float MOVE_DIST = 50.0f;

	bool bUpdateMovement = m_KeyDown['W'] || m_KeyDown['S'] || m_KeyDown['A'] || m_KeyDown['D'];
	if (!bUpdateMovement) return false;

	if (m_KeyDown['W']) m_pCamera->Move(m_pCamera->GetVectorForword(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['S']) m_pCamera->Move(m_pCamera->GetVectorForword(), -MOVE_DIST*fDetailTime);
	if (m_KeyDown['D']) m_pCamera->Move(m_pCamera->GetVectorRight(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['A']) m_pCamera->Move(m_pCamera->GetVectorRight(), -MOVE_DIST*fDetailTime);

	return true;
}

bool CMeshApp::UpdateRotation(float fDetailTime)
{
	static const float ROTATE_ADJUST = 0.3f;

	if (!m_bLButtonDown) return false;
	if (m_nMouseDetailX == 0 && m_nMouseDetailY == 0) return false;

	if (m_nMouseDetailX) m_pCamera->Rotate(m_pCamera->GetVectorUp(), m_nMouseDetailX*fDetailTime*ROTATE_ADJUST);
	if (m_nMouseDetailY) m_pCamera->Rotate(m_pCamera->GetVectorRight(), m_nMouseDetailY*fDetailTime*ROTATE_ADJUST);

	m_nMouseDetailY = 0;
	m_nMouseDetailX = 0;

	return true;
}

void CMeshApp::RebuildBoneVerts(float fTime)
{
	m_vVerts.clear();
	m_vIndis.clear();

	int nNumBones = m_pMesh->GetNumBones();
	m_matBones.resize(nNumBones);

	CMatrix4x4 matWorld;
	IOEMeshBone* pBone = m_pMesh->GetBone(0);
	BuildBoneVerts(m_vVerts, m_vIndis, fTime, pBone, matWorld, -1);
}

void CMeshApp::BuildBoneVerts(VVERTEX& vVerts, VUSHORT& vIndis, float fTime, IOEMeshBone* pBone, const CMatrix4x4& matParent, int nParentVertIndex)
{
	if (!pBone) return;

	CMatrix4x4 matLocal;
	pBone->SlerpMatrix(matLocal, fTime, true);
	CMatrix4x4 matWorld = matLocal * matParent;

	int nBoneIndex = pBone->GetID();
	if (nBoneIndex >= 0 && nBoneIndex < (int)m_matBones.size())
	{
		m_matBones[nBoneIndex] = matParent;
	}
	else
	{
		assert(false);
	}

	VERTEX Vertex;
	Vertex.x = matWorld.m[12];
	Vertex.y = matWorld.m[13];
	Vertex.z = matWorld.m[14];
	Vertex.nColor = 0xFFFFFFFF;

	int nSelfIndex = (int)vVerts.size();
	vVerts.push_back(Vertex);

	if (nParentVertIndex != -1)
	{
		vIndis.push_back(nParentVertIndex);
		vIndis.push_back(nSelfIndex);
	}

	int nNumChildBone = pBone->GetNumChildren();
	for (int i = 0; i < nNumChildBone; ++i)
	{
		BuildBoneVerts(vVerts, vIndis, fTime, pBone->GetChild(i), matWorld, nSelfIndex);
	}
}

void CMeshApp::BindInitializePose(IOEMesh* pMesh)
{
	int nNumBones = pMesh->GetNumBones();
	std::vector<CMatrix4x4> vBones;
	std::vector<CMatrix4x4> vBonesOffset;
	vBones.resize(nNumBones);
	vBonesOffset.resize(nNumBones);

	for (int i = 0; i < nNumBones; ++i)
	{
		IOEMeshBone* pBone = pMesh->GetBone(i);
		IOEMeshBone* pBoneParent = pBone->GetParent();
		if (pBoneParent)
		{
			CMatrix4x4 matLocal = pBone->GetLocalMatrix() * vBones[pBoneParent->GetID()];
			vBones[i] = matLocal;
		}
		else
		{
			vBones[i] = pBone->GetLocalMatrix();
		}

		vBonesOffset[i] = vBones[i] * vBones[i];
	}

	//IOEMeshPiece* pPiece = pMesh->GetPiece(0);
	//int nNumVerts = pPiece->GetNumVerts();
	//VERTEX_PIECE* pCurrVert = (VERTEX_PIECE*)pPiece->GetVerts();
	//for (int i = 0; i < nNumVerts; ++i)
	//{
	//	int nBoneIndex = pCurrVert[i].nBoneIndex[0];

	//	//if (nBoneIndex == 0)
	//	//{
	//		CVector3 vPos;
	//		vPos.x = pCurrVert[i].x;
	//		vPos.y = pCurrVert[i].y;
	//		vPos.z = pCurrVert[i].z;

	//		//vPos = vPos * vBonesOffset[nBoneIndex];
	//		COEMath::InverseTranslateVector(vPos, vBones[nBoneIndex]);
	//		COEMath::InverseRotateVector(vPos, vBones[nBoneIndex]);

	//		pCurrVert[i].x = vPos.x;
	//		pCurrVert[i].y = vPos.y;
	//		pCurrVert[i].z = vPos.z;
	//	//}
	//}
}
