/*!
 * \file OED3DShaderMgr_Impl.cpp
 * \date 1-6-2009 16:41:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DShaderMgr_Impl.h"
#include "OED3DShader_Impl.h"

COED3DShaderMgr_Impl::COED3DShaderMgr_Impl()
{
	g_pOEShaderMgr = this;
	Init();
}

COED3DShaderMgr_Impl::~COED3DShaderMgr_Impl()
{
	Destroy();
	g_pOEShaderMgr = NULL;
}

void COED3DShaderMgr_Impl::Init()
{
	// TODO: 
}

void COED3DShaderMgr_Impl::Destroy()
{
	// TODO: 
}

IOEShader* COED3DShaderMgr_Impl::CreateShader(const IOEVertDecl::ELEMENT* pElement, const tchar* pstrFileName)
{
	COED3DShader_Impl* pShader = new COED3DShader_Impl(pElement, pstrFileName);
	if (!pShader || !pShader->IsOK())
	{
		SAFE_RELEASE(pShader);
		// TODO: logout
		return NULL;
	}

	return pShader;
}
