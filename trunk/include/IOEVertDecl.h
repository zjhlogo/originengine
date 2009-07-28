/*!
 * \file IOEVertDecl.h
 * \date 24-5-2009 17:48:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEVERTDECL_H__
#define __IOEVERTDECL_H__

#include "IOEObject.h"

class IOEVertDecl : public IOEObject
{
public:
	enum CONST_DEFINE
	{
		MAX_ELEMENT_COUNT = 32,
	};

	enum TYPE
	{
		T_UNKNOWN = 0,
		T_FLOAT1,
		T_FLOAT2,
		T_FLOAT3,
		T_FLOAT4,
		T_COLOR,
		T_MAX,
	};

	enum USAGE
	{
		U_UNKNOWN = 0,
		U_POSITION,
		U_POSITIONT,
		U_NORMAL,
		U_TEXCOORD,
		U_COLOR,
		U_MAX,
	};

	typedef struct ELEMENT_tag
	{
		TYPE eType;
		USAGE eUsage;
		uint nIndex;
	} ELEMENT;

public:
	IOEVertDecl() {};
	virtual ~IOEVertDecl() {};

	virtual int GetStrideSize() const = 0;

};

#endif // __IOEVERTDECL_H__
