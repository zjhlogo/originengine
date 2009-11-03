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
	m_pModel = NULL;
	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));

	m_pNormalVertDecl = NULL;
}

void CMeshApp::Destroy()
{
	// TODO: 
}

bool CMeshApp::Initialize()
{
	static IOEVertDecl::ELEMENT s_NormalVertDecl[] = 
	{
		{IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0},
		{IOEVertDecl::T_COLOR, IOEVertDecl::U_COLOR, 0},
		{IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0},
	};

	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape) return false;
	if (!m_pSimpleShape->Initialize()) return false;

	IOEXmlDocument* pDocument = g_pOEXmlMgr->CreateDocument(t("Model.xml"));
	pDocument->SaveFile(t("Model2.xml"));

	m_pModel = g_pOEModelMgr->CreateModelFromFile(t("Model.xml"));
	if (!m_pModel) return false;

	m_pNormalVertDecl = g_pOEDevice->CreateVertDecl(s_NormalVertDecl);
	if (!m_pNormalVertDecl) return false;

	return true;
}

void CMeshApp::Terminate()
{
	SAFE_RELEASE(m_pNormalVertDecl);
	SAFE_RELEASE(m_pModel);
	SAFE_DELETE(m_pSimpleShape);
	SAFE_DELETE(m_pCamera);
}

void CMeshApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderer->SetTransform(IOERenderer::TT_VIEW, m_pCamera->GetViewMatrix());

	m_pModel->Update(fDetailTime);
}

void CMeshApp::Render(float fDetailTime)
{
	static float s_fTotalTime = 0.0f;

	CMatrix4x4 matWorldViewProj;
	g_pOERenderer->GetTransform(matWorldViewProj, IOERenderer::TT_WORLD_VIEW_PROJ);

	//g_pOERenderer->SetFillMode(IOERenderer::FM_WIREFRAME);
	//g_pOERenderer->SetCullMode(IOERenderer::CMT_NONE);

	s_fTotalTime += fDetailTime*0.0f;
	CVector3 vLightPos;
	vLightPos.x = cos(s_fTotalTime)*300.0f;
	vLightPos.y = 150.0f;
	vLightPos.z = sin(s_fTotalTime)*300.0f;

	//m_pModel->Render(fDetailTime);
	IOEMesh* pMesh = m_pModel->GetMesh();
	int nNumPiece = pMesh->GetNumPieces();
	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = pMesh->GetPiece(i);

		int nMaterialID = pPiece->GetMaterialID();
		IOEModel::MATERIAL* pMaterial = m_pModel->GetMaterial(nMaterialID);
		if (!pMaterial) continue;
		if (pPiece->GetVertDecl() != pMaterial->nVertDecl) continue;

		pMaterial->pShader->SetMatrix(t("g_matWorldViewProj"), matWorldViewProj);
		pMaterial->pShader->SetTexture(t("g_texBase"), pMaterial->pTexture);
		pMaterial->pShader->SetTexture(t("g_texNormal"), pMaterial->pTexNormal);
		pMaterial->pShader->SetVector(t("g_vLightPos"), vLightPos);
		pMaterial->pShader->SetMatrixArray(t("g_matBoneMatrix"), m_pModel->GetMatrixPalette(), m_pModel->GetNumMatrixPalette());
		g_pOERenderer->SetShader(pMaterial->pShader);

		g_pOERenderer->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());

		//RenderPieceNormal(pPiece);
	}

	g_pOERenderer->SetShader(NULL);
	m_pSimpleShape->DrawCube(g_pOERenderer, vLightPos, 10.0f, 0xFFFFFFFF);
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

void CMeshApp::RenderPieceNormal(IOEPiece* pPiece)
{
	static bool s_bFirstRender = true;

	MODEL_VERTEX* pVertSrc = (MODEL_VERTEX*)pPiece->GetVerts();
	int nNumVerts = pPiece->GetNumVerts();

	if (s_bFirstRender)
	{
		int nSize = nNumVerts*2;

		m_vNormalVerts.resize(nSize);
		for (int i = 0; i < nSize; ++i)
		{
			m_vNormalVerts[i].nColor = 0xFFFFFFFF;
		}

		m_vNormalIndis.resize(nSize);
		for (int i = 0; i < nSize; ++i)
		{
			m_vNormalIndis[i] = i;
		}

		s_bFirstRender = false;
	}

	for (int i = 0; i < nNumVerts; ++i)
	{
		m_vNormalVerts[i*2].x = pVertSrc[i].x;
		m_vNormalVerts[i*2].y = pVertSrc[i].y;
		m_vNormalVerts[i*2].z = pVertSrc[i].z;

		m_vNormalVerts[i*2+1].x = pVertSrc[i].x + pVertSrc[i].nx * 2.0f;
		m_vNormalVerts[i*2+1].y = pVertSrc[i].y + pVertSrc[i].ny * 2.0f;
		m_vNormalVerts[i*2+1].z = pVertSrc[i].z + pVertSrc[i].nz * 2.0f;
	}

	g_pOERenderer->SetShader(NULL);
	g_pOERenderer->SetVertDecl(m_pNormalVertDecl);
	g_pOERenderer->DrawLineList(&m_vNormalVerts[0], m_vNormalVerts.size(), &m_vNormalIndis[0], m_vNormalIndis.size());
}
