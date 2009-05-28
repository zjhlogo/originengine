/*!
 * \file Vector3.cpp
 * \date 26-5-2009 21:15:59
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEMath/Vector3.h>
#include <OEMath/OEMath.h>
#include <assert.h>

void CVector3::Normalize()
{
	float fMag = Length();

	if (fMag > 0.0f)
	{
		float fInvMag = 1.0f/fMag;

		x *= fInvMag;
		y *= fInvMag;
		z *= fInvMag;
	}
	else
	{
		assert(false);
	}
}
