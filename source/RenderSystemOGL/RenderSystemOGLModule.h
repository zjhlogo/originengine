/*!
 * \file RenderSystemOGLModule.h
 * \date 3-3-2010 11:00:51
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __RENDERSYSTEMOGLMODULE_H__
#define __RENDERSYSTEMOGLMODULE_H__

#include <libOEBase/OEHolder.h>

extern "C" __declspec(dllexport) bool OEModuleInit(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleTerm(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleSyncInterfaces(COEHolder& Holder);
#endif // __RENDERSYSTEMOGLMODULE_H__
