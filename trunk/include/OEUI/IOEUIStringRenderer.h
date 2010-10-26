/*!
 * \file IOEUIStringRenderer.h
 * \date 10-26-2010 0:01:13
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOEUISTRINGRENDERER_H__
#define __IOEUISTRINGRENDERER_H__

#include "../libOEUI/OEUIType.h"
#include "IOEUIRenderableObject.h"
#include "IOEUIFont.h"

class IOEUIStringRenderer : public IOEUIRenderableObject
{
public:
	RTTI_DEF(IOEUIStringRenderer, IOEUIRenderableObject);

	IOEUIStringRenderer() {};
	virtual ~IOEUIStringRenderer() {};

	virtual void SetText(const tstring& strText) = 0;
	virtual const tstring& GetText() const = 0;

	virtual void SetTextColor(uint nColor) = 0;
	virtual uint GetTextColor() const = 0;

	virtual bool SetFont(IOEUIFont* pFont) = 0;
	virtual const IOEUIFont* GetFont() const = 0;

	virtual void SetPosition(const CPoint& pos) = 0;
	virtual const CPoint& GetPosition() = 0;

};
#endif // __IOEUISTRINGRENDERER_H__
