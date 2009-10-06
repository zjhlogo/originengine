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
	m_pMesh = NULL;

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

	m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	if (!m_pDecl) return false;

	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape || !m_pSimpleShape->Initialize()) return false;

	m_pMesh = g_pOEMeshMgr->CreateMeshFromFile(t("bone.mesh"));
	if (!m_pMesh) return false;

	return true;
}

void CMeshApp::Terminate()
{
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
	static VERTEX s_Verts[4] = 
	{
		{0.0f, 0.0f, 0.0f, 0xFF00FF00},
		{0.0f, 10.0f, 0.0f, 0xFF00FF00},
		{10.0f, 10.0f, 0.0f, 0xFF00FF00},
		{10.0f, 20.0f, 0.0f, 0xFF00FF00},
	};

	static ushort s_Indis[4] = {0, 1, 2, 3};

	static float s_fTotalTime = 0.0f;

	s_fTotalTime += fDetailTime;
	CVector3 vLightPos;
	vLightPos.x = cos(s_fTotalTime)*10.0f;
	vLightPos.y = 5.0f;
	vLightPos.z = sin(s_fTotalTime)*10.0f;

	m_pSimpleShape->DrawCube(g_pOERenderer, vLightPos);

	RebuildBoneVerts(s_fTotalTime);

	g_pOERenderer->SetVertDecl(m_pDecl);
	//g_pOERenderer->DrawLineStrip(s_Verts, 4, s_Indis, 4);
	g_pOERenderer->DrawLineStrip(&m_vVerts[0], (uint)m_vVerts.size(), &m_vIndis[0], (uint)m_vIndis.size());
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
