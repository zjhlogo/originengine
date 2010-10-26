/*!
 * \file OEUIResMgr_Impl.h
 * \date 10-26-2010 0:06:04
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEUIRESMGR_IMPL_H__
#define __OEUIRESMGR_IMPL_H__

#include <OEUI/IOEUIResMgr.h>
#include <map>

class COEUIResMgr_Impl : public IOEUIResMgr
{
public:
	typedef std::map<tstring, IOEUIFont*> TM_FONT;

public:
	RTTI_DEF(COEUIResMgr_Impl, IOEUIResMgr);

	COEUIResMgr_Impl();
	virtual ~COEUIResMgr_Impl();

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
#endif // __OEUIRESMGR_IMPL_H__
