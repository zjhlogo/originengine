/*!
 * \file IOEBone.h
 * \date 10-23-2009 13:54:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEBONE_H__
#define __IOEBONE_H__

#include "IOEObject.h"
#include "OEMath/OEMath.h"

class IOEBone : public IOEObject
{
public:
	IOEBone() {};
	virtual ~IOEBone() {};

	virtual const tstring& GetName() const = 0;
	virtual int GetID() const = 0;
	virtual int GetParentID() const = 0;

	virtual float GetTimeLength() const = 0;
	virtual const CMatrix4x4& GetLocalMatrix() const = 0;
	virtual const CMatrix4x4& GetWorldMatrix() const = 0;
	virtual const CMatrix4x4& GetWorldMatrixInv() const = 0;

	virtual bool SlerpMatrix(CMatrix4x4& matOut, float fTime, bool bLoop = true) = 0;

};
#endif // __IOEBONE_H__
