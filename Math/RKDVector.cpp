#include "RKDVector.h"
#include <cmath>

RKDVector3::RKDVector3() :
	XMFLOAT3{ 0,0,0 }
{
}

RKDVector3::RKDVector3(float x, float y, float z) :
	XMFLOAT3{ x,y,z }
{
}

float RKDVector3::length() const
{
	return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
}

RKDVector3& RKDVector3::nomalize()
{
	float len = length();

	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

float RKDVector3::dot(const RKDVector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

RKDVector3 RKDVector3::cross(const RKDVector3& v) const
{
	return RKDVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

RKDVector3 RKDVector3::operator+() const
{
	return *this;
}

RKDVector3 RKDVector3::operator-() const
{
	return RKDVector3(-1 * x, -1 * y, -1 * z);
}

RKDVector3& RKDVector3::operator=(const XMFLOAT3& v)
{
	RKDVector3 result = RKDVector3(v.x, v.y, v.z);
	return result;
}

RKDVector3& RKDVector3::operator+=(const RKDVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

RKDVector3& RKDVector3::operator-=(const RKDVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

RKDVector3& RKDVector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

RKDVector3& RKDVector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const RKDVector3 operator+(const RKDVector3& v1, const RKDVector3& v2)
{
	RKDVector3 temp(v1);
	return temp += (v2);
}

const RKDVector3 operator-(const RKDVector3& v1, const RKDVector3& v2)
{
	RKDVector3 temp(v1);
	return temp -= (v2);
}

const RKDVector3 operator*(const RKDVector3& v, float s)
{
	RKDVector3 temp(v);
	return temp *= s;
}

const RKDVector3 operator*(float s, const RKDVector3& v)
{
	return v * s;
}

const RKDVector3 operator/(const RKDVector3& v, float s)
{
	RKDVector3 temp(v);
	return temp /= s;
}
