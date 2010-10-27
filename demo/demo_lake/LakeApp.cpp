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
#include <OECore/IOEResMgr.h>
#include <libOEMsg/OEMsgList.h>

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
	m_pModelLand = NULL;
	m_pModelLake = NULL;
}

void CLakeApp::Destroy()
{
	// TODO: 
}

bool CLakeApp::UserDataInit()
{
	m_pModelLand = g_pOEResMgr->CreateModel(TS("scene_sand.xml"));
	if (!m_pModelLand) return false;

	m_pModelLake = g_pOEResMgr->CreateModel(TS("scene_water.xml"));
	if (!m_pModelLake) return false;

	IOENode* pRootNode = g_pOECore->GetRootNode();
	pRootNode->AttachObject(m_pModelLand);
	pRootNode->AttachObject(m_pModelLake);

	ResetCameraPosRot(m_pModelLand);
	return true;
}

void CLakeApp::UserDataTerm()
{
	SAFE_DELETE(m_pModelLand);
	SAFE_DELETE(m_pModelLake);
}

void CLakeApp::Update(float fDetailTime)
{
	// TODO: 
}
