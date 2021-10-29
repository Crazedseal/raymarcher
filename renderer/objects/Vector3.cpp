#include <math.h>
#include "Vector3.h"

namespace vectors
{
	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3::Vector3() : x(0), y(0), z(0) {}

	float Vector3::length()
	{
		return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	}

	Vector3 Vector3::normalise()
	{
		Vector3 vect = *this;
		return vect / this->length();
	}

	Vector3 Vector3::abs()
	{
		Vector3 vect = *this;
		if (vect.x < 0)
		{
			vect.x *= -1;
		}
		if (vect.y < 0)
		{
			vect.y *= -1;
		}
		if (vect.z < 0)
		{
			vect.z *= -1;
		}
		return vect;
	}

	float dot(Vector3 a, Vector3 b)
	{
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	Vector3 cross(Vector3 a, Vector3 b)
	{
		return Vector3(a.y * b.z - a.z * b.y,
					   a.z * b.x - a.x * b.z,
					   a.x * b.y - a.y * b.x);
	}

	Vector3 reflect(Vector3 i, Vector3 n)
	{
		return i - n * (2.0f * dot(n, i));
	}
} // namespace vectors