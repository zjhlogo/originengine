/*!
 * \file TerrainApp.cpp
 * \date 24-5-2009 0:11:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "TerrainApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>
#include <OECore/IOEResMgr.h>

IMPLEMENT_OEAPP(CTerrainApp);

CTerrainApp::CTerrainApp()
{
	Init();
}

CTerrainApp::~CTerrainApp()
{
	Destroy();
}

void CTerrainApp::Init()
{
	m_pModel = NULL;
	m_pTerrain = NULL;
}

void CTerrainApp::Destroy()
{
	// TODO: 
}

bool CTerrainApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("skybox.xml"));
	if (!m_pModel) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pModel);

	m_pTerrain = new CTerrain();
	if (!m_pTerrain || !m_pTerrain->IsOK()) return false;
	if (!m_pTerrain->LoadTerrain()) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pTerrain);

	SetMovementSpeed(500.0f);
	ResetCameraPosRot(CVector3(16000.0f, 1000.0f, 16000.0f), 0.0f, -COEMath::PI/4.0f);
	return true;
}

void CTerrainApp::UserDataTerm()
{
	SAFE_DELETE(m_pTerrain);
	SAFE_RELEASE(m_pModel);
}

void CTerrainApp::Update(float fDetailTime)
{
	// TODO: 
}
