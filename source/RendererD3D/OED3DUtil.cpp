/*!
 * \file OED3DUtil.cpp
 * \date 24-5-2009 18:03:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OED3DUtil.h"
#include <assert.h>

D3DDECLTYPE COED3DUtil::ToD3DVertType(IOEVertDecl::TYPE eType)
{
	static const D3DDECLTYPE DECLARE_MAP[IOEVertDecl::T_MAX] =
	{
		D3DDECLTYPE_UNUSED,
		D3DDECLTYPE_FLOAT1,  // 1D float expanded to (value, 0., 0., 1.)
		D3DDECLTYPE_FLOAT2,  // 2D float expanded to (value, value, 0., 1.)
		D3DDECLTYPE_FLOAT3,  // 3D float expanded to (value, value, value, 1.)
		D3DDECLTYPE_FLOAT4,  // 4D float
		D3DDECLTYPE_D3DCOLOR,
	};

	assert(eType >= 0 && eType < IOEVertDecl::T_MAX);
	return DECLARE_MAP[eType];
}

IOEVertDecl::TYPE COED3DUtil::ToOEVertType(D3DDECLTYPE eType)
{
	static const int MAP_SIZE = 18;
	static const IOEVertDecl::TYPE DECLARE_MAP[MAP_SIZE] =
	{
		IOEVertDecl::T_FLOAT1,
		IOEVertDecl::T_FLOAT2,
		IOEVertDecl::T_FLOAT3,
		IOEVertDecl::T_FLOAT4,
		IOEVertDecl::T_COLOR,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
		IOEVertDecl::T_UNKNOWN,
	};

	assert(eType >= 0 && eType < MAP_SIZE);
	return DECLARE_MAP[eType];
}

D3DDECLUSAGE COED3DUtil::ToD3DVertUsage(IOEVertDecl::USAGE eUsage)
{
	static const D3DDECLUSAGE USAGE_MAP[IOEVertDecl::U_MAX] =
	{
		D3DDECLUSAGE_POSITION,
		D3DDECLUSAGE_POSITION,
		D3DDECLUSAGE_NORMAL,        // 3
		D3DDECLUSAGE_TEXCOORD,      // 5
		D3DDECLUSAGE_COLOR,         // 10
	};

	assert(eUsage >= 0 && eUsage < IOEVertDecl::U_MAX);
	return USAGE_MAP[eUsage];
}

IOEVertDecl::USAGE COED3DUtil::ToOEVertUsage(D3DDECLUSAGE eUsage)
{
	static const int MAP_SIZE = 14;
	static const IOEVertDecl::USAGE USAGE_MAP[MAP_SIZE] =
	{
		IOEVertDecl::U_POSITION,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_NORMAL,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_TEXCOORD,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_COLOR,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
		IOEVertDecl::U_UNKNOWN,
	};

	assert(eUsage >= 0 && eUsage < MAP_SIZE);
	return USAGE_MAP[eUsage];
}

int COED3DUtil::GetVertTypeSize(IOEVertDecl::TYPE eType)
{
	static const int DECLARE_MAP[IOEVertDecl::T_MAX] =
	{
		0,
		sizeof(float)*1,	// 1D float expanded to (value, 0., 0., 1.)
		sizeof(float)*2,	// 2D float expanded to (value, value, 0., 1.)
		sizeof(float)*3,	// 3D float expanded to (value, value, value, 1.)
		sizeof(float)*4,	// 4D float
		sizeof(uint),		// color
	};

	assert(eType >= 0 && eType < IOEVertDecl::T_MAX);
	return DECLARE_MAP[eType];
}
