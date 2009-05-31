/*!
 * \file OEOSUnix.cpp
 * \date 24-5-2009 10:00:18
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEOS.h"
#include <OEHolder.h>

#define MODULE_INIT_FUNC_NAME _T("OEModuleInit")
#define MODULE_TERM_FUNC_NAME _T("OEModuleTerm")

typedef bool (*FUNC_MODULE_INIT)(COEHolder& Holder);
typedef void (*FUNC_MODULE_TERM)(COEHolder& Holder);

const char* COEOS::tchar2char(const tchar* pstrIn)
{
	// TODO: 
}

const tchar* COEOS::char2tchar(const char* pstrIn)
{
	// TODO: 
}

COEOS::OEMODULE COEOS::LoadOEModule(const tchar* pstrModuleName)
{
	// TODO: 
}

void COEOS::FreeOEModule(OEMODULE hModule)
{
	// TODO: 
}
