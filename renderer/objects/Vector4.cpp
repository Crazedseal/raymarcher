#include "Vector4.h"

namespace vectors
{
	Vector4d::Vector4d(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4d::Vector4d(Vector3 vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
	Vector4d::Vector4d() : x(0), y(0), z(0), w(0) {}

	float Vector4d::sum()
	{
		return this->x + this->y + this->z + this->w;
	}

	Vector3 Vector4d::toVector3()
	{
		Vector3 vec;
		vec.x = this->x;
		vec.y = this->y;
		vec.z = this->z;
		return vec;
	}
} // namespace vectors