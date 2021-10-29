#include "Matrix4.h"

namespace vectors
{
	Matrix4::Matrix4(Vector4d x, Vector4d y, Vector4d z, Vector4d w) : x(Vector4d(x.x, y.x, z.x, w.x)), y(Vector4d(x.y, y.y, z.y, w.y)), z(Vector4d(x.z, y.z, z.z, w.z)), w(Vector4d(x.w, y.w, z.w, w.w)) {}
} // namespace vectors