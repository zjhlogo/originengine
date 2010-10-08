/*!
 * \file IOEUIString.h
 * \date 27-7-2009 17:50:41
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEUISTRING_H__
#define __IOEUISTRING_H__

#include "../libOEUI/OEUIType.h"
#include "IOEUIRenderableObject.h"
#include "IOEUIFont.h"

class IOEUIString : public IOEUIRenderableObject
{
public:
	RTTI_DEF(IOEUIString, IOEUIRenderableObject);

	IOEUIString() {};
	virtual ~IOEUIString() {};

	virtual void SetText(const tstring& strText) = 0;
	virtual const tstring& GetText() const = 0;

	virtual void SetTextColor(uint nColor) = 0;
	virtual uint GetTextColor() const = 0;

	virtual bool SetFont(IOEUIFont* pFont) = 0;
	virtual const IOEUIFont* GetFont() const = 0;

	virtual void SetPosition(const CPoint& pos) = 0;
	virtual const CPoint& GetPosition() = 0;

};
#endif // __IOEUISTRING_H__
