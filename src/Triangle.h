#pragma once
#include <list>
#include <string>
#include <vector>

#include "MathGraLib.h"

#define PLUS_ZERO 0.00001
#define MINUS_ZERO -0.0001

class TgaBuffer;
class VertexProcessor;

class Triangle
{
public:

	FloatVec3 a;
	FloatVec3 b;
	FloatVec3 c;

	Triangle()
	{
		a = b = c = FloatVec3(0,0,0);
	}

	Triangle(FloatVec3 v1, FloatVec3 v2, FloatVec3 v3)
		: a(v1), b(v2), c(v3)
	{}

	FloatVec3& operator[](int i)
	{
		if (i <= 0) return a;
		if (i == 1) return b;
		return c;
	}

	void Transform(FloatMat4 const& mat)
	{
		a = a * mat;
		b = b * mat;
		c = c * mat;
	}

	int IntersectRay(Ray ray, FloatVec3& pa)
	{
		FloatVec3 na = c - a;
		FloatVec3 nb = b - a;

		FloatVec3 norm = na.Cross(nb);
		Plane plane(a,norm);

		FloatVec3 intpoint;
		int hit = plane.IntersectRay(ray, intpoint);

		if(hit == 1)
		{
			FloatVec3 fa = a - intpoint;
			FloatVec3 fb = b - intpoint;
			FloatVec3 fc = c - intpoint;

			fa.Normalize();
			fb.Normalize();
			fc.Normalize();

			FloatVec3 x = fa.Cross(fb);
			if(x.Dot(plane.normal) > PLUS_ZERO)
			{
				return 0;
			}

			x = fb.Cross(fc);
			if(x.Dot(plane.normal) > PLUS_ZERO)
			{
				return 0;
			}

			x = fc.Cross(fa);
			if (x.Dot(plane.normal) > PLUS_ZERO)
			{
				return 0;
			}

			pa = intpoint;
			return 1;
		}
		else
		{
			return -1;
		}
	}
};
