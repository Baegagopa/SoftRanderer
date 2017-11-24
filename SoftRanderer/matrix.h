#pragma once

class Matrix3 {
public:
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	Matrix3() {};
	Matrix3(float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33)
	{
		_11 = m11;
		_12 = m12;
		_13 = m13;

		_21 = m21;
		_22 = m22;
		_23 = m23;

		_31 = m31;
		_32 = m32;
		_33 = m33;
	};

	~Matrix3() {};

	void SetTranslation(float a, float b)
	{
		Identity();
		// Add Code Here
		_13 = a;
		_23 = b;
	}

	void SetScale(float a, float b)
	{
		Identity();
		// Add Code Here
		_11 = a;
		_22 = b;
	}

	void SetRotation(float degree)
	{
		float radian = degree * 3.141592f / 180.0f;
		Identity();
		// Add Code Here
		_11 = cosf(radian);
		_12 = -sinf(radian);
		_21 = sinf(radian);
		_22 = cosf(radian);
	}

	void Identity()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;

		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;

		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
	};

	Matrix3 operator*(const Matrix3 &m) const
	{
		Matrix3 temp;

		temp._11 = _11 * m._11 + _12 * m._21 + _13 * m._31;
		temp._12 = _11 * m._12 + _12 * m._22 + _13 * m._32;
		temp._13 = _11 * m._13 + _12 * m._23 + _13 * m._33;

		temp._21 = _21 * m._11 + _22 * m._21 + _23 * m._31;
		temp._22 = _21 * m._12 + _22 * m._22 + _23 * m._32;
		temp._23 = _21 * m._13 + _22 * m._23 + _23 * m._33;

		temp._31 = _31 * m._11 + _32 * m._21 + _33 * m._31;
		temp._32 = _31 * m._12 + _32 * m._22 + _33 * m._32;
		temp._33 = _31 * m._13 + _32 * m._23 + _33 * m._33;

		return temp;
	};
};

class Matrix4 {
public:
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	Matrix4() {};
	Matrix4(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44)
	{
		_11 = m11;
		_12 = m12;
		_13 = m13;
		_14 = m14;

		_21 = m21;
		_22 = m22;
		_23 = m23;
		_24 = m24;

		_31 = m31;
		_32 = m32;
		_33 = m33;
		_34 = m34;

		_41 = m41;
		_42 = m42;
		_43 = m43;
		_44 = m44;
	};

	~Matrix4() {};

	void SetTranslation(float a, float b, float c)
	{
		Identity();
		// Add Code Here
		_14 = a;
		_24 = b;
		_34 = c;
	}

	void SetScale(float a, float b, float c)
	{
		Identity();
		// Add Code Here
		_11 = a;
		_22 = b;
		_33 = c;
	}

	void SetRotationX(float degree)
	{
		float radian = degree * 3.141592f / 180.0f;
		Identity();
		float s = sinf(radian);
		float c = cosf(radian);
		_22 = c;
		_32 = -s;
		_23 = s;
		_33 = c;
	}

	void SetRotationY(float degree)
	{
		float radian = degree * 3.141592f / 180.0f;
		Identity();
		float s = sinf(radian);
		float c = cosf(radian);
		_11 = c;
		_13 = -s;
		_31 = s;
		_33 = c;
	}

	void SetRotationZ(float degree)
	{
		float radian = degree * 3.141592f / 180.0f;
		Identity();
		float s = sinf(radian);
		float c = cosf(radian);
		_11 = c;
		_12 = -s;
		_21 = s;
		_22 = c;
	}

	void Identity()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_14 = 0.0f;

		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;
		_24 = 0.0f;

		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
		_34 = 0.0f;

		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
	};

	Matrix4 operator*(const Matrix4 &m) const
	{
		Matrix4 temp;

		temp._11 = _11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41;
		temp._12 = _11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42;
		temp._13 = _11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43;
		temp._14 = _11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44;

		temp._21 = _21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41;
		temp._22 = _21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42;
		temp._23 = _21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43;
		temp._24 = _21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44;

		temp._31 = _31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41;
		temp._32 = _31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42;
		temp._33 = _31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43;
		temp._34 = _31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44;

		temp._41 = _41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41;
		temp._42 = _41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42;
		temp._43 = _41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43;
		temp._44 = _41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44;

		return temp;
	};
};

