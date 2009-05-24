/*!
 * \file IOECore.h
 * \date 24-5-2009 9:20:11
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOECORE_H__
#define __IOECORE_H__

#include "OEBasicType.h"

class IOECore
{
public:
	IOECore() {};
	virtual ~IOECore() {};

	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

	virtual void Run() = 0;
	virtual void End() = 0;

};

extern IOECore* g_pOECore;

#endif // __IOECORE_H__
