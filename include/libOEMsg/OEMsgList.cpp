/*!
 * \file OEMsgList.cpp
 * \date 10-29-2010 9:35:37
 * 
 * 
 * \author MessageGenerator (zjhlogo@gmail.com)
 */
#include "OEMsgList.h"

#include "OEMsgObjectDestroy.h"
#include "OEMsgObjectAttach.h"
#include "OEMsgMouse.h"
#include "OEMsgKeyboard.h"
#include "OEMsgCommand.h"
#include "OEMsgShaderParam.h"
#include "OEMsgTransform.h"
#include "OEMsgRenderState.h"

static COEMsgObjectDestroy* __AutoGen_COEMsgObjectDestroy(COEDataBufferRead* pDBRead)
{
	return new COEMsgObjectDestroy(pDBRead);
}

static COEMsgObjectAttach* __AutoGen_COEMsgObjectAttach(COEDataBufferRead* pDBRead)
{
	return new COEMsgObjectAttach(pDBRead);
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

static COEMsgTransform* __AutoGen_COEMsgTransform(COEDataBufferRead* pDBRead)
{
	return new COEMsgTransform(pDBRead);
}

static COEMsgRenderState* __AutoGen_COEMsgRenderState(COEDataBufferRead* pDBRead)
{
	return new COEMsgRenderState(pDBRead);
}

const IOEMsgMgr::MSG_GENERATE_MAP g_OEMsgListDB[30] = 
{
	{OMI_OBJECT_DESTROY,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgObjectDestroy},
	{OMI_OBJECT_ATTACH,               (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgObjectAttach},
	{OMI_LBUTTON_DOWN,                (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_LBUTTON_UP,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_MBUTTON_DOWN,                (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_MBUTTON_UP,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_RBUTTON_DOWN,                (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_RBUTTON_UP,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_MOUSE_MOVE,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_MOUSE_WHEEL,                 (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgMouse},
	{OMI_KEY_DOWN,                    (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgKeyboard},
	{OMI_KEY_UP,                      (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgKeyboard},
	{OMI_CHAR,                        (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgKeyboard},
	{OMI_START_PERFORM,               (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_PRE_UPDATE,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UPDATE,                      (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_POST_UPDATE,                 (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_PRE_RENDER,                  (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_RENDER,                      (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_POST_RENDER,                 (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_PRE_UPDATE,               (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_UPDATE,                   (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_POST_UPDATE,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_PRE_RENDER,               (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_RENDER,                   (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_UI_POST_RENDER,              (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgCommand},
	{OMI_SETUP_SHADER_PARAM,          (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgShaderParam},
	{OMI_GET_TRANSFORM,               (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgTransform},
	{OMI_RESTORE_RENDER_STATE,        (IOEMsgMgr::MSG_GENERATE_FUNC)__AutoGen_COEMsgRenderState},
	{0,                               (IOEMsgMgr::MSG_GENERATE_FUNC)NULL}
};
