/*!
 * \file IOEPiece.h
 * \date 10-23-2009 13:59:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEPIECE_H__
#define __IOEPIECE_H__

#include "../libOEBase/IOEObject.h"
#include <vector>

class IOEPiece : public IOEObject
{
public:
	IOEPiece() {};
	virtual ~IOEPiece() {};

	virtual const tstring& GetName() const = 0;

	virtual int GetMaterialID() const = 0;
	virtual int GetVertDeclMask() const = 0;

	virtual int GetNumVerts() const = 0;
	virtual int GetNumIndis() const = 0;
	virtual void* GetVerts() const = 0;
	virtual ushort* GetIndis() const = 0;

};

typedef std::vector<IOEPiece*> TV_PIECE;

#endif // __IOEPIECE_H__
