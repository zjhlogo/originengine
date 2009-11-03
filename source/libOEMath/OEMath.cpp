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

CMatrix4x4 operator *(const CMatrix4x4& m1, const CMatrix4x4& m2)
{
	return CMatrix4x4(m1.m[0]*m2.m[0] + m1.m[1]*m2.m[4] + m1.m[2]*m2.m[8] + m1.m[3]*m2.m[12],
					  m1.m[0]*m2.m[1] + m1.m[1]*m2.m[5] + m1.m[2]*m2.m[9] + m1.m[3]*m2.m[13],
					  m1.m[0]*m2.m[2] + m1.m[1]*m2.m[6] + m1.m[2]*m2.m[10] + m1.m[3]*m2.m[14],
					  m1.m[0]*m2.m[3] + m1.m[1]*m2.m[7] + m1.m[2]*m2.m[11] + m1.m[3]*m2.m[15],

					  m1.m[4]*m2.m[0] + m1.m[5]*m2.m[4] + m1.m[6]*m2.m[8] + m1.m[7]*m2.m[12],
					  m1.m[4]*m2.m[1] + m1.m[5]*m2.m[5] + m1.m[6]*m2.m[9] + m1.m[7]*m2.m[13],
					  m1.m[4]*m2.m[2] + m1.m[5]*m2.m[6] + m1.m[6]*m2.m[10] + m1.m[7]*m2.m[14],
					  m1.m[4]*m2.m[3] + m1.m[5]*m2.m[7] + m1.m[6]*m2.m[11] + m1.m[7]*m2.m[15],

					  m1.m[8]*m2.m[0] + m1.m[9]*m2.m[4] + m1.m[10]*m2.m[8] + m1.m[11]*m2.m[12],
					  m1.m[8]*m2.m[1] + m1.m[9]*m2.m[5] + m1.m[10]*m2.m[9] + m1.m[11]*m2.m[13],
					  m1.m[8]*m2.m[2] + m1.m[9]*m2.m[6] + m1.m[10]*m2.m[10] + m1.m[11]*m2.m[14],
					  m1.m[8]*m2.m[3] + m1.m[9]*m2.m[7] + m1.m[10]*m2.m[11] + m1.m[11]*m2.m[15],

					  m1.m[12]*m2.m[0] + m1.m[13]*m2.m[4] + m1.m[14]*m2.m[8] + m1.m[15]*m2.m[12],
					  m1.m[12]*m2.m[1] + m1.m[13]*m2.m[5] + m1.m[14]*m2.m[9] + m1.m[15]*m2.m[13],
					  m1.m[12]*m2.m[2] + m1.m[13]*m2.m[6] + m1.m[14]*m2.m[10] + m1.m[15]*m2.m[14],
					  m1.m[12]*m2.m[3] + m1.m[13]*m2.m[7] + m1.m[14]*m2.m[11] + m1.m[15]*m2.m[15]);
}

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

void COEMath::BuildQuaternionFromMatrix(CQuaternion& qOut, const CMatrix4x4& matIn)
{
	float fWSquaredMinus1 = matIn.m[0] + matIn.m[5] + matIn.m[10];
	float fXSquaredMinus1 = matIn.m[0] - matIn.m[5] - matIn.m[10];
	float fYSquaredMinus1 = matIn.m[5] - matIn.m[0] - matIn.m[10];
	float fZSquaredMinus1 = matIn.m[10] - matIn.m[0] - matIn.m[5];

	int nBiggestIndex = 0;
	float fBiggestSquaredMinus1 = fWSquaredMinus1;

	if (fXSquaredMinus1 > fBiggestSquaredMinus1)
	{
		fBiggestSquaredMinus1 = fXSquaredMinus1;
		nBiggestIndex = 1;
	}

	if (fYSquaredMinus1 > fBiggestSquaredMinus1)
	{
		fBiggestSquaredMinus1 = fYSquaredMinus1;
		nBiggestIndex = 2;
	}

	if (fZSquaredMinus1 > fBiggestSquaredMinus1)
	{
		fBiggestSquaredMinus1 = fZSquaredMinus1;
		nBiggestIndex = 3;
	}

	// 
	float fBiggestValue = sqrt(fBiggestSquaredMinus1 + 1.0f) * 0.5f;
	float fMult = 0.25f / fBiggestValue;

	// 
	switch (nBiggestIndex)
	{
	case 0:
		qOut.w = fBiggestValue;
		qOut.x = (matIn.m[6] - matIn.m[9]) * fMult;
		qOut.y = (matIn.m[8] - matIn.m[2]) * fMult;
		qOut.z = (matIn.m[1] - matIn.m[4]) * fMult;
		break;
	case 1:
		qOut.x = fBiggestValue;
		qOut.w = (matIn.m[6] - matIn.m[9]) * fMult;
		qOut.y = (matIn.m[1] + matIn.m[4]) * fMult;
		qOut.z = (matIn.m[8] + matIn.m[2]) * fMult;
		break;
	case 2:
		qOut.y = fBiggestValue;
		qOut.w = (matIn.m[8] - matIn.m[2]) * fMult;
		qOut.x = (matIn.m[1] + matIn.m[4]) * fMult;
		qOut.z = (matIn.m[6] + matIn.m[9]) * fMult;
		break;
	case 3:
		qOut.z = fBiggestValue;
		qOut.w = (matIn.m[1] - matIn.m[4]) * fMult;
		qOut.x = (matIn.m[8] + matIn.m[2]) * fMult;
		qOut.y = (matIn.m[6] + matIn.m[9]) * fMult;
		break;
	}
}

void COEMath::BuildMatrixFromQuaternion(CMatrix4x4& matOut, const CQuaternion& qIn)
{
	float f2WX = 2.0f*qIn.w*qIn.x;
	float f2WY = 2.0f*qIn.w*qIn.y;
	float f2WZ = 2.0f*qIn.w*qIn.z;
	float f2XX = 2.0f*qIn.x*qIn.x;
	float f2XY = 2.0f*qIn.x*qIn.y;
	float f2XZ = 2.0f*qIn.x*qIn.z;
	float f2YY = 2.0f*qIn.y*qIn.y;
	float f2YZ = 2.0f*qIn.y*qIn.z;
	float f2ZZ = 2.0f*qIn.z*qIn.z;

	matOut.m[0] = 1.0f - f2YY - f2ZZ;
	matOut.m[1] = f2XY + f2WZ;
	matOut.m[2] = f2XZ - f2WY;
	matOut.m[3] = 0.0f;

	matOut.m[4] = f2XY - f2WZ;
	matOut.m[5] = 1.0f - f2XX - f2ZZ;
	matOut.m[6] = f2YZ + f2WX;
	matOut.m[7] = 0.0f;

	matOut.m[8] = f2XZ + f2WY;
	matOut.m[9] = f2YZ - f2WX;
	matOut.m[10] = 1.0f - f2XX - f2YY;
	matOut.m[11] = 0.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = 0.0f;
	matOut.m[15] = 1.0f;
}

void COEMath::BuildQuaternionFromEuler(CQuaternion& qOut, float fRoll, float fPitch, float fYaw)
{
	//from : http://www.azure.com.cn
	//根据欧拉转动计算四元数的变换如下:
	//q = qyaw qpitch qroll

	//其中
	//qroll  = [cos(y/2), (sin(y/2), 0, 0)];
	//qpitch = [cos(q/2), (0, sin(q/2), 0)];
	//qyaw   = [cos(f/2), (0, 0, sin(f/2))];

	//计算求四元数时使用到的所有三角值
	float cr = cosf(fRoll / 2.0f);
	float cp = cosf(fPitch / 2.0f);
	float cy = cosf(fYaw / 2.0f);

	float sr = sinf(fRoll / 2.0f);
	float sp = sinf(fPitch / 2.0f);
	float sy = sinf(fYaw / 2.0f);
	float cpcy = cp * cy;
	float spsy = sp * sy;

	//组合这些值,生成四元数的向量和w
	qOut.x = sr*cpcy - cr*spsy;
	qOut.y = cr*sp*cy + sr*cp*sy;
	qOut.z = cr*cp*sy - sr*sp*cy;
	qOut.w = cr*cpcy + sr*spsy;
}

void COEMath::VectorLerp(CVector3& vOut, const CVector3& v1, const CVector3& v2, float t)
{
	float k0 = 1.0f - t;
	float k1 = t;

	vOut.x = k0*v1.x + k1*v2.x;
	vOut.y = k0*v1.y + k1*v2.y;
	vOut.z = k0*v1.z + k1*v2.z;
}

void COEMath::MatrixLerp(CMatrix4x4& matOut, const CMatrix4x4& mat1, const CMatrix4x4& mat2, float t)
{
	float k0 = 1.0f - t;
	float k1 = t;

	matOut.m[0] = k0*mat1.m[0] + k1*mat2.m[0];
	matOut.m[1] = k0*mat1.m[1] + k1*mat2.m[1];
	matOut.m[2] = k0*mat1.m[2] + k1*mat2.m[2];
	matOut.m[3] = 0.0f;

	matOut.m[4] = k0*mat1.m[4] + k1*mat2.m[4];
	matOut.m[5] = k0*mat1.m[5] + k1*mat2.m[5];
	matOut.m[6] = k0*mat1.m[6] + k1*mat2.m[6];
	matOut.m[7] = 0.0f;

	matOut.m[8] = k0*mat1.m[8] + k1*mat2.m[8];
	matOut.m[9] = k0*mat1.m[9] + k1*mat2.m[9];
	matOut.m[10] = k0*mat1.m[10] + k1*mat2.m[10];
	matOut.m[11] = 0.0f;

	matOut.m[12] = k0*mat1.m[12] + k1*mat2.m[12];
	matOut.m[13] = k0*mat1.m[13] + k1*mat2.m[13];
	matOut.m[14] = k0*mat1.m[14] + k1*mat2.m[14];
	matOut.m[15] = 1.0f;
}

void COEMath::QuaternionSlerp(CQuaternion& qOut, const CQuaternion& q1, const CQuaternion& q2, float t)
{
	float cosOmega = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;

	bool bInvert = false;
	if (cosOmega < 0.0f)
	{
		bInvert = true;
		cosOmega = -cosOmega;
	}

	float k0, k1;
	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float sinOmega = sqrt(1.0f - cosOmega*cosOmega);
		float omega = atan2f(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	if (bInvert)
	{
		qOut.w = q1.w*k0 - q2.w*k1;
		qOut.x = q1.x*k0 - q2.x*k1;
		qOut.y = q1.y*k0 - q2.y*k1;
		qOut.z = q1.z*k0 - q2.z*k1;
	}
	else
	{
		qOut.w = q1.w*k0 + q2.w*k1;
		qOut.x = q1.x*k0 + q2.x*k1;
		qOut.y = q1.y*k0 + q2.y*k1;
		qOut.z = q1.z*k0 + q2.z*k1;
	}
}
