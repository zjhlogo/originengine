/*!
 * \file TestApp.cpp
 * \date 24-5-2009 0:11:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "TestApp.h"
#include <OEInterfaces.h>

CTestApp::CTestApp()
{
	Init();
}

CTestApp::~CTestApp()
{
	Destroy();
}

void CTestApp::Init()
{
	m_pDecl = NULL;
}

void CTestApp::Destroy()
{
	// TODO: 
}

bool CTestApp::Initialize()
{
	static const IOEVertDecl::ELEMENT s_Decl[] =
	{
		IOEVertDecl::T_FLOAT3, IOEVertDecl::U_POSITION, 0,
		IOEVertDecl::T_COLOR, IOEVertDecl::U_COLOR, 0,
		IOEVertDecl::T_UNKNOWN, IOEVertDecl::U_UNKNOWN, 0,
	};

	m_pDecl = g_pOEDevice->CreateVertDecl(s_Decl);
	if (!m_pDecl) return false;

	return true;
}

void CTestApp::Terminate()
{
	SAFE_RELEASE(m_pDecl);
}

void CTestApp::Update(float fDetailTime)
{
	// TODO: 
}

void CTestApp::Render(float fDetailTime)
{
	static const VERTEX s_Verts[4] =
	{
		{-1.0f, -1.0f, 0.0f, 0xFFFFFFFF},
		{-1.0f, 1.0f, 0.0f, 0xFFFFFFFF},
		{1.0f, 1.0f, 0.0f, 0xFFFFFFFF},
		{1.0f, -1.0f, 0.0f, 0xFFFFFFFF},
	};

	static const ushort s_Indis[4] = {0, 1, 3, 2};

	g_pOERenderer->SetVertDecl(m_pDecl);
	g_pOERenderer->DrawTriStrip(s_Verts, 4, s_Indis, 4);
}
