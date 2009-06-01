/*!
*	created:	2009/06/01
*	filename: 	OriginEngine\OEFileMgr_Impl.h
*	author:		daishuidi
	
*	purpose:	
*/

#ifndef __OEFILEMGR_IMPL_H__
#define __OEFILEMGR_IMPL_H__
#include <IOEFileMgr.h>

class COEFileMgr_Impl : public IOEFileMgr
{
public:
	COEFileMgr_Impl();
	virtual ~COEFileMgr_Impl();
	virtual IOEFile* OpenFile(const tchar* pstrFileName, uint nFlag);
private:
};
#endif