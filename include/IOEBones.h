/*!
 * \file IOEBones.h
 * \date 1-3-2010 14:51:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEBONES_H__
#define __IOEBONES_H__

#include "IOEObject.h"
#include <IOEBone.h>

class IOEBones : public IOEObject
{
public:
	IOEBones() {};
	virtual ~IOEBones() {};

	virtual int GetBonesCount() = 0;
	virtual IOEBone* GetBone(int nIndex) = 0;

};

#endif // __IOEBONES_H__
