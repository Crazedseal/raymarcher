#include <math.h>
#include "Vector3.h"

#ifndef VECTOR4D_H
#define VECTOR4D_H
namespace vectors
{
	class Vector4d
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Vector4d(float x, float y, float z, float w);
		Vector4d(Vector3 vec, float w);
		Vector4d();

		friend Vector4d operator*(const Vector4d &a, const Vector4d &b)
		{
			Vector4d vec;
			vec.x = a.x * b.x;
			vec.y = a.y * b.y;
			vec.z = a.z * b.z;
			vec.w = a.w * b.w;
			return vec;
		}

		float sum();

		Vector3 toVector3();
	};
} // namespace vectors
#endif