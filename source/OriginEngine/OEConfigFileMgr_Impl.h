/*!
 * \file OEConfigFileMgr_Impl.h
 * \date 26-7-2009 9:03:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OECONFIGFILEMGR_IMPL_H__
#define __OECONFIGFILEMGR_IMPL_H__

#include <IOEConfigFileMgr.h>
#include <OEInterfaces.h>

class COEConfigFileMgr_Impl : public IOEConfigFileMgr
{
public:
	COEConfigFileMgr_Impl();
	virtual ~COEConfigFileMgr_Impl();

	virtual bool GetValue(int& nValue, const tstring& strParamName, int nDefaultValue = 0);
	virtual bool GetValue(float& fValue, const tstring& strParamName, float fDefaultValue = 0.0f);
	virtual bool GetValue(tstring& strValue, const tstring& strParamName, const tstring& strDefaultValue = EMPTY_STRING);

private:
	void Init();
	void Destroy();

	bool LoadConfigFile(const tstring& strFileName);

private:
	tstring m_strConfigFile;
	IOEXmlNode* m_pDocument;
	IOEXmlNode* m_pRootNode;

};

#endif // __OECONFIGFILEMGR_IMPL_H__
