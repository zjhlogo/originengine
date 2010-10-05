/*!
 * \file OED3DShaderMgr_Impl.cpp
 * \date 1-6-2009 16:41:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DShaderMgr_Impl.h"
#include "OED3DShader_Impl.h"

#include <OEBase/IOELogFileMgr.h>
#include <OECore/IOEConfigFileMgr.h>
#include <algorithm>
#include <assert.h>

COED3DShaderMgr_Impl::COED3DShaderMgr_Impl()
:IOEShaderMgr(TS("COED3DShaderMgr_Impl"))
{
	assert(!g_pOEShaderMgr);
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
	memset(m_pDefaultShader, 0, sizeof(m_pDefaultShader));
	return true;
}

void COED3DShaderMgr_Impl::Destroy()
{
	// TODO: 
}

bool COED3DShaderMgr_Impl::Initialize()
{
	tstring strDefaultShaderDir;
	g_pOEConfigFileMgr->GetValue(strDefaultShaderDir, TS("DefaultShaderDir"), TS("shader"));
	SetDefaultDir(strDefaultShaderDir);

	return true;
}

void COED3DShaderMgr_Impl::Terminate()
{
	// TODO: check m_ShaderMap whether is empty, and logout
	// TODO: check m_DefaultShaderMap whether is empty, and logout
}

IOEShader* COED3DShaderMgr_Impl::CreateShader(const VERT_DECL_ELEMENT* pElement, const tstring& strFile)
{
	// transform string to lower
	tstring strFilePath;
	if (!GetShaderFilePath(strFilePath, strFile)) return NULL;

	// find it first
	IOEShader* pShaderFound = FindShader(strFilePath);
	if (pShaderFound)
	{
		pShaderFound->IncRef();
		return pShaderFound;
	}

	// not found, create new
	COED3DShader_Impl* pShader = new COED3DShader_Impl(pElement, strFilePath);
	if (!pShader || !pShader->IsOK())
	{
		LOGOUT(TS("IOEShaderMgr::CreateShader Failed \"%s\""), strFilePath.c_str());
		SAFE_RELEASE(pShader);
		return NULL;
	}

	m_ShaderMap.insert(std::make_pair(strFilePath, pShader));
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

	tstring strFilePath;
	if (!GetDefaultShaderFile(strFilePath, eType)) return NULL;

	COED3DShader_Impl* pShader = new COED3DShader_Impl(pElement, strFilePath);
	if (!pShader || !pShader->IsOK())
	{
		LOGOUT(TS("IOEShaderMgr::CreateDefaultShader Failed \"%s\""), strFilePath.c_str());
		SAFE_RELEASE(pShader);
		return NULL;
	}

	m_pDefaultShader[eType] = pShader;
	return m_pDefaultShader[eType];
}

void COED3DShaderMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& COED3DShaderMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

IOEShader* COED3DShaderMgr_Impl::FindShader(const tstring& strFilePath)
{
	SHADER_MAP::iterator itfound = m_ShaderMap.find(strFilePath);
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
	static const tstring s_DefaultShader[DST_NUM] =
	{
		TS(""),
		TS("line.fx"),
		TS("poly_color.fx"),
		TS("poly_texture.fx"),
		TS("poly_ui.fx"),
	};
	assert(DST_NUM == 5);

	if (eType <= 0 || eType >= DST_NUM) return false;
	return GetShaderFilePath(strOut, s_DefaultShader[eType]);
}

IOEShader* COED3DShaderMgr_Impl::FindDefaultShader(DEFAULT_SHADER_TYPE eType)
{
	if (eType <= 0 && eType >= DST_NUM) return NULL;
	return m_pDefaultShader[eType];
}

bool COED3DShaderMgr_Impl::GetShaderFilePath(tstring& strFilePathOut, const tstring& strFile)
{
	strFilePathOut = m_strDefaultDir + TS("\\") + strFile;
	COEOS::tolower(strFilePathOut, strFilePathOut);
	return true;
}
