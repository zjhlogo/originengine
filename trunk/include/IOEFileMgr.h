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
	enum OPEN_FILE_FLAG
	{
		OFF_READ	= 0x00000001,
		OFF_WRITE	= 0x00000002,
	};

public:
	IOEFileMgr() {};
	virtual ~IOEFileMgr() {};

	/*!
	 * \brief 
	 * \param pstrFileName 
	 * \param nFlag can be combine of OPEN_FILE_FLAG
	 * \return 
	 */
	virtual IOEFile* OpenFile(const tchar* pstrFileName, uint nFlag) = 0;

};
#endif // __IOEFILEMGR_H__
