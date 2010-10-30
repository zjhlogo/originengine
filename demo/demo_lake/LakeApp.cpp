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
	m_pRenderTargetTexture = NULL;
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

	m_pRenderTargetTexture = g_pOETextureMgr->CreateRenderTargetTexture(800, 600, TF_A8R8G8B8);
	if (!m_pRenderTargetTexture) return false;
	m_pModelLake->GetRenderData()->SetTexture(TS("RenderTargetTexture"), m_pRenderTargetTexture);

	COEUIPicture* pPicture = new COEUIPicture(g_pOEUIRenderSystem->GetScreen());
	if (!pPicture) return false;
	pPicture->SetPicture(m_pRenderTargetTexture);
	pPicture->SetSize(CSize(200.0f, 150.0f));

	IOENode* pRootNode = g_pOECore->GetRootNode();
	pRootNode->AttachObject(m_pModelSky);
	pRootNode->AttachObject(m_pModelLake);
	pRootNode->AttachObject(m_pModelLand);

	ResetCameraPosRot(m_pModelLand);
	return true;
}

void CLakeApp::UserDataTerm()
{
	SAFE_RELEASE(m_pModelSky);
	SAFE_RELEASE(m_pModelLand);
	SAFE_RELEASE(m_pModelLake);
	SAFE_RELEASE(m_pRenderTargetTexture);
}

void CLakeApp::Update(float fDetailTime)
{
	// TODO: 
}
