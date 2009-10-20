/*!
 * \file Ms3dConv_Impl.h
 * \date 10-20-2009 15:20:29
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MS3DCONV_IMPL_H__
#define __MS3DCONV_IMPL_H__

#include "IBaseConv.h"
#include <vector>

class CMs3dConv_Impl : public IBaseConv
{
public:
	typedef struct VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		//int nBoneID;
	} VERTEX;

	typedef std::vector<VERTEX> TV_VERTEX;
	typedef std::vector<ushort> TV_INDEX;

public:
	CMs3dConv_Impl();
	virtual ~CMs3dConv_Impl();

	virtual bool CanConvert(const tstring& strFileExt);
	virtual bool DoConvert(const tstring& strFileIn, const tstring& strFileOut);

private:
	void Init();
	void Destroy();

	bool LoadFromFile(const tstring& strFile);
	bool SaveToFile(const tstring& strFile);

private:
	TV_VERTEX m_vVertices;
	TV_INDEX m_vIndices;

};
#endif // __MS3DCONV_IMPL_H__
