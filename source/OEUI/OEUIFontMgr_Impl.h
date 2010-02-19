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
	typedef std::map<tstring, IOEUIFont*> UIFONT_MAP;

public:
	COEUIFontMgr_Impl();
	virtual ~COEUIFontMgr_Impl();

	virtual IOEUIFont* CreateFont(const tstring& strFileName);
	virtual IOEUIFont* CreateBitmapFont(const tstring& strFileName);

private:
	bool Init();
	void Destroy();

private:
	UIFONT_MAP m_UIFontMap;

};
#endif // __OEUIFONTMGR_IMPL_H__
