/*!
 * \file OEMsgList.cpp
 * \date 10-8-2010 23:32:24
 * 
 * 
 * \author MessageGenerator (zjhlogo@gmail.com)
 */
#include "OEMsgList.h"

#include "OEMsgDestroy.h"
#include "OEMsgMouse.h"
#include "OEMsgKeyboard.h"
#include "OEMsgCommand.h"
#include "OEMsgShaderParam.h"

static COEMsgDestroy* __AutoGen_COEMsgDestroy(COEDataBufferRead* pDBRead)
{
	return new COEMsgDestroy(pDBRead);
}

static COEMsgMouse* __AutoGen_COEMsgMouse(COEDataBufferRead* pDBRead)
{
	return new COEMsgMouse(pDBRead);
}

static COEMsgKeyboard* __AutoGen_COEMsgKeyboard(COEDataBufferRead* pDBRead)
{
	return new COEMsgKeyboard(pDBRead);
}

static COEMsgCommand* __AutoGen_COEMsgCommand(COEDataBufferRead* pDBRead)
{
	return new COEMsgCommand(pDBRead);
}

static COEMsgShaderParam* __AutoGen_COEMsgShaderParam(COEDataBufferRead* pDBRead)
{
	return new COEMsgShaderParam(pDBRead);
}

const IOEMsgMgr::MSG_GENERATE_MAP g_OEMsgListDB[27] = 
{
	{OMI_OBJECT_DESTROY,          (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgDestroy},
	{OMI_LBUTTON_DOWN,            (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_LBUTTON_UP,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_MBUTTON_DOWN,            (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_MBUTTON_UP,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_RBUTTON_DOWN,            (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_RBUTTON_UP,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_MOUSE_MOVE,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_MOUSE_WHEEL,             (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_KEY_DOWN,                (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgKeyboard},
	{OMI_KEY_UP,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgKeyboard},
	{OMI_CHAR,                    (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgKeyboard},
	{OMI_START_PERFORM,           (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_PRE_UPDATE,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UPDATE,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_POST_UPDATE,             (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_PRE_RENDER,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_RENDER,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_POST_RENDER,             (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_PRE_UPDATE,           (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_UPDATE,               (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_POST_UPDATE,          (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_PRE_RENDER,           (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_RENDER,               (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_POST_RENDER,          (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_SETUP_SHADER_PARAM,      (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgShaderParam},
	{0,                           (IOEMsgMgr::MSG_GENERATE_FUNC)NULL}
};
