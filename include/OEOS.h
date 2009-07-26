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

	static bool tchar2char(std::string& strOut, const tchar* pstrIn);
	static bool char2tchar(tstring& strOut, const char* pstrIn);

	static bool str2int(int& nValue, const tchar* pstrIn);
	static bool str2float(float& fValue, const tchar* pstrIn);

	static bool int2str(tstring& strOut, int nValue);
	static bool float2str(tstring& strOut, float fValue);

	static bool strformat(tstring& strOut, const tchar* strFormat, ...);
};

#endif // __OEOS_H__
