/*!
 * \file WaterApp.cpp
 * \date 20-6-2009 9:57:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "WaterApp.h"
#include "resource.h"
#include "../common/AppHelper.h"
#include "../common/wxInitHelper.h"
#include <OECore/IOECore.h>
#include <OECore/IOEDevice.h>
#include <libOEMsg/OEMsgList.h>

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
	m_pDlgWaveSetting = NULL;
	m_pWater = NULL;
}

void CWaterApp::Destroy()
{
	// TODO: 
}

bool CWaterApp::UserDataInit()
{
	// initialize gui
	if (!wxInitHelper::Initialize()) return false;
	if (!wxInitHelper::AddMemoryXrc(TS("XRC"), IDR_XRC_DLGWAVESETTING, TS("DlgWaveSetting.xrc"))) return false;
	m_pDlgWaveSetting = new CDlgWaveSetting();
	if (!m_pDlgWaveSetting || !m_pDlgWaveSetting->Initialize()) return false;

	m_pWater = new CWater();
	if (!m_pWater || !m_pWater->IsOK()) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pWater);

	ResetCameraPosRot(CVector3(399.75037f, 532.55792f, -279.13873f), 0.0f, -COEMath::PI/4.0f);

	g_pOEDevice->RegisterEvent(OMI_KEY_DOWN, this, (MSG_FUNC)&CWaterApp::OnKeyDown);

	return true;
}

void CWaterApp::UserDataTerm()
{
	SAFE_DELETE(m_pDlgWaveSetting);
	wxInitHelper::Uninitialize();

	SAFE_DELETE(m_pWater);
}

void CWaterApp::Update(float fDetailTime)
{
	static float s_fTime = 0.0f;

	s_fTime += (fDetailTime*m_pDlgWaveSetting->GetTimeScale());
	m_pWater->SetTime(s_fTime);
	m_pWater->SetVecFreq(m_pDlgWaveSetting->GetVecFreq()*m_pDlgWaveSetting->GetFreqScale());
	m_pWater->SetVecSpeed(m_pDlgWaveSetting->GetVecSpeed()*m_pDlgWaveSetting->GetSpeedScale());
	m_pWater->SetVecDirX(m_pDlgWaveSetting->GetVecDirX());
	m_pWater->SetVecDirY(m_pDlgWaveSetting->GetVecDirY());
	m_pWater->SetVecHeight(m_pDlgWaveSetting->GetVecHeight()*m_pDlgWaveSetting->GetHeightScale());

	IOENode* pCameraNode = g_pOECore->GetRootNode()->GetChildNode(TS("Camera"));
	if (!pCameraNode) return;
	m_pWater->SetEyePos(pCameraNode->GetPosition());
}

bool CWaterApp::OnKeyDown(COEMsgKeyboard& msg)
{
	if (msg.GetKeyCode() == 0x70) m_pDlgWaveSetting->Show(!m_pDlgWaveSetting->IsVisible());		// TODO: 0x70 == VK_F1

	return true;
}
