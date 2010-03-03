/*!
 * \file OEOGLDevice_Impl.cpp
 * \date 3-3-2010 11:05:31
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEOGLDevice_Impl.h"

COEOGLevice_Impl::COEOGLevice_Impl()
{
	g_pOEDevice = this;
	m_bOK = Init();
}

COEOGLevice_Impl::~COEOGLevice_Impl()
{
	Destroy();
	g_pOEDevice = NULL;
}

bool COEOGLevice_Impl::Init()
{
	return true;
}

void COEOGLevice_Impl::Destroy()
{
	// TODO: 
}

bool COEOGLevice_Impl::Initialize()
{
	// TODO: 
	return false;
}

void COEOGLevice_Impl::Terminate()
{
	// TODO: 
}

void COEOGLevice_Impl::StartPerform()
{
	// TODO: 
}

void COEOGLevice_Impl::EndPerform()
{
	// TODO: 
}

float COEOGLevice_Impl::GetCurrTime() const
{
	// TODO: 
	return 0.0f;
}

float COEOGLevice_Impl::GetDetailTime() const
{
	// TODO: 
	return 0.0f;
}

IOEVertDecl* COEOGLevice_Impl::CreateVertDecl(const VERT_DECL_ELEMENT* pElement)
{
	// TODO: 
	return NULL;
}

bool COEOGLevice_Impl::GetDeviceParam(void* pData, const tstring& strParamName)
{
	return false;
}
