#pragma once
#include "stdafx.h"
#include "vector.h"

class SphericalCoordinate
{
public :
	SphericalCoordinate(Vector3 cartesian)
	{
		radius = sqrtf(cartesian.x * cartesian.x
							+ cartesian.y * cartesian.y
							+ cartesian.z * cartesian.z);

		azimuth = atan2f(cartesian.z, cartesian.x);
		elevation = asinf(cartesian.y / radius);
	}

	Vector3 ToCartesian()
	{
		float t = radius * cosf(elevation);
		Vector3 temp;
		temp.x = t * cosf(azimuth);
		temp.y = radius * sinf(elevation);
		temp.z = t * sinf(azimuth);
		return temp;
	}

	Vector3 AddRotationYaw(float speed)
	{
		azimuth += Deg2Rad(speed);
		return ToCartesian();
	}

	Vector3 AddRotationPitch(float speed)
	{
		elevation += Deg2Rad(speed);
		return ToCartesian();
	}

	Vector3 Zoom(float speed)
	{
		radius += speed;
		return ToCartesian();
	}

private :
	float radius, azimuth, elevation;
};