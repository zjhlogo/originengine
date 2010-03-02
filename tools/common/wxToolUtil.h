/*!
 * \file wxToolUtil.h
 * \date 1-3-2010 12:02:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __WXTOOLUTIL_H__
#define __WXTOOLUTIL_H__

#include <OEBaseType.h>
#include <windows.h>

class wxToolUtil
{
public:
	static bool AddMemoryXrc(const tstring& strResType, uint nResID, const tstring& strMemoryFileName, HINSTANCE hInstance = NULL);

private:
	static bool InitFileHandler();

private:
	static bool m_bOK;

};
#endif // __WXTOOLUTIL_H__
