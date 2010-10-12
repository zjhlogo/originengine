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
#include <OECore/IOEResMgr.h>

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
	m_pModel = NULL;
}

void CMeshApp::Destroy()
{
	// TODO: 
}

bool CMeshApp::Initialize()
{
	if (!CBaseApp::Initialize()) return false;

	m_pModel = g_pOEResMgr->CreateModel(TS("demo_mesh.xml"));
	if (!m_pModel) return false;

	IOEMesh* pMesh = m_pModel->GetRenderData()->GetMesh();
	m_pCamera->InitFromBBox(pMesh->GetBoundingBoxMin(), pMesh->GetBoundingBoxMax());

	g_pOECore->GetRootNode()->AttachObject(m_pModel);
	return true;
}

void CMeshApp::Terminate()
{
	SAFE_DELETE(m_pModel);
	CBaseApp::Terminate();
}
