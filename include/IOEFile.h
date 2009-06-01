/*!
 * \file IOEFile.h
 * \date 1-6-2009 8:50:38
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEFILE_H__
#define __IOEFILE_H__

#include "IOEObject.h"

enum OPEN_FILE_FLAG
{
	OFF_READ	= 0x00000001,
	OFF_WRITE	= 0x00000002,
};

class IOEFile : public IOEObject
{
public:
	IOEFile() {};
	virtual ~IOEFile() {};

	/*!
	 * \brief 
	 * \param pBuffOut
	 * \param nSize
	 * \return 
	 */
	virtual uint Read(void* pBuffOut, uint nSize) = 0;

	/*!
	 * \brief 
	 * \param pBuffIn
	 * \param nSize
	 * \return 
	 */
	virtual uint Write(const void* pBuffIn, uint nSize) = 0;

	/*!
	 * \brief seek from begin
	 * \param nOffset
	 * \return 
	 */
	virtual uint Seek(uint nOffset) = 0;

	/*!
	 * \brief 
	 * \return 
	 */
	virtual uint GetSize() const = 0;

	/*
	 *	\return current position of file
	 */
	virtual long Tell() const = 0;
};
#endif // __IOEFILE_H__
