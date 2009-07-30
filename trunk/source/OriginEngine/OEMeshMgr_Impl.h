/*!
 * \file OEMeshMgr_Impl.h
 * \date 30-7-2009 10:20:00
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMESHMGR_IMPL_H__
#define __OEMESHMGR_IMPL_H__

#include <IOEMeshMgr.h>

class COEMeshMgr_Impl : public IOEMeshMgr
{
public:
	COEMeshMgr_Impl();
	virtual ~COEMeshMgr_Impl();

	virtual IOEMesh* CreateMeshFromFile(const tstring& strFileName);

private:
	void Init();
	void Destroy();

private:

};

#endif // __OEMESHMGR_IMPL_H__
