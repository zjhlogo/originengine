/*!
 * \file OEBones_Impl.h
 * \date 1-3-2010 14:53:26
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBONES_IMPL_H__
#define __OEBONES_IMPL_H__

#include <IOEBones.h>
#include <OEBaseTypeEx.h>

class COEBones_Impl : public IOEBones
{
public:
	COEBones_Impl(const tstring& strFile);
	virtual ~COEBones_Impl();

	virtual int GetBonesCount();
	virtual IOEBone* GetBone(int nIndex);

private:
	void Init();
	void Destroy();

	bool CreateBones(const tstring& strFile);
	void DestroyBones();

private:
	TV_BONE m_vBones;

};
#endif // __OEBONES_IMPL_H__
