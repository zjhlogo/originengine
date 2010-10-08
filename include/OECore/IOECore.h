/*!
 * \file IOECore.h
 * \date 24-5-2009 9:20:11
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOECORE_H__
#define __IOECORE_H__

#include "IOEMgr.h"
#include "IOENode.h"

class IOECore : public IOEMgr
{
public:
	RTTI_DEF(IOECore, IOEMgr);

	IOECore() {};
	virtual ~IOECore() {};

	virtual void Run() = 0;
	virtual void End() = 0;

	virtual IOENode* GetRootNode() = 0;

};

extern IOECore* g_pOECore;

#endif // __IOECORE_H__
