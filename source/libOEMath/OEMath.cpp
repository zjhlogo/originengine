/*!
 * \file OEMath.cpp
 * \date 26-5-2009 22:42:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEMath/OEMath.h>

const float OEMATH_PI = 3.14159265f;
const float OEMATH_G = -32.174f;				// acceleration due to gravity, ft/s^2
const float OEMATH_RHO = 0.0023769f;			// desity of air at sea level, slugs/ft^3
const float OEMATH_TOL = 1E-5f;					// float type tolerance

void COEMath::BuildLookAtMatrixLH(CMatrix4x4& matOut, const CVector3& vEye, const CVector3& vAt, const CVector3& vUp)
{
	CVector3 vAxisZ = vAt-vEye;
	vAxisZ.Normalize();

	CVector3 vAxisX = vUp^vAxisZ;
	vAxisX.Normalize();

	CVector3 vAxisY = vAxisZ^vAxisX;

	matOut.m[0] = vAxisX.x;
	matOut.m[1] = vAxisY.x;
	matOut.m[2] = vAxisZ.x;
	matOut.m[3] = 0.0f;

	matOut.m[4] = vAxisX.y;
	matOut.m[5] = vAxisY.y;
	matOut.m[6] = vAxisZ.y;
	matOut.m[7] = 0.0f;

	matOut.m[8] = vAxisX.z;
	matOut.m[9] = vAxisY.z;
	matOut.m[10] = vAxisZ.z;
	matOut.m[11] = 0.0f;

	matOut.m[12] = -(vAxisX*vEye);
	matOut.m[13] = -(vAxisY*vEye);
	matOut.m[14] = -(vAxisZ*vEye);
	matOut.m[15] = 1.0f;
}

void COEMath::BuildProjectMatrixLH(CMatrix4x4& matOut, float fFov, float fAspect, float fNear, float fFar)
{
	float h = tanf(0.5f*fFov);
	float w = h*fAspect;

	matOut.m[0] = 1.0f/w;
	matOut.m[1] = 0.0f;
	matOut.m[2] = 0.0f;
	matOut.m[3] = 0.0f;

	matOut.m[4] = 0.0f;
	matOut.m[5] = 1.0f/h;
	matOut.m[6] = 0.0f;
	matOut.m[7] = 0.0f;

	matOut.m[8] = 0.0f;
	matOut.m[9] = 0.0f;
	matOut.m[10] = fFar/(fFar-fNear);
	matOut.m[11] = 1.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = -fNear*fFar/(fFar-fNear);
	matOut.m[15] = 0.0f;
}
