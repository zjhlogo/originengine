/*!
 * \file OEModelMgr_Impl.h
 * \date 9-8-2009 23:42:29
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODELMGR_IMPL_H__
#define __OEMODELMGR_IMPL_H__

#include <IOEModelMgr.h>

class COEModelMgr_Impl : public IOEModelMgr
{
public:
	COEModelMgr_Impl();
	virtual ~COEModelMgr_Impl();

	virtual IOEModel* CreateModelFromFile(const tstring& strFileName);

private:
	void Init();
	void Destroy();

};

#endif // __OEMODELMGR_IMPL_H__
