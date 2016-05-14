#pragma once

#include <cmath>

class Vector2
{
public:
	Vector2(float X = 0.0f, float Y = 0.0f)
	{
		x = X;
		y = Y;
	}

	Vector2 operator +=(const Vector2 &vec)
	{
		return (*this = (*this + vec));
	}

	Vector2 operator +(const Vector2 &vec)
	{
		return Vector2(vec.x + x, vec.y + y);
	}

	Vector2 operator -=(const Vector2 &vec)
	{
		return (*this = (*this - vec));
	}

	Vector2 operator -(const Vector2 &vec)
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	Vector2 operator *=(float num)
	{
		return (*this = (*this * num));
	}

	Vector2 operator *(float num)
	{
		return Vector2(x * num, y * num);
	}

	Vector2 operator /=(float num)
	{
		return (*this = (*this / num));
	}

	Vector2 operator /(float num)
	{
		return Vector2(x / num, y / num);
	}

	Vector2 operator -()
	{
		return Vector2(-x, -y);
	}

	float Dot(Vector2 &vec)
	{
		return (x * vec.x + y * vec.y);
	}

	Vector2 operator *(const Vector2 &vec)
	{
		return Vector2(y * vec.x - x * vec.y, x * vec.y - y * vec.x);
	}

	float Length()
	{
		return sqrt(x * x + y * y);
	}

	Vector2 Normalize()
	{
		float length = Length();

		x /= length;
		y /= length;

		return *this;
	}

	float Distance(Vector2 &vec)
	{
		float distX = vec.x - x;
		float distY = vec.y - y;

		return sqrt(distX * distX + distY * distY);
	}

	bool operator ==(Vector2 &vec)
	{
		return (vec.x == x && vec.y == y);
	}

	bool operator !=(Vector2 &vec)
	{
		return !(vec == *this);
	}

public:
	float x, y;
};