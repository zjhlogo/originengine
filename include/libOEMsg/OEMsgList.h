/*!
 * \file OEMsgList.h
 * \date 10-24-2010 8:35:5
 * 
 * 
 * \author MessageGenerator (zjhlogo@gmail.com)
 */
#ifndef __OEMSGLIST_H__
#define __OEMSGLIST_H__

#include <OEBase/IOEMsgMgr.h>

enum OEMSGLIST_ID_DEFINE
{
	OEMSGLIST_ID_MIN = 1000, 
	OMI_OBJECT_DESTROY,          // COEMsgObjectDestroy
	OMI_OBJECT_ATTACH,           // COEMsgObjectAttach
	OMI_LBUTTON_DOWN,            // COEMsgMouse
	OMI_LBUTTON_UP,              // COEMsgMouse
	OMI_MBUTTON_DOWN,            // COEMsgMouse
	OMI_MBUTTON_UP,              // COEMsgMouse
	OMI_RBUTTON_DOWN,            // COEMsgMouse
	OMI_RBUTTON_UP,              // COEMsgMouse
	OMI_MOUSE_MOVE,              // COEMsgMouse
	OMI_MOUSE_WHEEL,             // COEMsgMouse
	OMI_KEY_DOWN,                // COEMsgKeyboard
	OMI_KEY_UP,                  // COEMsgKeyboard
	OMI_CHAR,                    // COEMsgKeyboard
	OMI_START_PERFORM,           // COEMsgCommand
	OMI_PRE_UPDATE,              // COEMsgCommand
	OMI_UPDATE,                  // COEMsgCommand
	OMI_POST_UPDATE,             // COEMsgCommand
	OMI_PRE_RENDER,              // COEMsgCommand
	OMI_RENDER,                  // COEMsgCommand
	OMI_POST_RENDER,             // COEMsgCommand
	OMI_UI_PRE_UPDATE,           // COEMsgCommand
	OMI_UI_UPDATE,               // COEMsgCommand
	OMI_UI_POST_UPDATE,          // COEMsgCommand
	OMI_UI_PRE_RENDER,           // COEMsgCommand
	OMI_UI_RENDER,               // COEMsgCommand
	OMI_UI_POST_RENDER,          // COEMsgCommand
	OMI_SETUP_SHADER_PARAM,      // COEMsgShaderParam
};

extern const IOEMsgMgr::MSG_GENERATE_MAP g_OEMsgListDB[28];

#endif // __OEMSGLIST_H__
