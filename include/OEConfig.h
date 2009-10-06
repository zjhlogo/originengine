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
			#define MODULE_ORIGINENGINE t("OriginEngine_UD.dll")
			#define MODULE_RENDERER t("RendererD3D_UD.dll")
			#define MODULE_UI t("OEUI_UD.dll")
		#else
			#define MODULE_ORIGINENGINE t("OriginEngine_D.dll")
			#define MODULE_RENDERER t("RendererD3D_D.dll")
			#define MODULE_UI t("OEUI_D.dll")
		#endif // _UNICODE
	#else
		#ifdef _UNICODE
			#define MODULE_ORIGINENGINE t("OriginEngineU.dll")
			#define MODULE_RENDERER t("RendererD3DU.dll")
			#define MODULE_UI t("OEUIU.dll")
		#else
			#define MODULE_ORIGINENGINE t("OriginEngine.dll")
			#define MODULE_RENDERER t("RendererD3D.dll")
			#define MODULE_UI t("OEUI.dll")
		#endif // _UNICODE
	#endif // _DEBUG

#else

// Linux 平台下

#endif // WIN32

#define ENABLE_LOGOUT 1

#endif // __OECONFIG_H__
