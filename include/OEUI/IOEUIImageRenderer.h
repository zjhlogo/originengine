/*!
 * \file IOEUIImageRenderer.h
 * \date 10-26-2010 0:10:21
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOEUIIMAGERENDERER_H__
#define __IOEUIIMAGERENDERER_H__

#include "../libOEUI/OEUIType.h"
#include "../OECore/IOETexture.h"
#include "IOEUIRenderableObject.h"

class IOEUIImageRenderer : public IOEUIRenderableObject
{
public:
	RTTI_DEF(IOEUIImageRenderer, IOEUIRenderableObject);

	IOEUIImageRenderer() {};
	virtual ~IOEUIImageRenderer() {};

	virtual void SetImage(IOETexture* pTexture, bool bAdjustSizeFromImage = false) = 0;
	virtual IOETexture* GetImage() = 0;

	virtual void SetPosition(const CPoint& pos) = 0;
	virtual const CPoint& GetPosition() = 0;

	virtual void SetSize(const CSize& size) = 0;
	virtual const CSize& GetSize() = 0;

};
#endif // __IOEUIIMAGERENDERER_H__
