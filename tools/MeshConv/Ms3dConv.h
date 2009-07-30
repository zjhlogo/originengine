/*!
 * \file Ms3dConv.h
 * \date 30-7-2009 20:21:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MS3DCONV_H__
#define __MS3DCONV_H__

#include "BaseConv.h"
#include <vector>

class CMs3dConv : public CBaseConv
{
public:
	typedef struct VERTEX
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		//int nBoneID;
	} VERTEX;

	typedef std::vector<VERTEX> VVERTEX;
	typedef std::vector<ushort> VINDEX;

public:
	CMs3dConv();
	virtual ~CMs3dConv();

	virtual bool LoadFromFile(const tstring& strFileName);
	virtual bool DoConvert(const tstring& strFileName);

private:
	void Init();
	void Destroy();

private:
	VVERTEX m_vVertices;
	VINDEX m_vIndices;

};

#endif // __MS3DCONV_H__
