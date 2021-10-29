#include <math.h>

#ifndef VECTOR2D_H
#define VECTOR2D_H
namespace vectors
{
	class Vector2d
	{
	public:
		float x;
		float y;

		Vector2d(float x, float y);
		Vector2d();

		Vector2d operator/(const float &b)
		{
			Vector2d vect;
			vect.x = this->x / b;
			vect.y = this->y / b;
			return vect;
		}

		Vector2d operator*(const float &b)
		{
			Vector2d vect;
			vect.x = this->x * b;
			vect.y = this->y * b;
			return vect;
		}

		Vector2d operator/(const Vector2d &b)
		{
			Vector2d vect;
			vect.x = this->x / b.x;
			vect.y = this->y / b.y;
			return vect;
		}

		Vector2d operator*(const Vector2d &b)
		{
			Vector2d vect;
			vect.x = this->x * b.x;
			vect.y = this->y * b.y;
			return vect;
		}

		Vector2d operator-(const Vector2d &b)
		{
			Vector2d vect;
			vect.x = this->x - b.x;
			vect.y = this->y - b.y;
			return vect;
		}

		float length();
	};
	float dot(Vector2d a, Vector2d b);
} // namespace vectors
#endif