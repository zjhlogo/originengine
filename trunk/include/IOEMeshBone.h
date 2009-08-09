/*!
 * \file IOEMeshBone.h
 * \date 8-8-2009 22:01:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEMESHBONE_H__
#define __IOEMESHBONE_H__

#include "IOEObject.h"
#include "OEMath/OEMath.h"

class IOEMeshBone : public IOEObject
{
public:
	enum CONST_DEFINE
	{
		INVALID_BONE_ID = -1,
	};

	typedef struct BONE_FRAME_tag
	{
		float fTime;
		CMatrix4x4 matTransform;
	} BONE_FRAME;

public:
	IOEMeshBone() {};
	virtual ~IOEMeshBone() {};

	virtual const tstring& GetName() const = 0;
	virtual int GetID() const = 0;
	virtual IOEMeshBone* GetParent() const = 0;

	virtual float GetTimeLength() const = 0;
	virtual const CMatrix4x4& GetLocalMatrix() const = 0;

	virtual int GetFrameCount() const = 0;
	virtual const BONE_FRAME* GetFrame(int nIndex) const = 0;

	virtual int GetNumChildren() const = 0;
	virtual IOEMeshBone* GetChild(int nIndex) const = 0;

};
#endif // __IOEMESHBONE_H__
