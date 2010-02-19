/*!
 * \file IOEShader.h
 * \date 1-6-2009 15:42:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOESHADER_H__
#define __IOESHADER_H__

#include "IOEObject.h"
#include "OEMath/OEMath.h"

#include "IOETexture.h"
#include "IOEVertDecl.h"

class IOEShader : public IOEObject
{
public:
	IOEShader() {};
	virtual ~IOEShader() {};

	virtual bool SetInt(const tstring& strParamName, int nValue) = 0;
	virtual bool GetInt(int& nOut, const tstring& strParamName) = 0;

	virtual bool SetFloat(const tstring& strParamName, float fValue) = 0;
	virtual bool GetFloat(float& fOut, const tstring& strParamName) = 0;

	virtual bool SetVector(const tstring& strParamName, const CVector4& vIn) = 0;
	virtual bool SetVector(const tstring& strParamName, const CVector3& vIn) = 0;
	virtual bool GetVector(CVector4& vOut, const tstring& strParamName) = 0;
	virtual bool GetVector(CVector3& vOut, const tstring& strParamName) = 0;

	virtual bool SetMatrix(const tstring& strParamName, const CMatrix4x4& matIn) = 0;
	virtual bool GetMatrix(CMatrix4x4& matOut, const tstring& strParamName) = 0;
	virtual bool SetMatrixArray(const tstring& strParamName, const CMatrix4x4* pmatIn, uint nCount) = 0;
	virtual bool GetMatrixArray(CMatrix4x4* pmatOut, uint nCount, const tstring& strParamName) = 0;

	virtual bool SetTexture(const tstring& strParamName, IOETexture* pTexture) = 0;
	virtual bool SetTechnique(const tstring& strParamName) = 0;

	virtual IOEVertDecl* GetVertDecl() = 0;

};

#endif // __IOESHADER_H__
