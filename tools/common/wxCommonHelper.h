/*!
 * \file wxCommonHelper.h
 * \date 3-3-2010 15:39:07
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __WXCOMMONHELPER_H__
#define __WXCOMMONHELPER_H__

#include <OEBaseType.h>
#include <windows.h>

class wxCommonHelper
{
public:
	static bool Initialize();
	static void Terminate();

	static bool InitializeXrc();

	static bool AddMemoryXrc(const tstring& strResType, uint nResID, const tstring& strMemoryFileName, HINSTANCE hInstance = NULL);

};
#endif // __WXCOMMONHELPER_H__
