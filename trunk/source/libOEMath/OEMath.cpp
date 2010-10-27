/*!
 * \file OEMath.cpp
 * \date 26-5-2009 22:42:40
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <libOEMath/OEMath.h>

const float COEMath::PI_2 = 1.570796325f;
const float COEMath::PI = 3.14159265f;
const float COEMath::PIX2 = 6.2831853f;
const float COEMath::G = -32.174f;							// acceleration due to gravity, ft/s^2
const float COEMath::RHO = 0.0023769f;						// desity of air at sea level, slugs/ft^3
const float COEMath::TOL = 1E-5f;								// float type tolerance
const float COEMath::FLOAT_MIN = 1.175494351e-38F;			// 1.175494351e-38F
const float COEMath::FLOAT_MAX = 3.402823466e+38F;			// 3.402823466e+38F

const CVector3 COEMath::VECTOR_ZERO(0.0f, 0.0f, 0.0f);
const CVector3 COEMath::VECTOR_RIGHT(1.0f, 0.0f, 0.0f);
const CVector3 COEMath::VECTOR_UP(0.0f, 1.0f, 0.0f);
const CVector3 COEMath::VECTOR_FORWARD(0.0f, 0.0f, 1.0f);

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

void COEMath::BuildLookAtMatrixRH(CMatrix4x4& matOut, const CVector3& vEye, const CVector3& vAt, const CVector3& vUp)
{
	CVector3 vForward = vAt - vEye;
	vForward.Normalize();

	CVector3 vRight = vForward ^ vUp;
	vRight.Normalize();

	CVector3 vNewUp = vRight ^ vForward;
	vNewUp.Normalize();

	matOut.m[0] = vRight.x;
	matOut.m[4] = vRight.y;
	matOut.m[8] = vRight.z;
	matOut.m[12] = 0.0f;

	matOut.m[1] = vNewUp.x;
	matOut.m[5] = vNewUp.y;
	matOut.m[9] = vNewUp.z;
	matOut.m[13] = 0.0f;

	matOut.m[2] = -vForward.x;
	matOut.m[6] = -vForward.y;
	matOut.m[10] = -vForward.z;
	matOut.m[14] = 0.0f;

	matOut.m[3] = -vEye.x;
	matOut.m[7] = -vEye.y;
	matOut.m[11] = -vEye.z;
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

void COEMath::BuildProjectMatrixRH(CMatrix4x4& matOut, float fFov, float fAspect, float fNear, float fFar)
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
	matOut.m[11] = -1.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = fNear*fFar/(fFar-fNear);
	matOut.m[15] = 0.0f;
}

void COEMath::BuildMatrixFromQuaternion(CMatrix4x4& matOut, const CQuaternion& qIn)
{
	// formula
	//     [ 1-2*y*y-2*z*z,     2*x*y-2*z*w,       2*x*z+2*y*w,       0 ]
	// R = [ 2*x*y+2*z*w,       1-2*x*x-2*z*z,     2*y*z-2*x*w,       0 ]
	//     [ 2*x*z-2*y*w,       2*y*z+2*x*w,       1-2*x*x-2*y*y,     0 ]
	//     [ 0,                 0,                 0,                 1 ]

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

void COEMath::BuildMatrixFromEulerXYZ(CMatrix4x4& matOut, float x, float y, float z)
{
	// formula
	//     [ cos(y)*cos(z),                          -cos(y)*sin(z),                           sin(y),            0 ]
	// R = [ cos(z)*sin(x)*sin(y)+cos(x)*sin(z),      cos(x)*cos(z)-sin(x)*sin(y)*sin(z),     -cos(y)*sin(x),     0 ]
	//     [-cos(x)*cos(z)*sin(y)+sin(x)*sin(z),      cos(z)*sin(x)+cos(x)*sin(y)*sin(z),      cos(x)*cos(y),     0 ]
	//     [ 0,                                       0,                                       0,                 1 ]

	float cx = cosf(x);
	float cy = cosf(y);
	float cz = cosf(z);
	float sx = sinf(x);
	float sy = sinf(y);
	float sz = sinf(z);

	matOut.m[0] = cy*cz;
	matOut.m[1] = -cy*sz;
	matOut.m[2] = sy;
	matOut.m[3] = 0.0f;

	matOut.m[4] = cz*sx*sy + cx*sz;
	matOut.m[5] = cx*cz - sx*sy*sz;
	matOut.m[6] = -cy*sx;
	matOut.m[7] = 0.0f;

	matOut.m[8] = -cx*cz*sy + sx*sz;
	matOut.m[9] = cz*sx + cx*sy*sz;
	matOut.m[10] = cx*cy;
	matOut.m[11] = 0.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = 0.0f;
	matOut.m[15] = 1.0f;
}

void COEMath::BuildMatrixFromEulerYZX(CMatrix4x4& matOut, float x, float y, float z)
{
	// formula
	//     [ cos(y)*cos(z),     -cos(y)*sin(z)*cos(x)+sin(y)*sin(x),      cos(y)*sin(z)*sin(x)+sin(y)*cos(x),     0 ]
	// R = [ sin(z),             cos(z)*cos(x),                          -cos(z)*sin(x),                          0 ]
	//     [-sin(y)*cos(z),      sin(y)*sin(z)*cos(x)+cos(y)*sin(x),     -sin(y)*sin(z)*sin(x)+cos(y)*cos(x),     0 ]
	//     [ 0,                  0,                                       0,                                      1 ]

	float cx = cosf(x);
	float cy = cosf(y);
	float cz = cosf(z);
	float sx = sinf(x);
	float sy = sinf(y);
	float sz = sinf(z);

	matOut.m[0] = cy*cz;
	matOut.m[1] = -cy*sz*cx+sy*sx;
	matOut.m[2] = cy*sz*sx+sy*cx;
	matOut.m[3] = 0.0f;

	matOut.m[4] = sz;
	matOut.m[5] = cz*cx;
	matOut.m[6] = -cz*sx;
	matOut.m[7] = 0.0f;

	matOut.m[8] = -sy*cz;
	matOut.m[9] = sy*sz*cx+cy*sx;
	matOut.m[10] = -sy*sz*sx+cy*cx;
	matOut.m[11] = 0.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = 0.0f;
	matOut.m[15] = 1.0f;
}

void COEMath::BuildRotationMatrix(CMatrix4x4& matOut, const CVector3& vAxis, float fRadius)
{
	// formula
	//     [t*x*x+cos(r),      t*x*y-sin(r)*z,    t*x*z+sin(r)*y,    0]
	// R = [t*x*y+sin(r)*z,    t*y*y+cos(r),      t*y*z-sin(r)*x,    0]
	//     [t*x*z-sin(r)*y,    t*y*z+sin(r)*x,    t*z*z+cos(r),      0]
	//     [0,                 0,                 0,                 1]
	//
	// t = 1.0f - cos(r);

	float fCos = cosf(fRadius);
	float fSin = sinf(fRadius);
	float fSum = 1.0f - fCos;

	matOut.m[0] = (vAxis.x * vAxis.x) * fSum + fCos;
	matOut.m[1] = (vAxis.x * vAxis.y) * fSum - (vAxis.z * fSin);
	matOut.m[2] = (vAxis.x * vAxis.z) * fSum + (vAxis.y * fSin);
	matOut.m[3] = 0.0f;

	matOut.m[4] = (vAxis.y * vAxis.x) * fSum + (vAxis.z * fSin);
	matOut.m[5] = (vAxis.y * vAxis.y) * fSum + fCos ;
	matOut.m[6] = (vAxis.y * vAxis.z) * fSum - (vAxis.x * fSin);
	matOut.m[7] = 0.0f;

	matOut.m[8] = (vAxis.z * vAxis.x) * fSum - (vAxis.y * fSin);
	matOut.m[9] = (vAxis.z * vAxis.y) * fSum + (vAxis.x * fSin);
	matOut.m[10] = (vAxis.z * vAxis.z) * fSum + fCos;
	matOut.m[11] = 0.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = 0.0f;
	matOut.m[15] = 1.0f;
}

void COEMath::BuildRotationXMatrix(CMatrix4x4& matOut, float fRadius)
{
	// formula
	//     [ 1,     0,           0,          0 ]
	// R = [ 0,     cos(r),     -sin(r),     0 ]
	//     [ 0,     sin(r),      cos(r),     0 ]
	//     [ 0,     0,           0,          1 ]

	float cr = cos(fRadius);
	float sr = sin(fRadius);

	matOut.m[0] = 1.0f;
	matOut.m[1] = 0.0f;
	matOut.m[2] = 0.0f;
	matOut.m[3] = 0.0f;

	matOut.m[4] = 0.0f;
	matOut.m[5] = cr;
	matOut.m[6] = -sr;
	matOut.m[7] = 0.0f;

	matOut.m[8] = 0.0f;
	matOut.m[9] = sr;
	matOut.m[10] = cr;
	matOut.m[11] = 0.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = 0.0f;
	matOut.m[15] = 1.0f;
}

void COEMath::BuildRotationYMatrix(CMatrix4x4& matOut, float fRadius)
{
	// formula
	//     [ cos(r),     0,     sin(r),      0 ]
	// R = [ 0,          1,     0,           0 ]
	//     [-sin(r),     0,     cos(r),      0 ]
	//     [ 0,          0,     0,           1 ]

	float cr = cos(fRadius);
	float sr = sin(fRadius);

	matOut.m[0] = cr;
	matOut.m[1] = 0.0f;
	matOut.m[2] = sr;
	matOut.m[3] = 0.0f;

	matOut.m[4] = 0.0f;
	matOut.m[5] = 1.0f;
	matOut.m[6] = 0.0f;
	matOut.m[7] = 0.0f;

	matOut.m[8] = -sr;
	matOut.m[9] = 0.0f;
	matOut.m[10] = cr;
	matOut.m[11] = 0.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = 0.0f;
	matOut.m[15] = 1.0f;
}

void COEMath::BuildRotationZMatrix(CMatrix4x4& matOut, float fRadius)
{
	// formula
	//     [ cos(r),    -sin(r),     0,      0 ]
	// R = [ sin(r),     cos(r),     0,      0 ]
	//     [ 0,          0,          1,      0 ]
	//     [ 0,          0,          0,      1 ]

	float cr = cos(fRadius);
	float sr = sin(fRadius);

	matOut.m[0] = cr;
	matOut.m[1] = -sr;
	matOut.m[2] = 0.0f;
	matOut.m[3] = 0.0f;

	matOut.m[4] = sr;
	matOut.m[5] = cr;
	matOut.m[6] = 0.0f;
	matOut.m[7] = 0.0f;

	matOut.m[8] = 0.0f;
	matOut.m[9] = 0.0f;
	matOut.m[10] = 1.0f;
	matOut.m[11] = 0.0f;

	matOut.m[12] = 0.0f;
	matOut.m[13] = 0.0f;
	matOut.m[14] = 0.0f;
	matOut.m[15] = 1.0f;
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

void COEMath::BuildQuaternionFromEulerXYZ(CQuaternion& qOut, float x, float y, float z)
{
	// from http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// 根据欧拉角计算四元数的变换如下(四元数的连接顺序是从右到左):
	// q = qz * qy * qx

	// 其中
	// qz = [ cos(z/2), [0, 0, sin(z/2)] ];
	// qy = [ cos(y/2), [0, sin(y/2), 0] ];
	// qx = [ cos(x/2), [sin(x/2), 0, 0] ];

	// 结果
	// q.w = [ cos(x/2)*cos(y/2)*cos(z/2) + sin(x/2)*sin(y/2)*sin(z/2) ]
	// q.x = [ sin(x/2)*cos(y/2)*cos(z/2) - cos(x/2)*sin(y/2)*sin(z/2) ]
	// q.y = [ cos(x/2)*sin(y/2)*cos(z/2) + sin(x/2)*cos(y/2)*sin(z/2) ]
	// q.z = [ cos(x/2)*cos(y/2)*sin(z/2) - sin(x/2)*sin(y/2)*cos(z/2) ]

	// 计算求四元数时使用到的所有三角值
	float cx = cosf(x/2.0f);
	float cy = cosf(y/2.0f);
	float cz = cosf(z/2.0f);
	float sx = sinf(x/2.0f);
	float sy = sinf(y/2.0f);
	float sz = sinf(z/2.0f);

	//组合这些值,生成四元数的向量和w
	qOut.w = cx*cy*cz + sx*sy*sz;
	qOut.x = sx*cy*cz - cx*sy*sz;
	qOut.y = cx*sy*cz + sx*cy*sz;
	qOut.z = cx*cy*sz - sx*sy*cz;
}

void COEMath::BuildQuaternionFromEulerYZX(CQuaternion& qOut, float x, float y, float z)
{
	// formula
	// q.w = [ cos(y/2)*cos(z/2)*cos(x/2) - sin(y/2)*sin(z/2)*sin(x/2) ]
	// q.x = [ sin(y/2)*sin(z/2)*cos(x/2) + cos(y/2)*cos(z/2)*sin(x/2) ]
	// q.y = [ sin(y/2)*cos(z/2)*cos(x/2) + cos(y/2)*sin(z/2)*sin(x/2) ]
	// q.z = [ cos(y/2)*sin(z/2)*cos(x/2) - sin(y/2)*cos(z/2)*sin(x/2) ]

	// 计算求四元数时使用到的所有三角值
	float cx = cosf(x/2.0f);
	float cy = cosf(y/2.0f);
	float cz = cosf(z/2.0f);
	float sx = sinf(x/2.0f);
	float sy = sinf(y/2.0f);
	float sz = sinf(z/2.0f);

	//组合这些值,生成四元数的向量和w
	qOut.w = cy*cz*cx - sy*sz*sx;
	qOut.x = sy*sz*cx + cy*cz*sx;
	qOut.y = sy*cz*cx + cy*sz*sx;
	qOut.z = cy*sz*cx - sy*cz*sx;
}

void COEMath::GetEulerAngle(float& x, float& y, float& z, const CQuaternion& qIn)
{
	float test = qIn.x*qIn.y + qIn.z*qIn.w;

	if (test > 0.499f)			// singularity at north pole
	{
		y = 2.0f * atan2f(qIn.x, qIn.w);
		z = PI_2;
		x = 0.0f;
		return;
	}

	if (test < -0.499f)			// singularity at south pole
	{
		y = -2.0f * atan2(qIn.x, qIn.w);
		z = -PI_2;
		x = 0;
		return;
	}

	float sqx = qIn.x*qIn.x;
	float sqy = qIn.y*qIn.y;
	float sqz = qIn.z*qIn.z;
	y = atan2(2.0f*qIn.y*qIn.w-2.0f*qIn.x*qIn.z , 1.0f - 2.0f*sqy - 2.0f*sqz);
	z = asin(2.0f*test);
	x = atan2(2.0f*qIn.x*qIn.w-2.0f*qIn.y*qIn.z , 1.0f - 2.0f*sqx - 2.0f*sqz);
}

//void COEMath::GetEulerAngle(float& x, float& y, float& z, const CMatrix4x4& matIn)
//{
//
//}

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
