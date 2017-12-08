#pragma once
#include "vector.h"
#include <math.h>
struct Quaternion
{
	Vector3 v;
	float s;

	Quaternion() {}
	Quaternion(float real, const Vector3 &i) : s(real), v(i) {}

	// euler 벡터에 각도를 넣어 Quaternion으로 변환
	Quaternion(Vector3 eulerAngleDegree)
	{

		float x = eulerAngleDegree.x * PI * 0.5f / 180.0f;
		float y = eulerAngleDegree.y * PI * 0.5f / 180.0f;
		float z = eulerAngleDegree.z * PI * 0.5f / 180.0f;

		float sinp = sin(x);
		float siny = sin(y);
		float sinr = sin(z);
		float cosp = cos(x);
		float cosy = cos(y);
		float cosr = cos(z);

		s = cosr * cosp * cosy + sinr * sinp * siny;
		v.x = sinr * cosp * cosy - cosr * sinp * siny;
		v.y = cosr * sinp * cosy + sinr * cosp * siny;
		v.z = cosr * cosp * siny - sinr * sinp * cosy;
	}

	Quaternion Normalize()
	{
		float invlength = 1 / sqrtf(s * s + v.x * v.x + v.y * v.y + v.z * v.z);
		return Quaternion(s * invlength, Vector3(v.x * invlength, v.y * invlength, v.z * invlength));
	}

	// Quaternion의 결과를 Euler 축 각도 값으로 변환
	Vector3 ToEulerDegree()
	{

		return Vector3();
	}

	static Quaternion Lerp(Quaternion &q1, Quaternion &q2, float t)
	{
		return (q1 * (1.0f - t) + q2 * t).Normalize();
	}

};