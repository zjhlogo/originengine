/*!
 * \file IOEUIFont.h
 * \date 27-7-2009 17:49:03
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEUIFONT_H__
#define __IOEUIFONT_H__

#include "../libOEUI/OEUIType.h"
#include "../OECore/IOEObject.h"
#include "../OECore/IOETexture.h"

class IOEUIFont : public IOEObject
{
public:
	typedef struct CHAR_INFO_tag
	{
		int nID;
		float x;
		float y;
		float width;
		float height;
		float u;
		float v;
		float w;
		float h;
		IOETexture* pTexture;
		float fOffsetX;
		float fOffsetY;
		float fAdvance;
		int nRef;
	} CHAR_INFO;

public:
	IOEUIFont() {};
	virtual ~IOEUIFont() {};

	virtual float GetLineHeight() const = 0;
	virtual const CHAR_INFO* GetCharInfo(int nID) const = 0;
	virtual float GetKerning(int nFirstID, int nSecondID) const = 0;

};
#endif // __IOEUIFONT_H__
