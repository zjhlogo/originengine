/*!
 * \file OEUIFont_Impl.h
 * \date 27-7-2009 17:57:10
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUIFONT_IMPL_H__
#define __OEUIFONT_IMPL_H__

#include <OEUI/IOEUIFont.h>

class COEUIFont_Impl : public IOEUIFont
{
public:
	COEUIFont_Impl();
	virtual ~COEUIFont_Impl();

	virtual float GetLineHeight() const;
	virtual const CHAR_INFO* GetCharInfo(int nID) const;
	virtual float GetKerning(int nFirstID, int nSecondID) const;

private:
	bool Init();
	void Destroy();

};
#endif // __OEUIFONT_IMPL_H__
