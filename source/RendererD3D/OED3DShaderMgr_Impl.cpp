/*!
 * \file OED3DShaderMgr_Impl.cpp
 * \date 1-6-2009 16:41:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DShaderMgr_Impl.h"
#include "OED3DShader_Impl.h"

#include <algorithm>

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
	m_ShaderMap.clear();
}

void COED3DShaderMgr_Impl::Destroy()
{
	// TODO: check m_ShaderMap whether is empty, and logout
}

IOEShader* COED3DShaderMgr_Impl::CreateShader(const IOEVertDecl::ELEMENT* pElement, const tchar* pstrFileName)
{
	// transform string to lower
	tstring strFileName = pstrFileName;
	std::transform(strFileName.begin(), strFileName.end(), strFileName.begin(), tolower);

	// find from map, if exist just increase reference and return
	SHADER_MAP::iterator itfound = m_ShaderMap.find(strFileName);
	if (itfound != m_ShaderMap.end())
	{
		IOEShader* pShader = itfound->second;
		pShader->IncRef();
		return pShader;
	}

	COED3DShader_Impl* pShader = new COED3DShader_Impl(pElement, pstrFileName);
	if (!pShader || !pShader->IsOK())
	{
		SAFE_RELEASE(pShader);
		// TODO: logout
		return NULL;
	}

	m_ShaderMap.insert(std::make_pair(strFileName, pShader));

	return pShader;
}