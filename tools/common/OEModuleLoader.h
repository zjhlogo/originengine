/*!
 * \file OEModuleLoader.h
 * \date 3-3-2010 16:58:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODULELOADER_H__
#define __OEMODULELOADER_H__

#include <windows.h>

class COEModuleLoader
{
public:
	static bool Initialize(HINSTANCE hInst = NULL);
	static void Terminate();

};
#endif // __OEMODULELOADER_H__
