/*!
 * \file wxInitHelper.h
 * \date 28-11-2009 9:25:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __WXINITHELPER_H__
#define __WXINITHELPER_H__

#include <libOEBase/OEBaseType.h>
#include <windows.h>

class wxInitHelper
{
public:
	static bool Initialize();
	static void Uninitialize();

	static bool AddMemoryXrc(const tstring& strResType, uint nResID, const tstring& strMemoryFileName, HINSTANCE hInstance = NULL);

};
#endif // __WXINITHELPER_H__
