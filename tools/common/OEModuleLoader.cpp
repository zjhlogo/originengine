/*!
 * \file OEModuleLoader.cpp
 * \date 3-3-2010 16:58:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModuleLoader.h"
#include <OEBase/IOEFileMgr.h>
#include <OEBase/IOELogFileMgr.h>
#include <OEBase/IOEXmlMgr.h>
#include <OEBase/IOEMsgMgr.h>
#include <libOEBase/OEOS.h>

static COEOS::OEMODULE g_hModuleBase = NULL;
static COEHolder g_OEHolder;

bool COEModuleLoader::Initialize(HINSTANCE hInst /* = NULL */)
{
	tchar szFilePath[MAX_PATH];
	::GetModuleFileName(hInst, szFilePath, MAX_PATH);

	tstring strFileDir;
	COEOS::GetFileDir(strFileDir, szFilePath);

	tstring strFile;
	COEOS::strformat(strFile, TS("%s\\%s"), strFileDir.c_str(), MODULE_BASE);

	// load base module
	g_hModuleBase = COEOS::LoadOEModule(strFile.c_str(), g_OEHolder);
	if (!g_hModuleBase) return false;

	COEOS::SyncModuleInterfaces(g_hModuleBase, g_OEHolder);

	// 文件管理类
	if (!g_pOEFileMgr->Initialize()) return false;

	// 日志管理类
	if (!g_pOELogFileMgr->Initialize()) return false;

	// Xml 管理类
	if (!g_pOEXmlMgr->Initialize()) return false;

	// 消息管理类
	if (!g_pOEMsgMgr->Initialize()) return false;

	return true;
}

void COEModuleLoader::Terminate()
{
	// 消息管理类
	g_pOEMsgMgr->Terminate();

	// Xml 管理类
	g_pOEXmlMgr->Terminate();

	// 日志管理类
	g_pOELogFileMgr->Terminate();

	// 文件管理类
	g_pOEFileMgr->Terminate();

	// free base module
	COEOS::FreeOEModule(g_hModuleBase, g_OEHolder);
	g_hModuleBase = NULL;
}
