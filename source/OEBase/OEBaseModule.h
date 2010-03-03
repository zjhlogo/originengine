/*!
 * \file OEBaseModule.h
 * \date 3-3-2010 7:42:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBASEMODULE_H__
#define __OEBASEMODULE_H__

#include <OEHolder.h>

extern "C" __declspec(dllexport) bool OEModuleInit(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleTerm(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleSyncInterfaces(COEHolder& Holder);

#endif // __OEBASEMODULE_H__
