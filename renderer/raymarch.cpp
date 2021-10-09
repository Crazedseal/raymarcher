//#define DEBUG
//#define VIEWDEBUG
//#define COLDEBUG
//#define LDEBUG
//#define BIGCHUNGUSMODE

#include "objects/Vector4.h"
#include "objects/Vector3.h"
#include "objects/Vector2.h"
#include "objects/Matrix4.h"
#include "../const.h"
#include "raymarch.h"
#include <iostream>
//#include <windows.h>
#include <chrono>

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

// #define PI fRand(3, 4)
#define PI 3.14159265

using namespace vectors;

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;
int fps = 1;

int c = 0;
float timee = 0.0f;

double radians(float deg)
{
	return deg * PI / 180;
}

vectors::Vector3 maxv(Vector3 a, Vector3 b)
{
	return Vector3(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
}

vectors::Vector2d maxv(Vector2d a, Vector2d b)
{
	return Vector2d(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y);
}

float clamp(float n, float max, float min)
{
	return std::min(std::max(n, max), min);
}

float sign(float a)
{
	return a / abs(a);
}

Matrix4 rotX(float angleRaw)
{
	double angle = radians(angleRaw);
	return Matrix4(
		Vector4d(1, 0, 0, 0),
		Vector4d(0, cos(angle), -sin(angle), 0),
		Vector4d(0, sin(angle), cos(angle), 0),
		Vector4d(0, 0, 0, 1));
}

Matrix4 rotY(float angleRaw)
{
	double angle = radians(angleRaw);
	return Matrix4(
		Vector4d(cos(angle), 0, sin(angle), 0),
		Vector4d(0, 1, 0, 0),
		Vector4d(-sin(angle), 0, cos(angle), 0),
		Vector4d(0, 0, 0, 1));
}

Matrix4 rotZ(float angleRaw)
{
	double angle = radians(angleRaw);
	return Matrix4(
		Vector4d(cos(angle), -sin(angle), 0, 0),
		Vector4d(sin(angle), cos(angle), 0, 0),
		Vector4d(0, 0, 1, 0),
		Vector4d(0, 0, 0, 1));
}

float cubeSDF(Vector3 p)
{
	Vector3 d = p.abs() - Vector3(1.0, 1.0, 1.0);

	// Assuming p is inside the cube, how far is it from the surface?
	// Result will be negative or zero.
	float insideDistance = std::min(std::max(d.x, std::max(d.y, d.z)), 0.0f);

	// Assuming p is outside the cube, how far is it from the surface?
	// Result will be positive or zero.
	float outsideDistance = maxv(d, Vector3()).length();

	return insideDistance + outsideDistance;
}

float sphereSDF(Vector3 samplePoint)
{
	return samplePoint.length() - 1.0;
}

float opExtrusion(Vector3 p, float primitive, float h)
{
	float d = primitive;
	Vector2d w = Vector2d(d, abs(p.z) - h);
	return std::min(std::max(w.x, w.y), 0.0f) + maxv(w, Vector2d(0.0, 0.0)).length();
}

float sdTriangleIsosceles(Vector2d p, Vector2d q)
{
	p.x = abs(p.x);
	Vector2d a = p - q * clamp(dot(p, q) / dot(q, q), 0.0, 1.0);
	Vector2d b = p - q * Vector2d(clamp(p.x / q.x, 0.0, 1.0), 1.0);
	float k = sign(q.y);
	float d = std::min(dot(a, a), dot(b, b));
	float s = std::max(k * (p.x * q.y - p.y * q.x), k * (p.y - q.y));
	return sqrt(d) * sign(s);
}

float sdEllipsoidApproximated(Vector2d p, Vector2d r)
{
	float k0 = (p / r).length();
	float k1 = (p / (r * r)).length();
	return k0 * (k0 - 1.0) / k1;
}

float sdArchLogo(Vector2d p)
{
	Vector2d tri = Vector2d(0.5, -1.1);	 // width, height
	Vector2d tri2 = Vector2d(0.5, -0.1); // width, height
	Vector2d el = Vector2d(0.7, 0.5);
	Vector2d el2 = Vector2d(0.2, 0.4);
	return std::max(std::max(std::max(std::max(sdTriangleIsosceles(p - Vector2d(0.0, 0.5), tri), -sdEllipsoidApproximated(p - Vector2d(0.0, -0.95), el)), -sdEllipsoidApproximated(p - Vector2d(0.0, -0.65), el2)), -sdTriangleIsosceles(p - Vector2d(0.75, -0.2), tri2)), -sdTriangleIsosceles(p - Vector2d(-0.55, 0.2), tri2));
}

float sdHexagram(Vector2d p)
{
	Vector2d tri = Vector2d(0.75, -1); // width, height
	Vector2d tri2 = Vector2d(0.75, 1); // width, height

	return std::min(sdTriangleIsosceles(p - Vector2d(0.0, 0.5), tri), sdTriangleIsosceles(p - Vector2d(0.0, -0.75), tri2));
}

float sceneSDF(Vector3 location)
{
#ifdef DEBUG
	std::cout << location.x << "|" << location.y << "|" << location.z << "=" << location.length() << std::endl;
#endif
	float scale = 4.0f;
	Vector3 rotLoc = (rotY(-timee * 30) * Vector4d(location, 1.0)).toVector3() / scale;
	return opExtrusion(rotLoc, sdArchLogo(Vector2d(-rotLoc.x, -rotLoc.y)), 0.1f) * scale;
	//return opExtrusion(rotLoc, sdHexagram(Vector2d(-rotLoc.x, -rotLoc.y)), 0.1f) * scale;
	//return cubeSDF(rotLoc);
}

Vector3 rayDirection(float fieldOfView, Vector2d size, Vector2d fragCoord)
{
	Vector2d xy = fragCoord - size / 2.0;
#ifdef DEBUG
	std::cout << xy.x << "|" << xy.y << std::endl;
#endif
	float z = size.y / tan(radians(fieldOfView) / 2.0);
	return Vector3(xy.x, xy.y, -z).normalise();
}

float shortestDistanceToSurface(float start, float end, Vector3 viewRayDirection, Vector3 eye)
{
	float depth = start;
	for (int i = 0; i < MAX_MARCHING_STEPS; i++)
	{
		float dist = sceneSDF(eye + viewRayDirection * depth);
		if (dist < EPSILON)
		{
			return depth;
		}
		depth += dist;
		if (depth >= end)
		{
			return end;
		}
	}
	return end;
}

Vector3 estimateNormal(Vector3 p)
{
	Vector3 result = Vector3(
		sceneSDF(Vector3(p.x + EPSILON, p.y, p.z)) - sceneSDF(Vector3(p.x - EPSILON, p.y, p.z)),
		sceneSDF(Vector3(p.x, p.y + EPSILON, p.z)) - sceneSDF(Vector3(p.x, p.y - EPSILON, p.z)),
		sceneSDF(Vector3(p.x, p.y, p.z + EPSILON)) - sceneSDF(Vector3(p.x, p.y, p.z - EPSILON)));
	return result.normalise();
}

Vector3 lambert(Vector3 n)
{
	Vector3 lightDir(8, 0, 7);		 // Light direction
	Vector3 lightCol(1.0, 1.0, 1.0); // Light color

	float NdotL = dot(n, lightDir);
	if (NdotL < 0)
		NdotL = 0;
	Vector3 c;
	c = Vector3(1.0, 0, 0) * lightCol * NdotL;
	return c;
}

Vector3 phongContribForLight(Vector3 k_d, Vector3 k_s, float alpha, Vector3 p, Vector3 eye,
							 Vector3 lightPos, Vector3 lightIntensity)
{
	Vector3 N = estimateNormal(p);
	Vector3 L = lightPos - p;
	L = L.normalise();
	Vector3 V = eye - p;
	V = V.normalise();
	Vector3 R = reflect(-L, N);
	R = R.normalise();

	float dotLN = dot(L, N);
	float dotRV = dot(R, V);
#ifdef LDEBUG
	std::cout << L.x << "|" << L.y << "|" << L.z << "=" << L.length() << std::endl;
	std::cout << N.x << "|" << N.y << "|" << N.z << "=" << N.length() << std::endl;
	std::cout << dotLN << std::endl;
	std::cout << std::endl;
#endif

	if (dotLN < 0.0)
	{
		// Light not visible from this point on the surface
		return Vector3(0.0, 0.0, 0.0);
	}

	if (dotRV < 0.0)
	{
		// Light reflection in opposite direction as viewer, apply only diffuse
		// component
		return lightIntensity * (k_d * dotLN);
	}
	return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

Vector3 phongIllumination(Vector3 k_a, Vector3 k_d, Vector3 k_s, float alpha, Vector3 p, Vector3 eye)
{
	const Vector3 ambientLight = 0.5 * Vector3(1.0, 1.0, 1.0);
	Vector3 color = ambientLight * k_a;
#ifdef COLDEBUG
	std::cout << color.x << "|" << color.y << "|" << color.z << std::endl;
	std::cout << std::endl;
#endif

	Vector3 light1Pos = Vector3(4.0 * sin(timee),
								2.0,
								4.0 * cos(timee));
	Vector3 light1Intensity = Vector3(0.4, 0.4, 0.4);

	color = color + phongContribForLight(k_d, k_s, alpha, p, eye,
										 light1Pos,
										 light1Intensity);
#ifdef COLDEBUG
	std::cout << color.x << "|" << color.y << "|" << color.z << std::endl;
	std::cout << std::endl;
#endif

	Vector3 light2Pos = Vector3(2.0 * sin(0.37 * timee),
								2.0 * cos(0.37 * timee),
								2.0);
	Vector3 light2Intensity = Vector3(0.4, 0.4, 0.4);

	color = color + phongContribForLight(k_d, k_s, alpha, p, eye,
										 light2Pos,
										 light2Intensity);
#ifdef COLDEBUG
	std::cout << color.x << "|" << color.y << "|" << color.z << std::endl;
	std::cout << std::endl;
#endif
	return color;
}

Vector3 renderSurf(Vector3 p)
{
	Vector3 n = estimateNormal(p);
	return lambert(n);
}

void frame(int fpss)
{
	c++;
	fps = fpss;
	timee += 1.0f / (float)fps;
}

Matrix4 viewMatrix(Vector3 eye, Vector3 center, Vector3 up)
{
	Vector3 f = center - eye;
	f = f.normalise();
	Vector3 s = cross(f, up);
	s = s.normalise();
	Vector3 u = cross(s, f);
#ifdef VIEWDEBUG
	std::cout << f.x << "|" << f.y << "|" << f.z << "=" << f.length() << std::endl;
	std::cout << s.x << "|" << s.y << "|" << s.z << "=" << s.length() << std::endl;
	std::cout << u.x << "|" << u.y << "|" << u.z << "=" << u.length() << std::endl;
	std::cout << std::endl;
#endif
	return Matrix4(
		Vector4d(s, 0.0),
		Vector4d(u, 0.0),
		Vector4d(-f, 0.0),
		Vector4d(0.0, 0.0, 0.0, 1));
}

Vector3 rotVec3Z(Vector3 v, float angle)
{
	return (rotZ(angle) * Vector4d(v, 1.0)).toVector3();
}

Vector3 mainImage(Vector2d frag)
{
	Vector3 dir = rayDirection(45.0, RESOLUTION, frag);
	Vector3 eye = Vector3(2.5, 0.0, 5.0);

	Matrix4 viewToWorld = viewMatrix(eye, Vector3(2.5, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));

	dir = rotVec3Z(dir, -90);

	Vector3 worldDir = (viewToWorld * Vector4d(dir, 1.0)).toVector3();
#ifdef DEBUG
	std::cout << worldDir.x << "|" << worldDir.y << "|" << worldDir.z << "=" << worldDir.length() << std::endl;
	std::cout << dir.x << "|" << dir.y << "|" << dir.z << "=" << dir.length() << std::endl;
	std::cout << std::endl;
#endif
	float dist = shortestDistanceToSurface(MIN_DIST, MAX_DIST, worldDir, eye);

	if (dist > MAX_DIST - EPSILON)
	{
		// Didn't hit anything
		return Vector3(0.05, 0.05, 0.05);
	}

	//Vector3 color = renderSurf(eye + dir * dist);
	Vector3 p = eye + dist * dir;

	Vector3 K_a = Vector3(0.2, 0.2, 0.2);
	Vector3 K_d = Vector3(0.09, 0.58, 0.82);
	Vector3 K_s = Vector3(1.0, 1.0, 1.0);
	float shininess = 10.0;

	Vector3 color = phongIllumination(K_a, K_d, K_s, shininess, p, eye);

	return color;
}