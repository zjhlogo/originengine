/*!
 * \file IOEMeshPiece.h
 * \date 8-8-2009 21:26:31
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMESHPIECE_H__
#define __IOEMESHPIECE_H__

#include "IOEObject.h"

class IOEMeshPiece : public IOEObject
{
public:
	IOEMeshPiece() {};
	virtual ~IOEMeshPiece() {};

	virtual const tstring& GetName() const = 0;

};

#endif // __IOEMESHPIECE_H__
