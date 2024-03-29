/*!
 *	created:	2009/06/01
 *	filename: 	OriginEngine\OEFileMgr_Impl.h
 *	author:		daishuidi
	
 *	purpose:	
 */
#ifndef __OEFILEMGR_IMPL_H__
#define __OEFILEMGR_IMPL_H__

#include <OEBase/IOEFileMgr.h>

class COEFileMgr_Impl : public IOEFileMgr
{
public:
	RTTI_DEF(COEFileMgr_Impl, IOEFileMgr);

	COEFileMgr_Impl();
	virtual ~COEFileMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual IOEFile* OpenFile(const tstring& strFileName, uint nFlag = IOEFile::OFF_READ);

private:
	bool Init();
	void Destroy();

};

#endif // __OEFILEMGR_IMPL_H__
