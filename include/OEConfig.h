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
			#define MODULE_ORIGINENGINE _T("OriginEngine_UD.dll")
			#define MODULE_RENDERER _T("RendererD3D_UD.dll")
		#else
			#define MODULE_ORIGINENGINE _T("OriginEngine_D.dll")
			#define MODULE_RENDERER _T("RendererD3D_D.dll")
		#endif // _UNICODE
	#else
		#ifdef _UNICODE
			#define MODULE_ORIGINENGINE _T("OriginEngineU.dll")
			#define MODULE_RENDERER _T("RendererD3DU.dll")
		#else
			#define MODULE_ORIGINENGINE _T("OriginEngine.dll")
			#define MODULE_RENDERER _T("RendererD3D.dll")
		#endif // _UNICODE
	#endif // _DEBUG

#else

// Linux 平台下

#endif // WIN32

#endif // __OECONFIG_H__
