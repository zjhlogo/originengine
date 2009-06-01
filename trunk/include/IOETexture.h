/*!
 * \file IOETexture.h
 * \date 1-6-2009 14:17:46
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOETEXTURE_H__
#define __IOETEXTURE_H__

#include "IOEObject.h"

class IOETexture : public IOEObject
{
public:
	enum TEXTURE_FORMAT
	{
		TF_UNKNOWN,
		TF_A8R8G8B8,
		TF_A4R4G4B4,
		TF_MAX,
	};

public:
	IOETexture() {};
	virtual ~IOETexture() {};

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual TEXTURE_FORMAT GetFormat() const = 0;

};

#endif // __IOETEXTURE_H__
