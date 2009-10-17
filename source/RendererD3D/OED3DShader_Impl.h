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
	COED3DShader_Impl(const IOEVertDecl::ELEMENT* pElement, const tstring& strFileName);
	virtual ~COED3DShader_Impl();

	virtual bool SetInt(const tstring& strParamName, int nValue);
	virtual bool GetInt(int& nOut, const tstring& strParamName);

	virtual bool SetFloat(const tstring& strParamName, float fValue);
	virtual bool GetFloat(float& fOut, const tstring& strParamName);

	virtual bool SetVector(const tstring& strParamName, const CVector4& vIn);
	virtual bool SetVector(const tstring& strParamName, const CVector3& vIn);
	virtual bool GetVector(CVector4& vOut, const tstring& strParamName);
	virtual bool GetVector(CVector3& vOut, const tstring& strParamName);

	virtual bool SetMatrix(const tstring& strParamName, const CMatrix4x4& matIn);
	virtual bool GetMatrix(CMatrix4x4& matOut, const tstring& strParamName);
	virtual bool SetMatrixArray(const tstring& strParamName, const CMatrix4x4* pmatIn, uint nCount);
	virtual bool GetMatrixArray(CMatrix4x4* pmatOut, uint nCount, const tstring& strParamName);

	virtual bool SetTexture(const tstring& strParamName, IOETexture* pTexture);
	virtual bool SetTechnique(const tstring& strParamName);

	COED3DVertDecl_Impl* GetVertDecl() const;
	ID3DXEffect* GetEffect() const;

private:
	void Init();
	void Destroy();

	bool Create(const IOEVertDecl::ELEMENT* pElement, const tstring& strFileName);

private:
	COED3DVertDecl_Impl* m_pDecl;
	ID3DXEffect* m_pEffect;

};

#endif // __OED3DSHADER_IMPL_H__
