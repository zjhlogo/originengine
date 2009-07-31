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
	m_pVerts = NULL;
	m_pIndis = NULL;
	m_nVerts = 0;
	m_nIndis = 0;
	m_pShader = NULL;
	m_pTexture = NULL;
	m_pCamera = NULL;
	m_pSimpleShape = NULL;

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
		IOEVertDecl::T_FLOAT2, IOEVertDecl::U_TEXCOORD, 0,
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_TEXCOORD, 1,
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, _T("mesh.fx"));
	if (!m_pShader) return false;

	m_pTexture = g_pOETextureMgr->CreateTextureFromFile(_T("rock.png"));
	if (!m_pTexture) return false;

	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape || !m_pSimpleShape->Initialize()) return false;

	IOEFile* pFile = g_pOEFileMgr->OpenFile(_T("teapot.ms3d.mesh"));
	if (!pFile) return false;

	pFile->Read(&m_nVerts, sizeof(m_nVerts));
	m_pVerts = new VERTEX[m_nVerts];
	pFile->Read(m_pVerts, sizeof(VERTEX)*m_nVerts);

	pFile->Read(&m_nIndis, sizeof(m_nIndis));
	m_pIndis = new ushort[m_nIndis];
	pFile->Read(m_pIndis, sizeof(ushort)*m_nIndis);

	SAFE_RELEASE(pFile);
	return true;
}

void CMeshApp::Terminate()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTexture);

	SAFE_DELETE(m_pSimpleShape);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
	m_nVerts = 0;
	m_nIndis = 0;
}

void CMeshApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderer->SetTransform(IOERenderer::TT_VIEW, m_pCamera->GetViewMatrix());
}

void CMeshApp::Render(float fDetailTime)
{
	static float s_fTotalTime = 0.0f;

	CMatrix4x4 matWorldViewProj;
	g_pOERenderer->GetTransform(matWorldViewProj, IOERenderer::TT_WORLD_VIEW_PROJ);

	s_fTotalTime += fDetailTime;
	CVector3 vLightPos;
	vLightPos.x = cos(s_fTotalTime)*10.0f;
	vLightPos.y = 5.0f;
	vLightPos.z = sin(s_fTotalTime)*10.0f;

	m_pShader->SetMatrix(_T("g_matWorldViewProj"), matWorldViewProj);
	m_pShader->SetVector(_T("g_vLightPos"), vLightPos);
	m_pShader->SetTexture(_T("g_texBase"), m_pTexture);

	g_pOERenderer->SetShader(m_pShader);
	g_pOERenderer->DrawTriList(m_pVerts, m_nVerts, m_pIndis, m_nIndis);
	g_pOERenderer->SetShader(NULL);

	m_pSimpleShape->DrawCube(g_pOERenderer, vLightPos);
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
