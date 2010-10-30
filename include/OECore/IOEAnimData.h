/*!
 * \file IOEAnimData.h
 * \date 10-29-2010 18:42:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOEANIMDATA_H__
#define __IOEANIMDATA_H__

#include "../libOEBase/IOEObject.h"
#include "IOESkeleton.h"

class IOEAnimData : public IOEObject
{
public:
	RTTI_DEF(IOEAnimData, IOEObject);

	IOEAnimData() {};
	virtual ~IOEAnimData() {};

	virtual IOESkeleton* GetSkeleton() = 0;
	virtual float GetAnimLength() = 0;

	virtual void SetCurrTime(float fCurrTime) = 0;
	virtual float GetCurrTime() = 0;

	virtual CMatrix4x4* GetSkinMatrixs() = 0;
	virtual int GetNumSkinMatrixs() = 0;

};
#endif // __IOEANIMDATA_H__
