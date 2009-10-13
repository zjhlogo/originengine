/*!
 * \file OEOpenGLDevice_Impl.cpp
 * \date 24-5-2009 10:58:20
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEOpenGLDevice_Impl.h"

COEOpenGLDevice_Impl::COEOpenGLDevice_Impl()
{
	g_pOEDevice = this;
	Init();
	m_bOK = true;
}

COEOpenGLDevice_Impl::~COEOpenGLDevice_Impl()
{
	Destroy();
	g_pOEDevice = NULL;
}

void COEOpenGLDevice_Impl::Init()
{
	// TODO: 
}

void COEOpenGLDevice_Impl::Destroy()
{
	// TODO: 
}

bool COEOpenGLDevice_Impl::CreateDevice()
{
	// TODO: 
	return false;
}

void COEOpenGLDevice_Impl::DestroyDevice()
{
	// TODO: 
}

void COEOpenGLDevice_Impl::StartPerform()
{
	// TODO: 
}

void COEOpenGLDevice_Impl::EndPerform()
{
	// TODO: 
}
float COEOpenGLDevice_Impl::GetFPS()
{
	// TODO:
	return 0.0f;
}

IOEVertDecl* COEOpenGLDevice_Impl::CreateVertDecl(const IOEVertDecl::ELEMENT* pElement)
{
	// TODO: 
	return NULL;
}

bool COEOpenGLDevice_Impl::GetDeviceParam(void* pData, const tstring& strParamName)
{
	return false;
}
