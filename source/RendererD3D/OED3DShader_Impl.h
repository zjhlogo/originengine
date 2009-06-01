/*!
 * \file OED3DShader_Impl.h
 * \date 1-6-2009 15:47:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DSHADER_IMPL_H__
#define __OED3DSHADER_IMPL_H__

#include <IOEShader.h>
#include "OED3DVertDecl_Impl.h"

#include <d3dx9.h>

class COED3DShader_Impl : public IOEShader
{
public:
	COED3DShader_Impl(const IOEVertDecl::ELEMENT* pElement, const tchar* pstrFileName);
	virtual ~COED3DShader_Impl();

	virtual bool SetInt(const tchar* pstrParamName, int nValue);
	virtual bool GetInt(int& nOut, const tchar* pstrParamName);

	virtual bool SetFloat(const tchar* pstrParamName, float fValue);
	virtual bool GetFloat(float& fOut, const tchar* pstrParamName);

	virtual bool SetVector(const tchar* pstrParamName, const CVector4& vIn);
	virtual bool GetVector(CVector4& vOut, const tchar* pstrParamName);

	virtual bool SetMatrix(const tchar* pstrParamName, const CMatrix4x4& matIn);
	virtual bool GetMatrix(CMatrix4x4& matOut, const tchar* pstrParamName);

	virtual bool SetTexture(const tchar* pstrParamName, IOETexture* pTexture);
	virtual bool SetTechnique(const tchar* pstrParamName);

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);

private:
	void Init();
	void Destroy();

	bool Create(const IOEVertDecl::ELEMENT* pElement, const tchar* pstrFileName);

private:
	COED3DVertDecl_Impl* m_pDecl;
	ID3DXEffect* m_pEffect;

};

#endif // __OED3DSHADER_IMPL_H__
