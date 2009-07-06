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

#include <OEInterfaces.h>
#include <OEOS.h>

extern IDirect3DDevice9* g_pd3dDevice;

COED3DShader_Impl::COED3DShader_Impl(const IOEVertDecl::ELEMENT* pElement, const tchar* pstrFileName)
{
	Init();
	m_bOK = Create(pElement, pstrFileName);
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

bool COED3DShader_Impl::SetInt(const tchar* pstrParamName, int nValue)
{
	if (FAILED(m_pEffect->SetInt(COEOS::tchar2char(pstrParamName), nValue))) return false;
	return true;
}

bool COED3DShader_Impl::GetInt(int& nOut, const tchar* pstrParamName)
{
	if (FAILED(m_pEffect->GetInt(COEOS::tchar2char(pstrParamName), &nOut))) return false;
	return true;
}

bool COED3DShader_Impl::SetFloat(const tchar* pstrParamName, float fValue)
{
	if (FAILED(m_pEffect->SetFloat(COEOS::tchar2char(pstrParamName), fValue))) return false;
	return true;
}

bool COED3DShader_Impl::GetFloat(float& fOut, const tchar* pstrParamName)
{
	if (FAILED(m_pEffect->GetFloat(COEOS::tchar2char(pstrParamName), &fOut))) return false;
	return true;
}

bool COED3DShader_Impl::SetVector(const tchar* pstrParamName, const CVector4& vIn)
{
	D3DXVECTOR4 vD3D;
	COED3DUtil::ToD3DVector4(vD3D, vIn);

	if (FAILED(m_pEffect->SetVector(COEOS::tchar2char(pstrParamName), &vD3D))) return false;
	return true;
}

bool COED3DShader_Impl::SetVector(const tchar* pstrParamName, const CVector3& vIn)
{
	CVector4 vVec(vIn.x, vIn.y, vIn.z, 0.0f);
	return SetVector(pstrParamName, vVec);
}

bool COED3DShader_Impl::GetVector(CVector4& vOut, const tchar* pstrParamName)
{
	D3DXVECTOR4 vD3D;
	if (FAILED(m_pEffect->GetVector(COEOS::tchar2char(pstrParamName), &vD3D))) return false;

	COED3DUtil::ToOEVector4(vOut, vD3D);
	return true;
}

bool COED3DShader_Impl::GetVector(CVector3& vOut, const tchar* pstrParamName)
{
	CVector4 vVec;
	if (!GetVector(vVec, pstrParamName)) return false;

	vOut.x = vVec.x;
	vOut.y = vVec.y;
	vOut.z = vVec.z;
	return true;
}

bool COED3DShader_Impl::SetMatrix(const tchar* pstrParamName, const CMatrix4x4& matIn)
{
	D3DXMATRIX matD3D;
	COED3DUtil::ToD3DXMatrix(matD3D, matIn);

	if (FAILED(m_pEffect->SetMatrix(COEOS::tchar2char(pstrParamName), &matD3D))) return false;
	return true;
}

bool COED3DShader_Impl::GetMatrix(CMatrix4x4& matOut, const tchar* pstrParamName)
{
	D3DXMATRIX matD3D;
	if (FAILED(m_pEffect->GetMatrix(COEOS::tchar2char(pstrParamName), &matD3D))) return false;

	COED3DUtil::ToOEMatrix(matOut, matD3D);
	return true;
}

bool COED3DShader_Impl::SetTexture(const tchar* pstrParamName, IOETexture* pTexture)
{
	COED3DTexture_Impl* pD3DTexture = (COED3DTexture_Impl*)pTexture;
	if (FAILED(m_pEffect->SetTexture(COEOS::tchar2char(pstrParamName), pD3DTexture->GetTexture())))
	{
		// TODO: logout
		return false;
	}
	return true;
}

bool COED3DShader_Impl::SetTechnique(const tchar* pstrParamName)
{
	if (FAILED(m_pEffect->SetTechnique(COEOS::tchar2char(pstrParamName))))
	{
		// TODO: logout
		return false;
	}

	return true;
}

void COED3DShader_Impl::DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	uint nPass = 0;
	m_pEffect->Begin(&nPass, 0);
	for (uint i = 0; i < nPass; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pEffect->CommitChanges();

		g_pOERenderer->SetVertDecl(m_pDecl);
		g_pOERenderer->DrawTriList(pVerts, nVerts, pIndis, nIndis);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
}

void COED3DShader_Impl::DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	uint nPass = 0;
	m_pEffect->Begin(&nPass, 0);
	for (uint i = 0; i < nPass; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pEffect->CommitChanges();

		g_pOERenderer->SetVertDecl(m_pDecl);
		g_pOERenderer->DrawTriStrip(pVerts, nVerts, pIndis, nIndis);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
}

void COED3DShader_Impl::DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	uint nPass = 0;
	m_pEffect->Begin(&nPass, 0);
	for (uint i = 0; i < nPass; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pEffect->CommitChanges();

		g_pOERenderer->SetVertDecl(m_pDecl);
		g_pOERenderer->DrawTriFan(pVerts, nVerts, pIndis, nIndis);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
}

bool COED3DShader_Impl::Create(const IOEVertDecl::ELEMENT* pElement, const tchar* pstrFileName)
{
	m_pDecl = (COED3DVertDecl_Impl*)g_pOEDevice->CreateVertDecl(pElement);
	if (!m_pDecl) return false;

	// create effect
	ID3DXBuffer* pErrorBuffer = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(g_pd3dDevice, pstrFileName, NULL, NULL, NULL, NULL, &m_pEffect, &pErrorBuffer);

	if (pErrorBuffer)
	{
		const tchar* pstrMsg = COEOS::char2tchar((char*)pErrorBuffer->GetBufferPointer());
		// TODO: logout
		MessageBox(NULL, pstrMsg, NULL, MB_OK);
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
