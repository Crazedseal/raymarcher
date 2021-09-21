#include <math.h>

#ifndef VECTOR2_H
#define VECTOR2_H
namespace vectors
{

	//defining functions in *.cpp is overrated
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		Vector3(float x, float y, float z);
		Vector3();

		friend Vector3 operator*(const Vector3 &a, const Vector3 &b)
		{
			Vector3 vect;
			vect.x = a.x * b.x;
			vect.y = a.y * b.y;
			vect.z = a.z * b.z;
			return vect;
		}

		friend Vector3 operator-(const Vector3 &a, const Vector3 &b)
		{
			Vector3 vect;
			vect.x = a.x - b.x;
			vect.y = a.y - b.y;
			vect.z = a.z - b.z;
			return vect;
		}

		Vector3 operator-()
		{
			Vector3 vect;
			vect.x = -this->x;
			vect.y = -this->y;
			vect.z = -this->z;
			return vect;
		}

		friend Vector3 operator*(const Vector3 &a, const float &b)
		{
			Vector3 vect;
			vect.x = a.x * b;
			vect.y = a.y * b;
			vect.z = a.z * b;
			return vect;
		}

		friend Vector3 operator*(const float &b, const Vector3 &a)
		{
			Vector3 vect;
			vect.x = a.x * b;
			vect.y = a.y * b;
			vect.z = a.z * b;
			return vect;
		}

		Vector3 operator/(const float &b)
		{
			Vector3 vect;
			vect.x = this->x / b;
			vect.y = this->y / b;
			vect.z = this->z / b;
			return vect;
		}

		Vector3 operator+(const Vector3 &b)
		{
			Vector3 vect;
			vect.x = this->x + b.x;
			vect.y = this->y + b.y;
			vect.z = this->z + b.z;
			return vect;
		}

		Vector3 operator+=(const Vector3 &b)
		{
			Vector3 vect;
			vect.x = this->x + b.x;
			vect.y = this->y + b.y;
			vect.z = this->z + b.z;
			return vect;
		}

		float length();

		Vector3 normalise();
		Vector3 abs();
	};
	float dot(Vector3 a, Vector3 b);
	Vector3 cross(Vector3 a, Vector3 b);
	Vector3 reflect(Vector3 i, Vector3 n);
} // namespace vectors
#endif