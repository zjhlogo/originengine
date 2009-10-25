/*!
 * \file OELogFileMgr_Impl.h
 * \date 26-7-2009 10:32:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OELOGFILEMGR_IMPL_H__
#define __OELOGFILEMGR_IMPL_H__

#include <IOELogFileMgr.h>

#include <IOEFile.h>

class COELogFileMgr_Impl : public IOELogFileMgr
{
public:
	COELogFileMgr_Impl();
	virtual ~COELogFileMgr_Impl();

	virtual void SetLogFile(const tstring& strFile);
	virtual void LogOut(const tstring& strLogMsg);
	virtual tstring& GetStringBuffer();

private:
	void Init();
	void Destroy();

	bool CreateLogFile(const tstring& strFileName);

private:
	tstring m_strFile;
	tstring m_strBuffer;
	IOEFile* m_pLogFile;
	bool m_bFirstCreate;

};
#endif // __OELOGFILEMGR_IMPL_H__
