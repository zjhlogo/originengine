/*!
 * \file IOESkeleton.h
 * \date 1-3-2010 19:18:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOESKELETON_H__
#define __IOESKELETON_H__

#include "../libOEBase/IOEObject.h"
#include "IOEBone.h"

class IOESkeleton : public IOEObject
{
public:
	RTTI_DEF(IOESkeleton, IOEObject);

	IOESkeleton() {};
	virtual ~IOESkeleton() {};

	virtual int GetBonesCount() = 0;
	virtual IOEBone* GetBone(int nIndex) = 0;

};
#endif // __IOESKELETON_H__
