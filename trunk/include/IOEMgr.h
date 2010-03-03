/*!
 * \file IOEMgr.h
 * \date 3-3-2010 9:32:15
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMGR_H__
#define __IOEMGR_H__

#include "IOEObject.h"

class IOEMgr : public IOEObject
{
public:
	IOEMgr() {};
	virtual ~IOEMgr() {};

	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

};
#endif // __IOEMGR_H__
