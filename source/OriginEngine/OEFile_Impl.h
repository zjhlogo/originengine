/*!
 *	created:	2009/06/01
 *	filename: 	OEFile_Impl.h
 *	author:		daishuidi
	
 *	purpose:	
 */

#ifndef __OEFILE_IMPL_H__
#define __OEFILE_IMPL_H__

#include <IOEFile.h>

class COEFile_Impl : public IOEFile
{
public:
	COEFile_Impl(const tchar* pstrFileName, uint nFlag);
	virtual ~COEFile_Impl();

	virtual uint Read(void* pBuffOut, uint nSize);

	virtual uint Write(const void* pBuffIn, uint nSize);

	virtual uint Seek(uint nOffset);

	virtual uint GetSize() const;

	virtual uint Tell() const;

private:
	void Init();
	void Destroy();

	bool Open(const tchar* pstrFileName, uint nFlag);

private:
	FILE* m_pFile;
	uint m_nFlag;

};

#endif // __OEFILE_IMPL_H__