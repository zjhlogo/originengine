/*!
 * \file OEConfigFileMgr_Impl.h
 * \date 26-7-2009 9:03:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OECONFIGFILEMGR_IMPL_H__
#define __OECONFIGFILEMGR_IMPL_H__

#include <OECore/IOEConfigFileMgr.h>
#include <OEBase/IOEXmlDocument.h>

class COEConfigFileMgr_Impl : public IOEConfigFileMgr
{
public:
	RTTI_DEF(COEConfigFileMgr_Impl, IOEConfigFileMgr);

	COEConfigFileMgr_Impl();
	virtual ~COEConfigFileMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual bool GetValue(int& nValue, const tstring& strParamName, int nDefaultValue = 0);
	virtual bool GetValue(float& fValue, const tstring& strParamName, float fDefaultValue = 0.0f);
	virtual bool GetValue(tstring& strValue, const tstring& strParamName, const tstring& strDefaultValue = EMPTY_STRING);

private:
	bool Init();
	void Destroy();

	bool LoadConfigFile(const tstring& strFileName);

private:
	tstring m_strConfigFile;
	IOEXmlDocument* m_pXmlDocument;

};

#endif // __OECONFIGFILEMGR_IMPL_H__
