/*!
 * \file OED3DShader_Impl.cpp
 * \date 1-6-2009 15:49:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DShader_Impl.h"
#include "OED3DTexture_Impl.h"
#include "OED3DUtil.h"

#include <IOEDevice.h>
#include <IOELogFileMgr.h>
#include <OEOS.h>

extern IDirect3DDevice9* g_pd3dDevice;

COED3DShader_Impl::COED3DShader_Impl(const VERT_DECL_ELEMENT* pElement, const tstring& strFileName)
{
	Init();
	m_bOK = Create(pElement, strFileName);
}

COED3DShader_Impl::~COED3DShader_Impl()
{
	Destroy();
}

void COED3DShader_Impl::Init()
{
	m_pDecl = NULL;
	m_pEffect = NULL;
}

void COED3DShader_Impl::Destroy()
{
	SAFE_RELEASE(m_pDecl);
	SAFE_RELEASE(m_pEffect);
}

bool COED3DShader_Impl::SetInt(const tstring& strParamName, int nValue)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	if (FAILED(m_pEffect->SetInt(strANSIName.c_str(), nValue))) return false;
	return true;
}

bool COED3DShader_Impl::GetInt(int& nOut, const tstring& strParamName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	if (FAILED(m_pEffect->GetInt(strANSIName.c_str(), &nOut))) return false;
	return true;
}

bool COED3DShader_Impl::SetFloat(const tstring& strParamName, float fValue)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	if (FAILED(m_pEffect->SetFloat(strANSIName.c_str(), fValue))) return false;
	return true;
}

bool COED3DShader_Impl::GetFloat(float& fOut, const tstring& strParamName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	if (FAILED(m_pEffect->GetFloat(strANSIName.c_str(), &fOut))) return false;
	return true;
}

bool COED3DShader_Impl::SetVector(const tstring& strParamName, const CVector4& vIn)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	D3DXVECTOR4 vD3D;
	COED3DUtil::ToD3DVector4(vD3D, vIn);

	if (FAILED(m_pEffect->SetVector(strANSIName.c_str(), &vD3D))) return false;
	return true;
}

bool COED3DShader_Impl::SetVector(const tstring& strParamName, const CVector3& vIn)
{
	CVector4 vVec(vIn.x, vIn.y, vIn.z, 0.0f);
	return SetVector(strParamName, vVec);
}

bool COED3DShader_Impl::GetVector(CVector4& vOut, const tstring& strParamName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	D3DXVECTOR4 vD3D;
	if (FAILED(m_pEffect->GetVector(strANSIName.c_str(), &vD3D))) return false;

	COED3DUtil::ToOEVector4(vOut, vD3D);
	return true;
}

bool COED3DShader_Impl::GetVector(CVector3& vOut, const tstring& strParamName)
{
	CVector4 vVec;
	if (!GetVector(vVec, strParamName)) return false;

	vOut.x = vVec.x;
	vOut.y = vVec.y;
	vOut.z = vVec.z;
	return true;
}

bool COED3DShader_Impl::SetMatrix(const tstring& strParamName, const CMatrix4x4& matIn)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	D3DXMATRIX matD3D;
	COED3DUtil::ToD3DXMatrix(matD3D, matIn);

	if (FAILED(m_pEffect->SetMatrix(strANSIName.c_str(), &matD3D))) return false;
	return true;
}

bool COED3DShader_Impl::GetMatrix(CMatrix4x4& matOut, const tstring& strParamName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	D3DXMATRIX matD3D;
	if (FAILED(m_pEffect->GetMatrix(strANSIName.c_str(), &matD3D))) return false;

	COED3DUtil::ToOEMatrix(matOut, matD3D);
	return true;
}

bool COED3DShader_Impl::SetMatrixArray(const tstring& strParamName, const CMatrix4x4* pmatIn, uint nCount)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	D3DXMATRIX matD3D;
	if (FAILED(m_pEffect->SetMatrixArray(strANSIName.c_str(), (const D3DXMATRIX*)pmatIn, nCount))) return false;

	return true;
}

bool COED3DShader_Impl::GetMatrixArray(CMatrix4x4* pmatOut, uint nCount, const tstring& strParamName)
{
	// TODO: 
	return false;
}

bool COED3DShader_Impl::SetTexture(const tstring& strParamName, IOETexture* pTexture)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	COED3DTexture_Impl* pD3DTexture = (COED3DTexture_Impl*)pTexture;
	if (FAILED(m_pEffect->SetTexture(strANSIName.c_str(), pD3DTexture->GetTexture())))
	{
		LOGOUT(TS("IOEShader::SetTexture Failed \"%s\""), strParamName.c_str());
		return false;
	}
	return true;
}

bool COED3DShader_Impl::SetTechnique(const tstring& strParamName)
{
	std::string strANSIName;
	if (!COEOS::tchar2char(strANSIName, strParamName.c_str())) return false;

	if (FAILED(m_pEffect->SetTechnique(strANSIName.c_str())))
	{
		LOGOUT(TS("IOEShader::SetTechnique Failed \"%s\""), strParamName.c_str());
		return false;
	}

	return true;
}

COED3DVertDecl_Impl* COED3DShader_Impl::GetVertDecl() const
{
	return m_pDecl;
}

ID3DXEffect* COED3DShader_Impl::GetEffect() const
{
	return m_pEffect;
}

bool COED3DShader_Impl::Create(const VERT_DECL_ELEMENT* pElement, const tstring& strFileName)
{
	m_pDecl = (COED3DVertDecl_Impl*)g_pOEDevice->CreateVertDecl(pElement);
	if (!m_pDecl) return false;

	// create effect
	ID3DXBuffer* pErrorBuffer = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(g_pd3dDevice, strFileName.c_str(), NULL, NULL, NULL, NULL, &m_pEffect, &pErrorBuffer);

	if (pErrorBuffer)
	{
		tstring strMsg;
		COEOS::char2tchar(strMsg, (char*)pErrorBuffer->GetBufferPointer());
		LOGOUT(TS("COED3DShader_Impl::Create Failed \"%s\""), strMsg.c_str());
		SAFE_RELEASE(pErrorBuffer);
	}

	if (FAILED(hr)) return false;

	D3DXHANDLE hTechnique = m_pEffect->GetTechnique(0);
	if (hTechnique)
	{
		hr = m_pEffect->SetTechnique(hTechnique);
		if (FAILED(hr)) return false;
	}

	return true;
}
