/*!
 *	created:	2009/06/01
 *	filename: 	OEFile_Impl.h
 *	author:		daishuidi
	
 *	purpose:	
 */

#ifndef __OEFILE_IMPL_H__
#define __OEFILE_IMPL_H__

#include <OEBase/IOEFile.h>
#include <libOEBase/OEOS.h>

class COEFile_Impl : public IOEFile
{
public:
	RTTI_DEF(COEFile_Impl, IOEFile);

	COEFile_Impl(const tstring& strFileName, uint nFlag);
	virtual ~COEFile_Impl();

	virtual uint Read(void* pBuffOut, uint nSize);
	virtual uint Write(const void* pBuffIn, uint nSize);
	virtual uint Seek(uint nOffset);
	virtual uint GetSize() const;
	virtual uint Tell() const;

private:
	void Init();
	void Destroy();

	bool Open(const tstring& strFileName, uint nFlag);

private:
	COEOS::OEFILE m_hFile;
	uint m_nFlag;

};

#endif // __OEFILE_IMPL_H__
