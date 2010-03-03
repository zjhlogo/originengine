/*!
 * \file OEUIFontMgr_Impl.h
 * \date 27-7-2009 21:58:14
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUIFONTMGR_IMPL_H__
#define __OEUIFONTMGR_IMPL_H__

#include <OEUI/IOEUIFontMgr.h>
#include <map>

class COEUIFontMgr_Impl : public IOEUIFontMgr
{
public:
	typedef std::map<tstring, IOEUIFont*> TM_FONT;

public:
	COEUIFontMgr_Impl();
	virtual ~COEUIFontMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual IOEUIFont* CreateFont(const tstring& strFile);
	virtual IOEUIFont* CreateBitmapFont(const tstring& strFile);

	virtual void SetDefaultDir(const tstring& strDir);
	virtual const tstring& GetDefaultDir();

private:
	bool Init();
	void Destroy();

	bool GetFontFilePath(tstring& strFilePathOut, const tstring& strFile);

private:
	TM_FONT m_FontMap;
	tstring m_strDefaultDir;

};
#endif // __OEUIFONTMGR_IMPL_H__
