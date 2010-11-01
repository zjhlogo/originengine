/*!
 * \file OED3DUtil.h
 * \date 24-5-2009 18:03:21
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DUTIL_H__
#define __OED3DUTIL_H__

#include <OECore/IOEVertDecl.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/IOETexture.h>
#include <libOEMath/OEMath.h>
#include <d3dx9.h>

class COED3DUtil
{
public:
	static void ToD3DXMatrix(D3DXMATRIX& matOut, const CMatrix4x4& matIn);
	static void ToOEMatrix(CMatrix4x4& matOut, const D3DXMATRIX& matIn);

	static void ToD3DVector3(D3DXVECTOR3& vOut, const CVector3& vIn);
	static void ToOEVector3(CVector3& vOut, const D3DXVECTOR3& vIn);

	static void ToD3DVector4(D3DXVECTOR4& vOut, const CVector4& vIn);
	static void ToOEVector4(CVector4& vOut, const D3DXVECTOR4& vIn);

	static D3DDECLTYPE ToD3DVertType(VERT_DECL_TYPE eType);
	static VERT_DECL_TYPE ToOEVertType(D3DDECLTYPE eType);

	static D3DDECLUSAGE ToD3DVertUsage(VERT_DECL_USAGE eUsage);
	static VERT_DECL_USAGE ToOEVertUsage(D3DDECLUSAGE eUsage);

	static int GetVertTypeSize(VERT_DECL_TYPE eType);

	static D3DTRANSFORMSTATETYPE ToD3DTransformType(TRANSFORM_TYPE eType);
	static TRANSFORM_TYPE ToOETransformType(D3DTRANSFORMSTATETYPE eType);

	static D3DCULL ToD3DCullMode(CULL_MODE_TYPE eType);
	static CULL_MODE_TYPE ToOECullMode(D3DCULL eType);

	static D3DFILLMODE ToD3DFillMode(FILL_MODE eType);
	static FILL_MODE ToOEFillMode(D3DFILLMODE eType);

	//static D3DTEXTUREFILTERTYPE ToD3DSampleFilter(OE_SAMPLE_FILTER eType);
	//static OE_SAMPLE_FILTER ToOESampleFilter(D3DTEXTUREFILTERTYPE eType);

	static D3DFORMAT ToD3DTexFmt(TEXTURE_FORMAT eFormat);
	static TEXTURE_FORMAT ToOETexFmt(D3DFORMAT eFormat);

	static uint ToD3DColorWriteChannel(uint nColorWriteChannel);
	static uint ToOEColorWriteChannel(uint nColorWriteChannel);

};

#endif // __OED3DUTIL_H__
