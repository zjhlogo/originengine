/*!
 * \file IOETexture.h
 * \date 1-6-2009 14:17:46
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOETEXTURE_H__
#define __IOETEXTURE_H__

#include "OEBaseTypeEx.h"
#include "../libOEBase/IOEObject.h"

class IOETexture : public IOEObject
{
public:
	IOETexture(const tstring& strClassName):IOEObject(strClassName) {};
	virtual ~IOETexture() {};

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual TEXTURE_FORMAT GetFormat() const = 0;

};

#endif // __IOETEXTURE_H__
