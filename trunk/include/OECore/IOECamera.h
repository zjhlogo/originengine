/*!
 * \file IOECamera.h
 * \date 10-23-2010 9:38:57
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOECAMERA_H__
#define __IOECAMERA_H__

#include "../libOEBase/IOEObject.h"
#include "../libOEMath/OEMath.h"
#include "../OECore/IOENode.h"

class IOECamera : public IOEObject
{
public:
	RTTI_DEF(IOECamera, IOEObject);

	IOECamera() {};
	virtual ~IOECamera() {};

	virtual const CMatrix4x4& GetViewMatrix() const = 0;

	virtual void SetPosition(const CVector3& pos) = 0;
	virtual const CVector3& GetPosition() const = 0;

	virtual const CVector3& GetVectorUp() const = 0;
	virtual const CVector3& GetVectorForword() const = 0;
	virtual const CVector3& GetVectorRight() const = 0;

	virtual void SetTargetNode(IOENode* pNode) = 0;
	virtual IOENode* GetTargetNode() = 0;

	virtual void Update(float fDetailTime) = 0;
};
#endif // __IOECAMERA_H__
