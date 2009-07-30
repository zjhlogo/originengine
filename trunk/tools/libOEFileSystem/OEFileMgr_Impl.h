/*!
 * \file OEFileMgr_Impl.h
 * \date 30-7-2009 17:07:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEFILEMGR_IMPL_H__
#define __OEFILEMGR_IMPL_H__

#include <IOEFileMgr.h>

class COEFileMgr_Impl : public IOEFileMgr
{
public:
	COEFileMgr_Impl();
	virtual ~COEFileMgr_Impl();
	virtual IOEFile* OpenFile(const tstring& strFileName, uint nFlag = IOEFile::OFF_READ);

private:
	void Init();
	void Destroy();

};

#endif // __OEFILEMGR_IMPL_H__
