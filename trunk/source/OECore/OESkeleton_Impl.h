/*!
 * \file OESkeleton_Impl.h
 * \date 1-3-2010 19:21:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OESKELETON_IMPL_H__
#define __OESKELETON_IMPL_H__

#include <OECore/IOESkeleton.h>

class COESkeleton_Impl : public IOESkeleton
{
public:
	RTTI_DEF(COESkeleton_Impl, IOESkeleton);

	COESkeleton_Impl(const tstring& strFile);
	virtual ~COESkeleton_Impl();

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
#endif // __OESKELETON_IMPL_H__
