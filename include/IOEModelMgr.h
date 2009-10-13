/*!
 * \file IOEModelMgr.h
 * \date 9-8-2009 17:46:12
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMODELMGR_H__
#define __IOEMODELMGR_H__

#include "IOEObject.h"
#include "IOEModel.h"

class IOEModelMgr : public IOEObject
{
public:
	IOEModelMgr() {};
	virtual ~IOEModelMgr() {};

	virtual IOEModel* CreateModelFromFile(const tstring& strFileName) = 0;

};

extern IOEModelMgr* g_pOEModelMgr;

#endif // __IOEMODELMGR_H__
