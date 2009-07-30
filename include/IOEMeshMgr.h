/*!
 * \file IOEMeshMgr.h
 * \date 30-7-2009 10:05:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMESHMGR_H__
#define __IOEMESHMGR_H__

#include "IOEMesh.h"

class IOEMeshMgr
{
public:
	IOEMeshMgr() {};
	virtual ~IOEMeshMgr() {};

	virtual IOEMesh* CreateMeshFromFile(const tstring& strFileName) = 0;

};

extern IOEMeshMgr* g_pOEMeshMgr;

#endif // __IOEMESHMGR_H__
