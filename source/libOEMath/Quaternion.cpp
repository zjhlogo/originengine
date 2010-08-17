/*!
 * \file Quaternion.cpp
 * \date 26-5-2009 22:29:57
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <libOEMath/Quaternion.h>
#include <libOEMath/OEMath.h>
#include <assert.h>

void CQuaternion::Normalize()
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
