/*!
 * \file WaterApp.cpp
 * \date 20-6-2009 9:57:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "WaterApp.h"
#include <assert.h>

CWaterApp::CWaterApp()
{
	Init();
}

CWaterApp::~CWaterApp()
{
	Destroy();
}

void CWaterApp::Init()
{
	m_pVerts = NULL;
	m_pIndis = NULL;
	m_nVerts = 0;
	m_nIndis = 0;
	m_pShader = NULL;
	m_pCamera = NULL;
	m_pDlgWaveParam = NULL;

	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
}

void CWaterApp::Destroy()
{
	// TODO: 
}

bool CWaterApp::Initialize()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
		//IOEVertDecl::T_FLOAT3, IOEVertDecl::U_NORMAL, 0,
		//IOEVertDecl::T_FLOAT3, IOEVertDecl::U_COLOR, 0,
		IOEVertDecl::T_FLOAT2, IOEVertDecl::U_TEXCOORD, 0,
		//IOEVertDecl::T_FLOAT3, IOEVertDecl::U_TEXCOORD, 1,
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_nVerts = (NUM_X+1)*(NUM_Z+1);
	m_pVerts = new VERTEX[m_nVerts];
	if (!m_pVerts) return false;

	m_nIndis = NUM_X * NUM_Z * 2 * 3;
	m_pIndis = new ushort[m_nIndis];
	if (!m_pIndis) return false;

	for (int z = 0; z <= NUM_Z; ++z)
	{
		for (int x = 0; x <= NUM_X; ++x)
		{
			int nIndex = z*(NUM_X+1)+x;
			m_pVerts[nIndex].x = x*10.0f;
			m_pVerts[nIndex].y = 0.0f;
			m_pVerts[nIndex].z = z*10.0f;
			m_pVerts[nIndex].u = x/(NUM_X*10.0f);
			m_pVerts[nIndex].v = z/(NUM_Z*10.0f);
		}
	}

	int nIndisIndex = 0;
	for (int z = 0; z < NUM_Z; ++z)
	{
		for (int x = 0; x < NUM_X; ++x)
		{
			int nBaseIndex = z*(NUM_X+1)+x;
			m_pIndis[nIndisIndex++] = nBaseIndex;
			m_pIndis[nIndisIndex++] = nBaseIndex+(NUM_X+1);
			m_pIndis[nIndisIndex++] = nBaseIndex+1;
			m_pIndis[nIndisIndex++] = nBaseIndex+(NUM_X+1);
			m_pIndis[nIndisIndex++] = nBaseIndex+(NUM_X+1)+1;
			m_pIndis[nIndisIndex++] = nBaseIndex+1;
		}
	}

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, _T("water.fx"));
	if (!m_pShader) return false;

	m_pCamera = new CCamera();
	if (!m_pCamera) return false;

	m_pDlgWaveParam = new CDlgWaveParam();
	if (!m_pDlgWaveParam) return false;
	if (!m_pDlgWaveParam->Initialize()) return false;

	g_pOERenderer->SetCullMode(IOERenderer::CMT_NONE);
	g_pOERenderer->SetFillMode(IOERenderer::FM_WIREFRAME);

	return true;
}

void CWaterApp::Terminate()
{
	SAFE_DELETE(m_pDlgWaveParam);
	SAFE_RELEASE(m_pShader);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
}

void CWaterApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderer->SetTransform(IOERenderer::TT_VIEW, m_pCamera->GetViewMatrix());
}

void CWaterApp::Render(float fDetailTime)
{
	static float s_fTime = 0.0f;

	CMatrix4x4 matWorldViewProj;
	g_pOERenderer->GetTransform(matWorldViewProj, IOERenderer::TT_WORLD_VIEW_PROJ);
	m_pShader->SetMatrix(_T("g_matWorldViewProj"), matWorldViewProj);

	s_fTime += (fDetailTime*m_pDlgWaveParam->GetTimeScale());
	m_pShader->SetFloat(_T("fTime"), s_fTime);

	m_pShader->SetVector(_T("vWaveFreq"), m_pDlgWaveParam->GetVecFreq()*m_pDlgWaveParam->GetFreqScale());
	m_pShader->SetVector(_T("vWaveSpeed"), m_pDlgWaveParam->GetVecSpeed()*m_pDlgWaveParam->GetSpeedScale());
	m_pShader->SetVector(_T("vWaveDirX"), m_pDlgWaveParam->GetVecDirX());
	m_pShader->SetVector(_T("vWaveDirY"), m_pDlgWaveParam->GetVecDirY());
	m_pShader->SetVector(_T("vWaveHeight"), m_pDlgWaveParam->GetVecHeight()*m_pDlgWaveParam->GetHeightScale());

	m_pShader->DrawTriList(m_pVerts, m_nVerts, m_pIndis, m_nIndis);
}

void CWaterApp::OnLButtonDown(int x, int y)
{
	m_bLButtonDown = true;
}

void CWaterApp::OnLButtonUp(int x, int y)
{
	m_bLButtonDown = false;
}

void CWaterApp::OnMouseMove(int dx, int dy)
{
	if (!m_bLButtonDown) return;
	m_nMouseDetailX = dx;
	m_nMouseDetailY = dy;
}

void CWaterApp::OnKeyUp(int nKeyCode)
{
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = false;
}

void CWaterApp::OnKeyDown(int nKeyCode)
{
	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = true;

	if (nKeyCode == VK_ESCAPE) g_pOECore->End();
	else if (nKeyCode == VK_F1) m_pDlgWaveParam->Show(!m_pDlgWaveParam->IsVisible());
}

bool CWaterApp::UpdateMovement(float fDetailTime)
{
	static const float MOVE_DIST = 100.0f;

	bool bUpdateMovement = m_KeyDown['W'] || m_KeyDown['S'] || m_KeyDown['A'] || m_KeyDown['D'];
	if (!bUpdateMovement) return false;

	if (m_KeyDown['W']) m_pCamera->Move(m_pCamera->GetVectorForword(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['S']) m_pCamera->Move(m_pCamera->GetVectorForword(), -MOVE_DIST*fDetailTime);
	if (m_KeyDown['D']) m_pCamera->Move(m_pCamera->GetVectorRight(), MOVE_DIST*fDetailTime);
	if (m_KeyDown['A']) m_pCamera->Move(m_pCamera->GetVectorRight(), -MOVE_DIST*fDetailTime);

	return true;
}

bool CWaterApp::UpdateRotation(float fDetailTime)
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