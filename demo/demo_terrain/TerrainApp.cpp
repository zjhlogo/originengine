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
	m_pTerrain = NULL;
}

void CTerrainApp::Destroy()
{
	// TODO: 
}

bool CTerrainApp::UserDataInit()
{
	m_pTerrain = new CTerrain();
	if (!m_pTerrain || !m_pTerrain->IsOK()) return false;
	if (!m_pTerrain->LoadTerrain()) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pTerrain);

	return true;
}

void CTerrainApp::UserDataTerm()
{
	SAFE_DELETE(m_pTerrain);
}

void CTerrainApp::Update(float fDetailTime)
{
	// TODO: 
}
