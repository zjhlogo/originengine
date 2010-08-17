/*!
 * \file OECoreModule.h
 * \date 3-3-2010 10:58:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OECOREMODULE_H__
#define __OECOREMODULE_H__

#include <libOEBase/OEHolder.h>

extern "C" __declspec(dllexport) bool OEModuleInit(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleTerm(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleSyncInterfaces(COEHolder& Holder);
#endif // __OECOREMODULE_H__
