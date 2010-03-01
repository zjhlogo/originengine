/*!
 * \file ConvMgr.h
 * \date 10-20-2009 15:23:56
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __CONVMGR_H__
#define __CONVMGR_H__

#include "IBaseConv.h"
#include <vector>

class CConvMgr
{
public:
	typedef std::vector<IBaseConv*> TV_BASE_CONV;

public:
	CConvMgr();
	~CConvMgr();

	static CConvMgr& Get();

	bool DoConvert(const tstring& strFileIn, const tstring& strFileOut);

private:
	void Init();
	void Destroy();

private:
	TV_BASE_CONV m_vConvList;

};
#endif // __CONVMGR_H__
