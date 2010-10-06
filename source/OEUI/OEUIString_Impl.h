/*!
 * \file OEUIString_Impl.h
 * \date 28-7-2009 8:49:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUISTRING_IMPL_H__
#define __OEUISTRING_IMPL_H__

#include <OEUI/IOEUIString.h>
#include <vector>

class COEUIString_Impl : public IOEUIString
{
public:
	typedef struct CHAR_RENDER_INFO_tag
	{
		float x;
		float y;
		const IOEUIFont::CHAR_INFO* pCharInfo;
	} CHAR_RENDER_INFO;

	typedef std::vector<CHAR_RENDER_INFO> VCHAR_RENDER_INFO;

public:
	RTTI_DEF(COEUIString_Impl, IOEUIString);

	COEUIString_Impl(IOEUIFont* pFont);
	virtual ~COEUIString_Impl();

	virtual void SetText(const tstring& strText);
	virtual const tstring& GetText() const;

	virtual void SetTextColor(uint nColor);
	virtual uint GetTextColor() const;

	virtual bool SetFont(IOEUIFont* pFont);
	virtual const IOEUIFont* GetFont() const;

	virtual void Render(const CPoint& pos);

private:
	void Init();
	void Destroy();

	bool CreateRenderChars();
	void DestroyRenderChars();

private:
	IOEUIFont* m_pFont;
	tstring m_strText;
	uint m_nColor;
	VCHAR_RENDER_INFO m_vCharRenderInfo;

};
#endif // __OEUISTRING_IMPL_H__
