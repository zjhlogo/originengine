/*!
 * \file ConverterMgr.h
 * \date 1-3-2010 21:18:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __CONVERTERMGR_H__
#define __CONVERTERMGR_H__

#include "IConverter.h"
#include <vector>

class CConverterMgr
{
public:
	typedef std::vector<IConverter*> TV_CONVERTER;

public:
	CConverterMgr();
	~CConverterMgr();

	static CConverterMgr& Get();

	bool DoConvert(const tstring& strFileIn, const tstring& strFileOut);

private:
	void Init();
	void Destroy();

private:
	TV_CONVERTER m_vConverter;

};
#endif // __CONVERTERMGR_H__
