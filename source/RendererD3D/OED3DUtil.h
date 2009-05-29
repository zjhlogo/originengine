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
#include <IOERenderer.h>
#include <OEMath/OEMath.h>
#include <d3dx9.h>

class COED3DUtil
{
public:
	static D3DDECLTYPE ToD3DVertType(IOEVertDecl::TYPE eType);
	static IOEVertDecl::TYPE ToOEVertType(D3DDECLTYPE eType);

	static D3DDECLUSAGE ToD3DVertUsage(IOEVertDecl::USAGE eUsage);
	static IOEVertDecl::USAGE ToOEVertUsage(D3DDECLUSAGE eUsage);

	static int GetVertTypeSize(IOEVertDecl::TYPE eType);

	static D3DTRANSFORMSTATETYPE ToD3DTransformType(IOERenderer::TRANSFORM_TYPE eType);
	static IOERenderer::TRANSFORM_TYPE ToOETransformType(D3DTRANSFORMSTATETYPE eType);

	static D3DCULL ToD3DCullMode(IOERenderer::CULL_MODE_TYPE eType);
	static IOERenderer::CULL_MODE_TYPE ToOECullMode(D3DCULL eType);

	static D3DFILLMODE ToD3DFillMode(IOERenderer::FILL_MODE eType);
	static IOERenderer::FILL_MODE ToOEFillMode(D3DFILLMODE eType);

	static void ToD3DXMatrix(D3DXMATRIX& matOut, const CMatrix4x4& matIn);
	static void ToOEMatrix(CMatrix4x4& matOut, const D3DXMATRIX& matIn);
};

#endif // __OED3DUTIL_H__
