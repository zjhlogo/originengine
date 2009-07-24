/*!
 * \file OEOS.h
 * \date 24-5-2009 9:57:58
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEOS_H__
#define __OEOS_H__

#include "OEBasicType.h"

class COEOS
{
public:
	typedef void* OEMODULE;

public:
	static OEMODULE LoadOEModule(const tchar* pstrModuleName);
	static void FreeOEModule(OEMODULE hModule);

	static const char* tchar2char_Fast(const tchar* pstrIn);
	static bool char2tchar(tstring& strOut, const char* pstrIn);
};

#endif // __OEOS_H__
