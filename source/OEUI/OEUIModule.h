/*!
 * \file OEUIModule.h
 * \date 27-7-2009 17:51:31
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUIMODULE_H__
#define __OEUIMODULE_H__

#include <OEHolder.h>

extern "C" __declspec(dllexport) bool OEModuleInit(COEHolder& Holder);
extern "C" __declspec(dllexport) void OEModuleTerm(COEHolder& Holder);

#endif // __OEUIMODULE_H__
