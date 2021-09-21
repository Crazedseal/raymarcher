#include <math.h>
#include "Vector4.h"

#ifndef MATRIX4_H
#define MATRIX4_H
namespace vectors
{
	class Matrix4
	{
	public:
		Vector4d x;
		Vector4d y;
		Vector4d z;
		Vector4d w;

		Matrix4(Vector4d x, Vector4d y, Vector4d z, Vector4d w);

		friend Vector4d operator*(const Matrix4 &m, const Vector4d &v)
		{
			Vector4d vec;
			vec.x = (m.x * v).sum();
			vec.y = (m.y * v).sum();
			vec.z = (m.z * v).sum();
			vec.w = (m.w * v).sum();
			return vec;
		}

		friend Matrix4 operator*(const Matrix4 &a, const Matrix4 &b)
		{

			Matrix4 mat = Matrix4(Vector4d(), Vector4d(), Vector4d(), Vector4d());
			mat.x = Vector4d(
				(Vector4d(a.x.x, a.y.x, a.z.x, a.w.x) * b.x).sum(),
				(Vector4d(a.x.y, a.y.y, a.z.y, a.w.y) * b.x).sum(),
				(Vector4d(a.x.z, a.y.z, a.z.z, a.w.z) * b.x).sum(),
				(Vector4d(a.x.w, a.y.w, a.z.w, a.w.w) * b.x).sum());
			mat.y = Vector4d(
				(Vector4d(a.x.x, a.y.x, a.z.x, a.w.x) * b.y).sum(),
				(Vector4d(a.x.y, a.y.y, a.z.y, a.w.y) * b.y).sum(),
				(Vector4d(a.x.z, a.y.z, a.z.z, a.w.z) * b.y).sum(),
				(Vector4d(a.x.w, a.y.w, a.z.w, a.w.w) * b.y).sum());
			mat.z = Vector4d(
				(Vector4d(a.x.x, a.y.x, a.z.x, a.w.x) * b.z).sum(),
				(Vector4d(a.x.y, a.y.y, a.z.y, a.w.y) * b.z).sum(),
				(Vector4d(a.x.z, a.y.z, a.z.z, a.w.z) * b.z).sum(),
				(Vector4d(a.x.w, a.y.w, a.z.w, a.w.w) * b.z).sum());
			mat.w = Vector4d(
				(Vector4d(a.x.x, a.y.x, a.z.x, a.w.x) * b.w).sum(),
				(Vector4d(a.x.y, a.y.y, a.z.y, a.w.y) * b.w).sum(),
				(Vector4d(a.x.z, a.y.z, a.z.z, a.w.z) * b.w).sum(),
				(Vector4d(a.x.w, a.y.w, a.z.w, a.w.w) * b.w).sum());
			return mat;
		}

		Matrix4 operator*=(const Matrix4 &b)
		{
			return *this * b;
		}

		friend Vector4d operator*(const Vector4d &v, const Matrix4 &m)
		{
			Vector4d vec;
			vec.x = m.x.x * v.x + m.y.x * v.y + m.z.x * v.z + m.w.x * v.w;
			vec.y = m.x.y * v.x + m.y.y * v.y + m.z.y * v.z + m.w.y * v.w;
			vec.z = m.x.z * v.x + m.y.z * v.y + m.z.z * v.z + m.w.z * v.w;
			vec.w = m.x.w * v.x + m.y.w * v.y + m.z.w * v.z + m.w.w * v.w;
			return vec;
		}
	};
} // namespace vectors
#endif