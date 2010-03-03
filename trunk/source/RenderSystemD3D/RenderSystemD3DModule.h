/*!
 * \file RenderSystemD3DModule.h
 * \date 3-3-2010 11:00:15
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __RENDERSYSTEMD3DMODULE_H__
#define __RENDERSYSTEMD3DMODULE_H__

#include <OEHolder.h>

extern "C" __declspec(dllexport) bool OEModuleInit(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleTerm(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleSyncInterfaces(COEHolder& Holder);
#endif // __RENDERSYSTEMD3DMODULE_H__
