/*!
 * \file OEMsgID.h
 * \date 11-25-2009 14:05:36
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMSGID_H__
#define __OEMSGID_H__

enum OE_MSG_ID
{
	OMI_UNKNOWN = 0,

	//
	// device input
	//
	// mouse event message
	OMI_LBUTTON_DOWN,
	OMI_LBUTTON_UP,
	OMI_MBUTTON_DOWN,
	OMI_MBUTTON_UP,
	OMI_RBUTTON_DOWN,
	OMI_RBUTTON_UP,
	OMI_MOUSE_MOVE,
	OMI_MOUSE_WHEEL,
	// keyboard event message
	OMI_KEY_DOWN,
	OMI_KEY_UP,
	OMI_CHAR,

	//
	// device message
	//
	OMI_START_PERFORM,
	OMI_PRE_UPDATE,
	OMI_UPDATE,
	OMI_POST_UPDATE,
	OMI_PRE_RENDER_3D,
	OMI_RENDER_3D,
	OMI_POST_RENDER_3D,
	OMI_PRE_RENDER_2D,
	OMI_RENDER_2D,
	OMI_POST_RENDER_2D,

	//
	// model render message
	//
	OMI_SETUP_SHADER_PARAM,

	OMI_USER = 10000,
};

#endif // __OEMSGID_H__
