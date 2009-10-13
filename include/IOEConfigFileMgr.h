/*!
 * \file IOEConfigFileMgr.h
 * \date 26-7-2009 8:57:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOECONFIGFILEMGR_H__
#define __IOECONFIGFILEMGR_H__

#include "IOEObject.h"

class IOEConfigFileMgr : public IOEObject
{
public:
	IOEConfigFileMgr() {};
	virtual ~IOEConfigFileMgr() {};

	virtual bool GetValue(int& nValue, const tstring& strParamName, int nDefaultValue = 0) = 0;
	virtual bool GetValue(float& fValue, const tstring& strParamName, float fDefaultValue = 0.0f) = 0;
	virtual bool GetValue(tstring& strValue, const tstring& strParamName, const tstring& strDefaultValue = EMPTY_STRING) = 0;

};

extern IOEConfigFileMgr* g_pOEConfigFileMgr;

#endif // __IOECONFIGFILEMGR_H__
