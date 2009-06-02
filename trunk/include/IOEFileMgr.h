/*!
 * \file IOEFileMgr.h
 * \date 1-6-2009 9:07:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEFILEMGR_H__
#define __IOEFILEMGR_H__

#include "IOEFile.h"

class IOEFileMgr
{
public:
	IOEFileMgr() {};
	virtual ~IOEFileMgr() {};

	/*!
	 * \brief 
	 * \param pstrFileName 
	 * \param nFlag can be combine of OPEN_FILE_FLAG
	 * \return 
	 */
	virtual IOEFile* OpenFile(const tchar* pstrFileName, uint nFlag = IOEFile::OFF_READ) = 0;

};
extern IOEFileMgr* g_pOEFileMgr;
#endif // __IOEFILEMGR_H__
