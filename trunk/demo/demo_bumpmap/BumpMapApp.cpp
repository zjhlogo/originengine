/*!
 * \file BumpMapApp.cpp
 * \date 6-7-2009 9:23:27
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "BumpMapApp.h"
#include "../common/AppHelper.h"
#include <OECore/IOECore.h>

IMPLEMENT_OEAPP(CBumpMapApp);

CBumpMapApp::CBumpMapApp()
{
	Init();
}

CBumpMapApp::~CBumpMapApp()
{
	Destroy();
}

void CBumpMapApp::Init()
{
	m_pSimpleShape = NULL;
	m_pBumpMap = NULL;
}

void CBumpMapApp::Destroy()
{
	// TODO: 
}

bool CBumpMapApp::Initialize()
{
	if (!CBaseApp::Initialize()) return false;

	m_pSimpleShape = new CSimpleShape();
	if (!m_pSimpleShape || !m_pSimpleShape->IsOK()) return false;

	m_pBumpMap = new CBumpMap();
	if (!m_pBumpMap || !m_pBumpMap->IsOK()) return false;

	g_pOECore->GetRootNode()->AttachObject(m_pBumpMap);
	g_pOECore->GetRootNode()->AttachObject(m_pSimpleShape);

	return true;
}

void CBumpMapApp::Terminate()
{
	g_pOECore->GetRootNode()->DettachObject(m_pBumpMap);
	g_pOECore->GetRootNode()->DettachObject(m_pSimpleShape);
	SAFE_DELETE(m_pBumpMap);
	SAFE_DELETE(m_pSimpleShape);
	CBaseApp::Terminate();
}

void CBumpMapApp::Update(float fDetailTime)
{
	static float s_fTotalTime = 0.0f;

	CBaseApp::Update(fDetailTime);

	s_fTotalTime += fDetailTime;
	CVector3 vLightPos;
	vLightPos.x = cos(s_fTotalTime)*10.0f;
	vLightPos.y = 5.0f;
	vLightPos.z = sin(s_fTotalTime)*10.0f;

	m_pBumpMap->SetEyePos(m_pCamera->GetEyePos());
	m_pBumpMap->SetLightPos(vLightPos);

	m_pSimpleShape->SetPosition(vLightPos);
}

bool CBumpMapApp::OnKeyDown(COEMsgKeyboard& msg)
{
	CBaseApp::OnKeyDown(msg);

	if (m_KeyDown['1']) m_pBumpMap->SetTechnique(TS("NormalMap"));
	if (m_KeyDown['2']) m_pBumpMap->SetTechnique(TS("ParallaxMap"));
	if (m_KeyDown['3']) m_pBumpMap->SetTechnique(TS("DiffuseTexture"));
	if (m_KeyDown['4']) m_pBumpMap->SetTechnique(TS("NormalTexture"));
	if (m_KeyDown['5']) m_pBumpMap->SetTechnique(TS("HeightMapTexture"));

	return true;
}
