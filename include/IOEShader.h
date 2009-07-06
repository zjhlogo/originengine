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
#include "IOETexture.h"
#include "OEMath/OEMath.h"

class IOEShader : public IOEObject
{
public:
	IOEShader() {};
	virtual ~IOEShader() {};

	virtual bool SetInt(const tchar* pstrParamName, int nValue) = 0;
	virtual bool GetInt(int& nOut, const tchar* pstrParamName) = 0;

	virtual bool SetFloat(const tchar* pstrParamName, float fValue) = 0;
	virtual bool GetFloat(float& fOut, const tchar* pstrParamName) = 0;

	virtual bool SetVector(const tchar* pstrParamName, const CVector4& vIn) = 0;
	virtual bool SetVector(const tchar* pstrParamName, const CVector3& vIn) = 0;
	virtual bool GetVector(CVector4& vOut, const tchar* pstrParamName) = 0;
	virtual bool GetVector(CVector3& vOut, const tchar* pstrParamName) = 0;

	virtual bool SetMatrix(const tchar* pstrParamName, const CMatrix4x4& matIn) = 0;
	virtual bool GetMatrix(CMatrix4x4& matOut, const tchar* pstrParamName) = 0;

	virtual bool SetTexture(const tchar* pstrParamName, IOETexture* pTexture) = 0;
	virtual bool SetTechnique(const tchar* pstrParamName) = 0;

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriStrip(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;
	virtual void DrawTriFan(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

};

#endif // __IOESHADER_H__
