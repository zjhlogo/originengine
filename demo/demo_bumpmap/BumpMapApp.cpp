/*!
 * \file BumpMapApp.cpp
 * \date 6-7-2009 9:23:27
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "BumpMapApp.h"
#include <assert.h>

CBumpMapApp::CBumpMapApp()
{
	Init();
}

CBumpMapApp::~CBumpMapApp()
{
	Destroy();
}

void CBumpMapApp::Init()
{
	m_pCamera = NULL;
	m_pSimpleShape = NULL;

	m_pShader = NULL;
	m_pTexBase = NULL;
	m_pTexNormal = NULL;
	m_pTexHeight = NULL;

	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
}

void CBumpMapApp::Destroy()
{
	// TODO: 
}

bool CBumpMapApp::Initialize()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
		IOEVertDecl::T_FLOAT2, IOEVertDecl::U_TEXCOORD, 0,
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_TEXCOORD, 1,		// normal
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_TEXCOORD, 2,		// tangent
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, _T("bumpmap.fx"));
	if (!m_pShader) return false;

	m_pTexBase = g_pOETextureMgr->CreateTextureFromFile(_T("rock.png"));
	if (!m_pTexBase) return false;

	m_pTexNormal = g_pOETextureMgr->CreateTextureFromFile(_T("rock_normal.png"));
	if (!m_pTexNormal) return false;

	m_pTexHeight = g_pOETextureMgr->CreateTextureFromFile(_T("rock_height.png"));
	if (!m_pTexHeight) return false;

	m_pCamera = new CCamera();

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape->Initialize()) return false;

	return true;
}

void CBumpMapApp::Terminate()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTexBase);
	SAFE_RELEASE(m_pTexNormal);
	SAFE_RELEASE(m_pTexHeight);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSimpleShape);
}

void CBumpMapApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderer->SetTransform(IOERenderer::TT_VIEW, m_pCamera->GetViewMatrix());
}

void CBumpMapApp::Render(float fDetailTime)
{
	static float s_fTotalTime = 0.0f;

	static const VERTEX s_Verts[4] =
	{
		{-10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		{-10.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		{10.0f, 0.0f, 10.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		{10.0f, 0.0f, -10.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	};

	static const ushort s_Indis[6] = {0, 1, 3, 1, 2, 3};

	CMatrix4x4 matWorldViewProj;
	g_pOERenderer->GetTransform(matWorldViewProj, IOERenderer::TT_WORLD_VIEW_PROJ);
	m_pShader->SetMatrix(_T("g_matWorldViewProj"), matWorldViewProj);

	s_fTotalTime += fDetailTime;
	m_vLightPos.x = cos(s_fTotalTime)*10.0f;
	m_vLightPos.y = 5.0f;
	m_vLightPos.z = sin(s_fTotalTime)*10.0f;

	m_pShader->SetVector(_T("g_vLightPos"), m_vLightPos);

	m_pShader->SetTexture(_T("g_texBase"), m_pTexBase);
	m_pShader->SetTexture(_T("g_texNormal"), m_pTexNormal);
	m_pShader->SetTexture(_T("g_texHeight"), m_pTexHeight);

	g_pOERenderer->SetShader(m_pShader);
	g_pOERenderer->DrawTriList(s_Verts, 4, s_Indis, 6);
	g_pOERenderer->SetShader(NULL);

	m_pSimpleShape->DrawCube(g_pOERenderer, m_vLightPos);
}

void CBumpMapApp::OnLButtonDown(int x, int y)
{
	m_bLButtonDown = true;
}

void CBumpMapApp::OnLButtonUp(int x, int y)
{
	m_bLButtonDown = false;
}

void CBumpMapApp::OnMouseMove(int dx, int dy)
{
	if (!m_bLButtonDown) return;
	m_nMouseDetailX = dx;
	m_nMouseDetailY = dy;
}

void CBumpMapApp::OnKeyUp(int nKeyCode)
{
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = false;
}

void CBumpMapApp::OnKeyDown(int nKeyCode)
{
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE

	if (m_KeyDown['1']) m_pShader->SetTechnique(_T("Normal"));
	if (m_KeyDown['2']) m_pShader->SetTechnique(_T("Parallax"));
	if (m_KeyDown['3']) m_pShader->SetTechnique(_T("BaseTex"));
	if (m_KeyDown['4']) m_pShader->SetTechnique(_T("NormalTex"));
	if (m_KeyDown['5']) m_pShader->SetTechnique(_T("HeightTex"));
}

bool CBumpMapApp::UpdateMovement(float fDetailTime)
{
	static const float MOVE_DIST = 20.0f;

	bool bUpdateMovement = m_KeyDown['W'] || m_KeyDown['S'] || m_KeyDown['A'] || m_KeyDown['D'];
	if (!bUpdateMovement) return false;

	if (m_KeyDown['W']) m_pCamera->Move(m_pCamera->GetVectorForword(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['S']) m_pCamera->Move(m_pCamera->GetVectorForword(), -MOVE_DIST*fDetailTime);
	if (m_KeyDown['D']) m_pCamera->Move(m_pCamera->GetVectorRight(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['A']) m_pCamera->Move(m_pCamera->GetVectorRight(), -MOVE_DIST*fDetailTime);

	return true;
}

bool CBumpMapApp::UpdateRotation(float fDetailTime)
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
