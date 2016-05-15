#pragma once

#include <cmath>

class Vector3
{
public:
	Vector3(float X = 0.0f, float Y = 0.0f, float Z = 0.0f)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vector3 operator +=(const Vector3 &vec)
	{
		return (*this = (*this + vec));
	}

	Vector3 operator +(const Vector3 &vec)
	{
		return Vector3(vec.x + x, vec.y + y, vec.z + z);
	}

	Vector3 operator -=(const Vector3 &vec)
	{
		return (*this = (*this - vec));
	}

	Vector3 operator -(const Vector3 &vec)
	{
		return Vector3(x- vec.x, y - vec.y, z - vec.z);
	}

	Vector3 operator *=(float num)
	{
		return (*this = (*this * num));
	}

	Vector3 operator *(float num)
	{
		return Vector3(x * num, y * num, z * num);
	}

	Vector3 operator /=(float num)
	{
		return (*this = (*this / num));
	}

	Vector3 operator /(float num)
	{
		return Vector3(x / num, y / num, z / num);
	}

	Vector3 operator -()
	{
		return Vector3(-x, -y, -z);
	}

	float Dot(Vector3 &vec)
	{
		return (x * vec.x + y * vec.y + z * vec.z);
	}

	Vector3 operator *(const Vector3 &vec)
	{
		return Vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	}

	float Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 Normalize()
	{
		float length = Length();

		x /= length;
		y /= length;
		z /= length;

		return *this;
	}

	float Distance(Vector3 &vec)
	{
		float distX = vec.x - x;
		float distY = vec.y - y;
		float distZ = vec.z - z;

		return sqrt(distX * distX + distY * distY + distZ * distZ);
	}

	bool operator ==(Vector3 &vec)
	{
		return (vec.x == x && vec.y == y && vec.z == z);
	}

	bool operator !=(Vector3 &vec)
	{
		return !(vec == *this);
	}

public:
	float x, y, z;
};