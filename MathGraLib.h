#pragma once

#define USE_MATH_DEFINES

#include <cmath>
#include <corecrt_math_defines.h>
#include <math.h>

#include <iostream>

using namespace std;

class IntVec3
{

public:

	int a, b, c;

	IntVec3()
	{
		a = b = c = 0;
	}

	IntVec3(int v1, int v2, int v3)
	{
		a = v1;
		b = v2;
		c = v3;
	}

	int& operator[](int i)
	{
		if (i <= 0) return a;
		if (i == 1) return b;
		return c;
	}

	void Print()
	{
		cout << "x: " << a << " y: " << b << " z: " << c << endl;
	}
};

class FloatVec4
{

public:

	float x, y, z, w;

	FloatVec4()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}

	FloatVec4(float a, float b, float c, float d)
	{
		x = a;
		y = b;
		z = c;
		w = d;
	}

	float& operator[](int i)
	{
		if (i <= 0) return x;
		if (i == 1) return y;
		if (i == 2) return z;
		return w;
	}
};

// multiply function overloads
inline FloatVec4 operator* (FloatVec4 const& vec, float m)
{
	return FloatVec4(vec.x * m, vec.y * m, vec.z * m, vec.w * m);
}

inline FloatVec4 operator* (float m, FloatVec4 const& vec)
{
	return FloatVec4(vec.x * m, vec.y * m, vec.z * m, vec.w * m);
}

class FloatVec3
{

public:

	float x, y, z;

	FloatVec3()
	{
		x = 1.f;
		y = 1.f;
		z = 1.f;
	}

	FloatVec3(float px, float py, float pz)
	{
		x = px;
		y = py;
		z = pz;
	}

	void Print()
	{
		cout << "x: " << x << " y: " << y << " z: " << z << endl;
	}

	bool operator==(FloatVec3 vec)
	{
		return (this->x == vec.x && this->y == vec.y && this->z == vec.z);
	}

	bool operator!=(FloatVec3 vec)
	{
		return !(*this == vec);
	}

	float& operator[](int i)
	{
		if (i <= 0) return x;
		if (i == 1) return y;
		return z;
	}

	FloatVec3 operator+(FloatVec3 const& vec)
	{
		return FloatVec3(x + vec.x, y + vec.y, z + vec.z);
	}

	FloatVec3 operator-(FloatVec3 const& vec)
	{
		return FloatVec3(x - vec.x, y - vec.y, z - vec.z);
	}

	void operator+=(FloatVec3 const& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	void operator*=(FloatVec3 const& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}

	FloatVec3 operator/(float d)
	{
		return FloatVec3(x / d, y / d, z / d);
	}

	void operator/=(float d)
	{
		x /= d;
		y /= d;
		z /= d;
	}

	float Magnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}

	FloatVec3 Normalized()
	{
		const float epsilon = 0.000001f;
		const float mag = Magnitude();

		if(mag > epsilon)
		{
			return FloatVec3(x / mag, y / mag, z / mag);
		}

		return FloatVec3(0.f, 0.f, 0.f);
	}

	void Normalize()
	{
		const float epsilon = 0.000001f;
		const float mag = Magnitude();

		if (mag > epsilon)
		{
			x = x / mag;
			y = y / mag;
			z = z / mag;
		}
		else
		{
			x = 0;
			y = 0;
			z = 0;
		}
	}

	float Dot(FloatVec3 const& vec)
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	float AngleRad(FloatVec3 vec)
	{
		FloatVec3 a = Normalized();
		FloatVec3 b = vec.Normalized();
		return acos(a.Dot(b));
	}

	float AngleDeg(FloatVec3 const& vec)
	{
		return AngleRad(vec) / (2.0f * static_cast<float>(M_PI)) * 360.f;
	}

	FloatVec3 Cross(FloatVec3 const& vec)
	{
		return FloatVec3(y * vec.z - z * vec.y,z * vec.x - x * vec.z,x * vec.y - y * vec.x);
	}

};

// multiply function overloads
inline FloatVec3 operator* (FloatVec3 const& vec, float m)
{
	return FloatVec3(vec.x * m, vec.y * m, vec.z * m);
}

inline FloatVec3 operator* (float m, FloatVec3 const& vec)
{
	return FloatVec3(vec.x * m, vec.y * m, vec.z * m);
}

class FloatMat4
{

public:

	FloatVec4 c1, c2, c3, c4;

	FloatMat4()
	{
		c1[0] = c1[1] = c1[2] = c1[3] = 0.0f;
		c2[0] = c2[1] = c2[2] = c2[3] = 0.0f;
		c3[0] = c3[1] = c3[2] = c3[3] = 0.0f;
		c4[0] = c4[1] = c4[2] = c4[3] = 0.0f;
	}

	FloatVec4& operator[](int i)
	{
		if (i <= 0) return c1;
		if (i == 1) return c2;
		if (i == 2) return c3;
		return c4;
	}

	void Identity()
	{
		c1[0] = c1[1] = c1[2] = c1[3] = 0.0f;
		c2[0] = c2[1] = c2[2] = c2[3] = 0.0f;
		c3[0] = c3[1] = c3[2] = c3[3] = 0.0f;
		c4[0] = c4[1] = c4[2] = c4[3] = 0.0f;

		c1.x = 1.f;
		c2.y = 1.f;
		c3.z = 1.f;
		c4.w = 1.f;
	}

};

inline FloatMat4 operator*(FloatMat4 mata, FloatMat4 matb)
{
	FloatMat4 temp;

	//r1
	temp[0][0] = mata[0][0] * matb[0][0] + mata[1][0] * matb[0][1] + mata[2][0] * matb[0][2] + mata[3][0] * matb[0][3];
	temp[1][0] = mata[0][0] * matb[1][0] + mata[1][0] * matb[1][1] + mata[2][0] * matb[1][2] + mata[3][0] * matb[1][3];
	temp[2][0] = mata[0][0] * matb[2][0] + mata[1][0] * matb[2][1] + mata[2][0] * matb[2][2] + mata[3][0] * matb[2][3];
	temp[3][0] = mata[0][0] * matb[3][0] + mata[1][0] * matb[3][1] + mata[2][0] * matb[3][2] + mata[3][0] * matb[3][3];

	//r2
	temp[0][1] = mata[0][1] * matb[0][0] + mata[1][1] * matb[0][1] + mata[2][1] * matb[0][2] + mata[3][1] * matb[0][3];
	temp[1][1] = mata[0][1] * matb[1][0] + mata[1][1] * matb[1][1] + mata[2][1] * matb[1][2] + mata[3][1] * matb[1][3];
	temp[2][1] = mata[0][1] * matb[2][0] + mata[1][1] * matb[2][1] + mata[2][1] * matb[2][2] + mata[3][1] * matb[2][3];
	temp[3][1] = mata[0][1] * matb[3][0] + mata[1][1] * matb[3][1] + mata[2][1] * matb[3][2] + mata[3][1] * matb[3][3];

	//r3
	temp[0][2] = mata[0][2] * matb[0][0] + mata[1][2] * matb[0][1] + mata[2][2] * matb[0][2] + mata[3][2] * matb[0][3];
	temp[1][2] = mata[0][2] * matb[1][0] + mata[1][2] * matb[1][1] + mata[2][2] * matb[1][2] + mata[3][2] * matb[1][3];
	temp[2][2] = mata[0][2] * matb[2][0] + mata[1][2] * matb[2][1] + mata[2][2] * matb[2][2] + mata[3][2] * matb[2][3];
	temp[3][2] = mata[0][2] * matb[3][0] + mata[1][2] * matb[3][1] + mata[2][2] * matb[3][2] + mata[3][2] * matb[3][3];

	//r4
	temp[0][3] = mata[0][3] * matb[0][0] + mata[1][3] * matb[0][1] + mata[2][3] * matb[0][2] + mata[3][3] * matb[0][3];
	temp[1][3] = mata[0][3] * matb[1][0] + mata[1][3] * matb[1][1] + mata[2][3] * matb[1][2] + mata[3][3] * matb[1][3];
	temp[2][3] = mata[0][3] * matb[2][0] + mata[1][3] * matb[2][1] + mata[2][3] * matb[2][2] + mata[3][3] * matb[2][3];
	temp[3][3] = mata[0][3] * matb[3][0] + mata[1][3] * matb[3][1] + mata[2][3] * matb[3][2] + mata[3][3] * matb[3][3];

	return temp;
}

inline FloatVec4 operator*(FloatVec4 const& vec, FloatMat4 mat)
{
	FloatVec4 temp;

	temp[0] = vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + vec.w * mat[3][0];
	temp[1] = vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + vec.w * mat[3][1];
	temp[2] = vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + vec.w * mat[3][2];
	temp[3] = vec.x * mat[0][3] + vec.y * mat[1][3] + vec.z * mat[2][3] + vec.w * mat[3][3];

	return temp;
}

inline FloatVec3 operator*(FloatVec3 const& vec, FloatMat4 mat)
{
	FloatVec4 temp = FloatVec4(vec.x, vec.y, vec.z, 1.0f) * mat;

	return FloatVec3(temp.x / temp.w, temp.y / temp.w, temp.z / temp.w);
}

class Ray
{

public:

	FloatVec3 point;
	FloatVec3 dir;

	Ray(FloatVec3 p, FloatVec3 d)
	{
		point = p;
		dir = d.Normalized();
	}

	void PointAt(FloatVec3 p)
	{
		FloatVec3 pointVec = p - point;
		dir = pointVec.Normalized();
	}

};

class Sphere
{

public:

	FloatVec3 point;
	float rad;

	Sphere(FloatVec3 p, float r)
	{
		point = p;
		rad = r;
	}

	int IntersectRay(Ray ray, float t_min, float t_max, FloatVec3& pa, FloatVec3& pb)
	{
		int points = 0;
		FloatVec3 oc = ray.point - point;

		float _a = ray.dir.Dot(ray.dir);
		float _b = oc.Dot(ray.dir);
		float _c = oc.Dot(oc) - rad * rad;

		float discriminant = _b * _b - _a * _c;

		if (discriminant >= 0)
		{
			float temp = (-_b - sqrtf(discriminant)) / _a;

			if (temp < t_max && temp > t_min)
			{
				pa = ray.point + ray.dir * temp;

				points++;
			}

			if (discriminant > 0)
			{
				float temp2 = (-_b + sqrtf(discriminant)) / _a;

				if (temp2 < t_max && temp2 > t_min)
				{
					if (points == 1)
					{
						pb = ray.point + ray.dir * temp2;
					}
					else
					{
						pa = ray.point + ray.dir * temp2;
					}

					points++;
				}
			}
		}

		return points;
	}
	
};

class Plane
{

public:

	FloatVec3 point;
	FloatVec3 normal;

	Plane(FloatVec3 p, FloatVec3 n)
	{
		point = p;
		normal = n.Normalized();
	}

	int IntersectRay(Ray ray, float t_min, float t_max, FloatVec3& pa)
	{
		float dn = ray.dir.Dot(normal);

		if (dn < 0.000001f)
		{
			float temp = (point - ray.point).Dot(normal) / dn;

			pa = ray.point + (ray.dir * temp);

			return 1;
		}

		if (dn > -0.000001f && dn < 0.000001f)
		{
			float dot = normal.Dot(ray.point - point);

			if (dot > -0.000001f && dot < 0.000001f)
				return -1;
		}

		return 0;
	}

};
