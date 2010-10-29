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
#include <OECore/IOERenderSystem.h>
#include <OECore/IOEResMgr.h>
#include <libOEMsg/OEMsgList.h>

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

bool CMeshApp::UserDataInit()
{
	m_pModel = g_pOEResMgr->CreateModel(TS("demo_mesh.xml"));
	if (!m_pModel) return false;
	g_pOECore->GetRootNode()->AttachObject(m_pModel);

	ResetCameraPosRot(m_pModel);
	return true;
}

void CMeshApp::UserDataTerm()
{
	SAFE_RELEASE(m_pModel);
}

void CMeshApp::Update(float fDetailTime)
{
	// TODO: 
}
