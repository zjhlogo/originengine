/*!
 * \file D3DModule.h
 * \date 24-5-2009 10:41:56
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __D3DMODULE_H__
#define __D3DMODULE_H__

#include <OEHolder.h>

extern "C" __declspec(dllexport) bool OEModuleInit(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleTerm(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleSyncInterfaces(COEHolder& Holder);

#endif // __D3DMODULE_H__
