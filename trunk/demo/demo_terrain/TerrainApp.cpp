/*!
 * \file TerrainApp.cpp
 * \date 24-5-2009 0:11:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "TerrainApp.h"
#include "../common/AppHelper.h"

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
	//m_pDecl = NULL;
	m_pTerrainMgr = NULL;
	//m_pShader = NULL;
	//m_pTexture = NULL;
}

void CTerrainApp::Destroy()
{
	// TODO: 
}

bool CTerrainApp::Initialize()
{
	if (!CBaseApp::Initialize()) return false;

	m_pTerrainMgr = new CTerrainMgr();
	if (!m_pTerrainMgr->LoadTerrain()) return false;

	return true;
}

void CTerrainApp::Terminate()
{
	SAFE_DELETE(m_pTerrainMgr);
	CBaseApp::Terminate();
}

void CTerrainApp::Update(float fDetailTime)
{
	CBaseApp::Update(fDetailTime);
	m_pTerrainMgr->UpdateTerrain(m_pCamera->GetEyePos());
}

void CTerrainApp::Render(float fDetailTime)
{
	m_pTerrainMgr->Render(fDetailTime);
}
