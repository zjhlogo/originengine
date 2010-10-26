/*!
 * \file OEUIStringRenderer_Impl.h
 * \date 10-26-2010 0:02:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEUISTRINGRENDERER_IMPL_H__
#define __OEUISTRINGRENDERER_IMPL_H__

#include <OEUI/IOEUIStringRenderer.h>
#include <vector>

class COEUIStringRenderer_Impl : public IOEUIStringRenderer
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
	RTTI_DEF(COEUIStringRenderer_Impl, IOEUIStringRenderer);

	COEUIStringRenderer_Impl(IOEUIFont* pFont);
	virtual ~COEUIStringRenderer_Impl();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual void SetText(const tstring& strText);
	virtual const tstring& GetText() const;

	virtual void SetTextColor(uint nColor);
	virtual uint GetTextColor() const;

	virtual bool SetFont(IOEUIFont* pFont);
	virtual const IOEUIFont* GetFont() const;

	virtual void SetPosition(const CPoint& pos);
	virtual const CPoint& GetPosition();

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
	CPoint m_vPos;

};
#endif // __OEUISTRINGRENDERER_IMPL_H__
