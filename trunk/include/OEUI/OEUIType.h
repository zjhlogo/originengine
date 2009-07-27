/*!
 * \file OEUIType.h
 * \date 27-7-2009 16:12:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUITYPE_H__
#define __OEUITYPE_H__

#include "../OEBasicType.h"

typedef struct OEUI_VERTEX_tag
{
	float x, y, z, w;
	uint nColor;
	float u, v;
} OEUI_VERTEX;

class CPoint
{
public:
	CPoint()
	{
		x = 0.0f;
		y = 0.0f;
	}

	CPoint(float fX, float fY)
	{
		x = fX;
		y = fY;
	}

	~CPoint() {};

public:
	float x;
	float y;

};

class CSize
{
public:
	CSize()
	{
		width = 0.0f;
		height = 0.0f;
	}

	CSize(float fWidth, float fHeight)
	{
		width = fWidth;
		height = fHeight;
	}

	~CSize() {};

public:
	float width;
	float height;

};

class CRect
{
public:
	CRect()
	{
		x = 0.0f;
		y = 0.0f;
		width = 0.0f;
		height = 0.0f;
	}

	CRect(float fX, float fY, float fWidth, float fHeight)
	{
		x = fX;
		y = fY;
		width = fWidth;
		height = fHeight;
	}

	~CRect() {};

public:
	float x;
	float y;
	float width;
	float height;

};

#endif // __OEUITYPE_H__
