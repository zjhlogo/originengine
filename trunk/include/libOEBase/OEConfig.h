/*!
 * \file OEConfig.h
 * \date 24-5-2009 9:21:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OECONFIG_H__
#define __OECONFIG_H__

// Windows 平台下
#ifdef WIN32

	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0600
	#endif

	#ifdef _DEBUG
		#ifdef _UNICODE
			#define MODULE_BASE TS("OEBase_UD.dll")
			#define MODULE_CORE TS("OECore_UD.dll")
			#define MODULE_RENDERSYSTEM TS("RenderSystemD3D_UD.dll")
			#define MODULE_UI TS("OEUI_UD.dll")
		#else
			#define MODULE_BASE TS("OEBase_D.dll")
			#define MODULE_CORE TS("OECore_D.dll")
			#define MODULE_RENDERSYSTEM TS("RenderSystemD3D_D.dll")
			#define MODULE_UI TS("OEUI_D.dll")
		#endif // _UNICODE
	#else
		#ifdef _UNICODE
			#define MODULE_BASE TS("OEBaseU.dll")
			#define MODULE_CORE TS("OECoreU.dll")
			#define MODULE_RENDERSYSTEM TS("RenderSystemD3DU.dll")
			#define MODULE_UI TS("OEUIU.dll")
		#else
			#define MODULE_BASE TS("OEBase.dll")
			#define MODULE_CORE TS("OECore.dll")
			#define MODULE_RENDERSYSTEM TS("RenderSystemD3D.dll")
			#define MODULE_UI TS("OEUI.dll")
		#endif // _UNICODE
	#endif // _DEBUG

#else

// Linux 平台下

#endif // WIN32

#define ENABLE_LOGOUT 1

#endif // __OECONFIG_H__
