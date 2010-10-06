/*!
 * \file OEOGLDevice_Impl.cpp
 * \date 3-3-2010 11:05:31
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEOGLDevice_Impl.h"

COEOGLDevice_Impl::COEOGLDevice_Impl()
{
	g_pOEDevice = this;
	m_bOK = Init();
}

COEOGLDevice_Impl::~COEOGLDevice_Impl()
{
	Destroy();
	g_pOEDevice = NULL;
}

bool COEOGLDevice_Impl::Init()
{
	return true;
}

void COEOGLDevice_Impl::Destroy()
{
	// TODO: 
}

bool COEOGLDevice_Impl::Initialize()
{
	// TODO: 
	return false;
}

void COEOGLDevice_Impl::Terminate()
{
	// TODO: 
}

void COEOGLDevice_Impl::StartPerform()
{
	// TODO: 
}

void COEOGLDevice_Impl::EndPerform()
{
	// TODO: 
}

float COEOGLDevice_Impl::GetCurrTime() const
{
	// TODO: 
	return 0.0f;
}

float COEOGLDevice_Impl::GetDetailTime() const
{
	// TODO: 
	return 0.0f;
}

IOEVertDecl* COEOGLDevice_Impl::CreateVertDecl(const VERT_DECL_ELEMENT* pElement)
{
	// TODO: 
	return NULL;
}

bool COEOGLDevice_Impl::GetDeviceParam(void* pData, const tstring& strParamName)
{
	return false;
}
