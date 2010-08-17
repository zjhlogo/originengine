/*!
 * \file Vector4.cpp
 * \date 26-5-2009 21:48:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <libOEMath/Vector4.h>
#include <libOEMath/OEMath.h>
#include <assert.h>

void CVector4::Normalize()
{
	float fMag = Length();

	if (fMag > 0.0f)
	{
		float fInvMag = 1.0f/fMag;

		x *= fInvMag;
		y *= fInvMag;
		z *= fInvMag;
		w *= fInvMag;
	}
	else
	{
		assert(false);
	}
}
