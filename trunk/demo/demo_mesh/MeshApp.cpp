/*!
 * \file MeshApp.cpp
 * \date 31-7-2009 20:46:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>

IMPLEMENT_OEAPP(CMeshApp);

CMeshApp::CMeshApp()
{
	Init();
}

CMeshApp::~CMeshApp()
{
	Destroy();
}

void CMeshApp::Init()
{
	m_pMesh = NULL;
}

void CMeshApp::Destroy()
{
	// TODO: 
}

bool CMeshApp::Initialize()
{
	if (!CBaseApp::Initialize()) return false;

	m_pMesh = new CMesh();
	if (!m_pMesh || !m_pMesh->IsOK()) return false;
	m_pCamera->InitFromBBox(m_pMesh->GetBoundingBoxMin(), m_pMesh->GetBoundingBoxMax());

	g_pOECore->GetRootNode()->AttachObject(m_pMesh);
	return true;
}

void CMeshApp::Terminate()
{
	g_pOECore->GetRootNode()->DettachObject(m_pMesh);
	SAFE_DELETE(m_pMesh);
	CBaseApp::Terminate();
}
