#pragma once

#include "matrix.h"

struct Vector3
{
	float x, y, z;
	Vector3() { x = y = z = 0; }
	Vector3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}

	Vector3 operator*(const Matrix3 &m) const
	{
		Vector3 temp;

		temp.x = x * m._11 + y * m._12 + z * m._13; 
		temp.y = x * m._21 + y * m._22 + z * m._23; 
		temp.z = x * m._31 + y * m._32 + z * m._33; 

		return temp;
	}

	Vector3 operator*(const Matrix4 &m) const
	{
		Vector3 temp;

		temp.x = x * m._11 + y * m._12 + z * m._13;
		temp.y = x * m._21 + y * m._22 + z * m._23;
		temp.z = x * m._31 + y * m._32 + z * m._33;

		return temp;
	}

	void Set(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}

	void SetPoint(float a, float b)
	{
		// Add Code Here
		x = a;
		y = b;
		z = 1.0f;
	}

	void SetVector(float a, float b)
	{
		// Add Code Here
		x = a;
		y = b;
		z = 0.0f;
	}

	bool IsPoint()
	{
		return (z == 0.0f) ? false : true;
	}

	Vector3 operator-(const Vector3 &rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Vector3 operator^(const Vector3 &rhs) const
	{
		Vector3 rvalue;

		rvalue.x = y * rhs.z - z * rhs.y;
		rvalue.y = z * rhs.x - x * rhs.z;
		rvalue.z = x * rhs.y - y * rhs.x;

		return rvalue;
	}

	Vector3 Normalize(void)
	{
		float invlength = 1 / sqrtf(x * x + y * y + z * z);
		return Vector3(x * invlength, y * invlength, z * invlength);
	}

};

