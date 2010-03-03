/*!
 * \file OpenGLModule.h
 * \date 24-5-2009 10:55:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OPENGLMODULE_H__
#define __OPENGLMODULE_H__

#include <OEHolder.h>

extern "C" __declspec(dllexport) bool OEModuleInit(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleTerm(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleSyncInterfaces(COEHolder& Holder);

#endif // __OPENGLMODULE_H__
