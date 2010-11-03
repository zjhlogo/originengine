/*!
 * \file LakeApp.cpp
 * \date 10-27-2010 9:29:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "LakeApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/IOETextureMgr.h>
#include <OECore/IOEResMgr.h>
#include <OECore/IOERenderMgr.h>
#include <OECore/IOEDevice.h>
#include <OEUI/IOEUIRenderSystem.h>
#include <libOEMsg/OEMsgList.h>
#include <libOEUI/OEUIPicture.h>
#include "OEWaterRender_Impl.h"

IMPLEMENT_OEAPP(CLakeApp);

CLakeApp::CLakeApp()
{
	Init();
}

CLakeApp::~CLakeApp()
{
	Destroy();
}

void CLakeApp::Init()
{
	m_pModelSky = NULL;
	m_pModelLand = NULL;
	m_pModelLake = NULL;
	m_pRenderTargetReflect = NULL;
	m_pRenderTargetRefract = NULL;
}

void CLakeApp::Destroy()
{
	// TODO: 
}

bool CLakeApp::UserDataInit()
{
	g_pOERenderMgr->AddRenderHandle(new COEWaterRender_Impl());

	m_pModelSky = g_pOEResMgr->CreateModel(TS("skybox.xml"));
	if (!m_pModelSky) return false;

	m_pModelLand = g_pOEResMgr->CreateModel(TS("scene_sand.xml"));
	if (!m_pModelLand) return false;

	m_pModelLake = g_pOEResMgr->CreateModel(TS("scene_water.xml"));
	if (!m_pModelLake) return false;

	int nWindowWidth = 800;
	int nWindowHeight = 600;
	g_pOEDevice->GetDeviceParam(&nWindowWidth, TS("WINDOW_WIDTH"));
	g_pOEDevice->GetDeviceParam(&nWindowHeight, TS("WINDOW_HEIGHT"));

	m_pRenderTargetReflect = g_pOETextureMgr->CreateRenderTargetTexture(nWindowWidth>>2, nWindowHeight>>2, TF_A8R8G8B8);
	if (!m_pRenderTargetReflect) return false;

	m_pRenderTargetRefract = g_pOETextureMgr->CreateRenderTargetTexture(nWindowWidth, nWindowHeight, TF_A8R8G8B8);
	if (!m_pRenderTargetRefract) return false;

	m_pModelLake->GetRenderData()->SetTexture(TS("RenderTargetReflect"), m_pRenderTargetReflect);
	m_pModelLake->GetRenderData()->SetTexture(TS("RenderTargetRefract"), m_pRenderTargetRefract);

	//COEUIPicture* pPicReflect = new COEUIPicture(g_pOEUIRenderSystem->GetScreen());
	//if (!pPicReflect) return false;
	//pPicReflect->SetPicture(m_pRenderTargetReflect);
	//pPicReflect->SetSize(CSize(200.0f, 150.0f));

	//COEUIPicture* pPicRefract = new COEUIPicture(g_pOEUIRenderSystem->GetScreen());
	//if (!pPicRefract) return false;
	//pPicRefract->SetPicture(m_pRenderTargetRefract);
	//pPicRefract->SetPosition(CPoint(nWindowWidth-200.0f, 0.0f));
	//pPicRefract->SetSize(CSize(200.0f, 150.0f));

	IOENode* pRootNode = g_pOECore->GetRootNode();
	pRootNode->AttachObject(m_pModelSky);
	pRootNode->AttachObject(m_pModelLand);
	pRootNode->AttachObject(m_pModelLake);

	ResetCameraPosRot(CVector3(75.0f, 26.0f, -141.0f), CQuaternion(-0.1f, 0.28f, 0.03f, 0.95f));
	return true;
}

void CLakeApp::UserDataTerm()
{
	SAFE_RELEASE(m_pModelSky);
	SAFE_RELEASE(m_pModelLand);
	SAFE_RELEASE(m_pModelLake);
	SAFE_RELEASE(m_pRenderTargetReflect);
	SAFE_RELEASE(m_pRenderTargetRefract);
}

void CLakeApp::Update(float fDetailTime)
{
	// TODO: 
}
