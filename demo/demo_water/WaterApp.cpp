/*!
 * \file WaterApp.cpp
 * \date 20-6-2009 9:57:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "WaterApp.h"
#include "resource.h"
#include "DlgWaveSetting.h"

#include "../common/AppHelper.h"
#include "../common/wxInitHelper.h"

#include <OERenderSystemUtil.h>
#include <OEMsgID.h>
#include <assert.h>

#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>

IMPLEMENT_OEAPP(CWaterApp);

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
	m_pDlgWaveSetting = NULL;

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
	static const VERT_DECL_ELEMENT s_Decl[] =
	{
		VDT_FLOAT3, VDU_POSITION, 0,
		//VDT_FLOAT3, VDU_NORMAL, 0,
		//VDT_FLOAT3, VDU_COLOR, 0,
		VDT_FLOAT2, VDU_TEXCOORD, 0,
		//VDT_FLOAT3, VDU_TEXCOORD, 1,
		VDT_UNKNOWN, VDU_UNKNOWN, 0,
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

	m_pShader = g_pOEShaderMgr->CreateShader(s_Decl, TS("demo_water.fx"));
	if (!m_pShader) return false;

	m_pCamera = new CCamera();
	if (!m_pCamera) return false;
	m_pCamera->Initialize(CVector3(399.75037f, 532.55792f, -279.13873f), CVector3(399.73721f, 531.89636f, -278.38895f));
	g_pOERenderSystem->SetTransform(TT_VIEW, m_pCamera->GetViewMatrix());

	// initialize gui
	if (!wxInitHelper::Initialize()) return false;
	if (!wxInitHelper::AddMemoryXrc(TS("XRC"), IDR_XRC_DLGWAVESETTING, TS("DlgWaveSetting.xrc"))) return false;
	m_pDlgWaveSetting = new CDlgWaveSetting();
	if (!m_pDlgWaveSetting || !m_pDlgWaveSetting->Initialize()) return false;

	// registe message
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_DOWN, this, (MSG_FUNC)&CWaterApp::OnLButtonDown);
	g_pOEMsgMgr->RegisterMessage(OMI_LBUTTON_UP, this, (MSG_FUNC)&CWaterApp::OnLButtonUp);
	g_pOEMsgMgr->RegisterMessage(OMI_MOUSE_MOVE, this, (MSG_FUNC)&CWaterApp::OnMouseMove);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_DOWN, this, (MSG_FUNC)&CWaterApp::OnKeyDown);
	g_pOEMsgMgr->RegisterMessage(OMI_KEY_UP, this, (MSG_FUNC)&CWaterApp::OnKeyUp);

	return true;
}

void CWaterApp::Terminate()
{
	SAFE_DELETE(m_pDlgWaveSetting);
	wxInitHelper::Uninitialize();

	SAFE_RELEASE(m_pShader);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
}

void CWaterApp::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov) g_pOERenderSystem->SetTransform(TT_VIEW, m_pCamera->GetViewMatrix());
}

void CWaterApp::Render3D(float fDetailTime)
{
	static float s_fTime = 0.0f;

	CDefaultRenderState DefaultState;
	g_pOERenderSystem->SetFillMode(FM_WIREFRAME);

	CMatrix4x4 matWorldViewProj;
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_WORLD_VIEW_PROJ);
	m_pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);

	s_fTime += (fDetailTime*m_pDlgWaveSetting->GetTimeScale());
	m_pShader->SetFloat(TS("fTime"), s_fTime);

	m_pShader->SetVector(TS("vWaveFreq"), m_pDlgWaveSetting->GetVecFreq()*m_pDlgWaveSetting->GetFreqScale());
	m_pShader->SetVector(TS("vWaveSpeed"), m_pDlgWaveSetting->GetVecSpeed()*m_pDlgWaveSetting->GetSpeedScale());
	m_pShader->SetVector(TS("vWaveDirX"), m_pDlgWaveSetting->GetVecDirX());
	m_pShader->SetVector(TS("vWaveDirY"), m_pDlgWaveSetting->GetVecDirY());
	m_pShader->SetVector(TS("vWaveHeight"), m_pDlgWaveSetting->GetVecHeight()*m_pDlgWaveSetting->GetHeightScale());
	m_pShader->SetVector(TS("g_vEyePos"), m_pCamera->GetEyePos());
	g_pOERenderSystem->SetShader(m_pShader);
	g_pOERenderSystem->DrawTriList(m_pVerts, m_nVerts, m_pIndis, m_nIndis);
}

void CWaterApp::Render2D(float fDetailTime)
{
	// TODO: 
}

bool CWaterApp::OnLButtonDown(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_bLButtonDown = true;
	return true;
}

bool CWaterApp::OnLButtonUp(uint nMsgID, COEDataBufferRead* pDBRead)
{
	m_bLButtonDown = false;
	return true;
}

bool CWaterApp::OnMouseMove(uint nMsgID, COEDataBufferRead* pDBRead)
{
	if (!m_bLButtonDown) return true;
	COEMsg msg(pDBRead);
	msg.Read(m_nMouseDetailX);
	msg.Read(m_nMouseDetailY);
	return true;
}

bool CWaterApp::OnKeyUp(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	int nKeyCode = 0;
	msg.Read(nKeyCode);

	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = false;

	return true;
}

bool CWaterApp::OnKeyDown(uint nMsgID, COEDataBufferRead* pDBRead)
{
	COEMsg msg(pDBRead);

	int nKeyCode = 0;
	msg.Read(nKeyCode);

	assert(nKeyCode > 0 && nKeyCode < KEY_COUNT);
	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE
	else if (m_KeyDown[0x70]) m_pDlgWaveSetting->Show(!m_pDlgWaveSetting->IsVisible());		// TODO: 0x70 == VK_F1

	return true;
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
