/*!
 * \file OEModule.h
 * \date 24-5-2009 10:25:15
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODULE_H__
#define __OEMODULE_H__

#include <OEHolder.h>

extern "C" __declspec(dllexport) bool OEModuleInit(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleTerm(COEHolder& Holder);

#endif // __OEMODULE_H__
