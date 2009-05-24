/*!
 * \file OED3DUtil.h
 * \date 24-5-2009 18:03:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DUTIL_H__
#define __OED3DUTIL_H__

#include <IOEVertDecl.h>
#include <d3d9.h>

class COED3DUtil
{
public:
	static D3DDECLTYPE ToD3DVertType(IOEVertDecl::TYPE eType);
	static IOEVertDecl::TYPE ToOEVertType(D3DDECLTYPE eType);

	static D3DDECLUSAGE ToD3DVertUsage(IOEVertDecl::USAGE eUsage);
	static IOEVertDecl::USAGE ToOEVertUsage(D3DDECLUSAGE eUsage);

	static int GetVertTypeSize(IOEVertDecl::TYPE eType);

};

#endif // __OED3DUTIL_H__
