#pragma once
#define M_PI		3.14159265358979323846	
#include <Windows.h>
#include <math.h>
struct BaseMatrix
{
	float at[16]{};
};


class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length() {
		return sqrt(x * x + y * y + z * z);
	}
	//inline FRotator ToFRotator() {

	//}

	void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}
	float DistTo(Vector3 ape)
	{
		return (*this - ape).Length();
	}
	float distance(Vector3 vec)
	{
		return sqrt(
			pow(vec.x - x, 2) +
			pow(vec.y - y, 2)
		);
	}

	

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float flNum) { return Vector3(x * flNum, y * flNum, z * flNum); }
};
class Vector2
{
public:
	Vector2() : x(0.f), y(0.f)
	{

	}

	Vector2(float _x, float _y) : x(_x), y(_y)
	{

	}
	~Vector2()
	{

	}

	float x;
	float y;
	Vector2 operator+(int i) {
		return { x + i, y + i };
	}
	Vector2 operator-(Vector2 v) {
		return { x - v.x, y - v.y };
	}
	Vector2 flip() {
		return { y, x };
	}

};
struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};
float GetCrossDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}
inline float dist(const Vector3 p1, const Vector3 p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;
	return sqrt(x * x + y * y + z * z);
}

Vector2 CalcAngle(const Vector3& src, const Vector3& dst)
{
	Vector2 angle;
	Vector3 delta = Vector3((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	angle.x = atan(delta.z / hyp) * (180.0f / M_PI);
	angle.y = atan(delta.y / delta.x) * (180.0f / M_PI);
	if (delta.x >= 0.0) angle.y += 180.0f;

	return angle;
}

void NormalizeAngles(Vector2& angle)
{
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

void normalY(float& f) {
	while (f > 180.f)
		f -= 360.f;

	while (f < -180.f)
		f += 360.f;
}