#include "Vector2.h"

namespace vectors
{
	Vector2d::Vector2d(float x, float y) : x(x), y(y) {}
	Vector2d::Vector2d() : x(0), y(0) {}

	float Vector2d::length()
	{
		return sqrt(this->x * this->x + this->y * this->y);
	}

	float dot(Vector2d a, Vector2d b)
	{
		return (a.x * b.x) + (a.y * b.y);
	}
} // namespace vectors