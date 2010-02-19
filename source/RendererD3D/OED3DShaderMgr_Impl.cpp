/*!
 * \file OED3DShaderMgr_Impl.cpp
 * \date 1-6-2009 16:41:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DShaderMgr_Impl.h"
#include "OED3DShader_Impl.h"

#include <IOELogFileMgr.h>
#include <algorithm>

COED3DShaderMgr_Impl::COED3DShaderMgr_Impl()
{
	g_pOEShaderMgr = this;
	m_bOK = Init();
}

COED3DShaderMgr_Impl::~COED3DShaderMgr_Impl()
{
	Destroy();
	g_pOEShaderMgr = NULL;
}

bool COED3DShaderMgr_Impl::Init()
{
	m_ShaderMap.clear();
	memset(m_pDefaultShader, 0, sizeof(m_pDefaultShader));

	return true;
}

void COED3DShaderMgr_Impl::Destroy()
{
	// TODO: check m_ShaderMap whether is empty, and logout
	// TODO: check m_DefaultShaderMap whether is empty, and logout
}

IOEShader* COED3DShaderMgr_Impl::CreateShader(const VERT_DECL_ELEMENT* pElement, const tstring& strFile)
{
	// transform string to lower
	tstring strLowName = strFile;
	std::transform(strLowName.begin(), strLowName.end(), strLowName.begin(), tolower);

	// find it first
	IOEShader* pShaderFound = FindShader(strLowName);
	if (pShaderFound)
	{
		pShaderFound->IncRef();
		return pShaderFound;
	}

	// not found, create new
	COED3DShader_Impl* pShader = new COED3DShader_Impl(pElement, strLowName);
	if (!pShader || !pShader->IsOK())
	{
		LOGOUT(TS("IOEShaderMgr::CreateShader Failed \"%s\""), strFile.c_str());
		SAFE_RELEASE(pShader);
		return NULL;
	}

	m_ShaderMap.insert(std::make_pair(strLowName, pShader));
	return pShader;
}

IOEShader* COED3DShaderMgr_Impl::CreateDefaultShader(DEFAULT_SHADER_TYPE eType)
{
	// find it first
	IOEShader* pShaderFound = FindDefaultShader(eType);
	if (pShaderFound)
	{
		pShaderFound->IncRef();
		return pShaderFound;
	}

	// not found, create new
	const VERT_DECL_ELEMENT* pElement = GetDefaultVertDecl(eType);
	if (!pElement) return NULL;

	tstring strFile;
	if (!GetDefaultShaderFile(strFile, eType)) return NULL;

	COED3DShader_Impl* pShader = new COED3DShader_Impl(pElement, strFile);
	if (!pShader || !pShader->IsOK())
	{
		LOGOUT(TS("IOEShaderMgr::CreateDefaultShader Failed \"%s\""), strFile.c_str());
		SAFE_RELEASE(pShader);
		return NULL;
	}

	m_pDefaultShader[eType] = pShader;
	return m_pDefaultShader[eType];
}

IOEShader* COED3DShaderMgr_Impl::FindShader(const tstring& strLowerFile)
{
	SHADER_MAP::iterator itfound = m_ShaderMap.find(strLowerFile);
	if (itfound != m_ShaderMap.end()) return itfound->second;

	return NULL;
}

const VERT_DECL_ELEMENT* COED3DShaderMgr_Impl::GetDefaultVertDecl(DEFAULT_SHADER_TYPE eType)
{
	static const VERT_DECL_ELEMENT s_DeclColor[] =
	{
		VDT_FLOAT3, VDU_POSITION, 0,
		VDT_COLOR, VDU_COLOR, 0,
		VDT_UNKNOWN, VDU_UNKNOWN, 0,
	};

	static const VERT_DECL_ELEMENT s_DeclTexture[] =
	{
		VDT_FLOAT3, VDU_POSITION, 0,
		VDT_FLOAT2, VDU_TEXCOORD, 0,
		VDT_UNKNOWN, VDU_UNKNOWN, 0,
	};

	static const VERT_DECL_ELEMENT s_DeclUI[] =
	{
		VDT_FLOAT4, VDU_POSITIONT, 0,
		VDT_COLOR, VDU_COLOR, 0,
		VDT_FLOAT2, VDU_TEXCOORD, 0,
		VDT_UNKNOWN, VDU_UNKNOWN, 0,
	};

	switch (eType)
	{
	case DST_LINE:
	case DST_POLYC:
		{
			return s_DeclColor;
		}
		break;
	case DST_POLYT:
		{
			return s_DeclTexture;
		}
		break;
	case DST_POLY_UI:
		{
			return s_DeclUI;
		}
		break;
	}

	return NULL;
}

bool COED3DShaderMgr_Impl::GetDefaultShaderFile(tstring& strOut, DEFAULT_SHADER_TYPE eType)
{
	switch (eType)
	{
	case DST_LINE:
		{
			strOut = TS("line.fx");
		}
		break;
	case DST_POLYC:
		{
			strOut = TS("poly_color.fx");
		}
		break;
	case DST_POLYT:
		{
			strOut = TS("poly_texture.fx");
		}
		break;
	case DST_POLY_UI:
		{
			strOut = TS("poly_ui.fx");
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return true;
}

IOEShader* COED3DShaderMgr_Impl::FindDefaultShader(DEFAULT_SHADER_TYPE eType)
{
	if (eType < 0 && eType >= DST_NUM) return NULL;
	return m_pDefaultShader[eType];
}
