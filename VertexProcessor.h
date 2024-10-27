#pragma once

#include "Triangle.h"

class VertexProcessor
{

public:

	FloatMat4 rotation;
	FloatMat4 obj2world, world2view, view2proj;

	VertexProcessor()
	{
		rotation.Identity();

		obj2world.Identity();
		world2view.Identity();
		view2proj.Identity();
	}

	void SetPerspective(float fovy, float aspect, float near, float far)
	{
		FloatMat4 viewproj;

		fovy *= static_cast<float>(M_PI) / 360.0f;
		const float f = cos(fovy) / sin(fovy);

		viewproj[0][0] = f / aspect;
		viewproj[1][1] = f;
		viewproj[2][2] = (far + near) / (near - far);
		viewproj[2][3] = -1;
		viewproj[3][2] = 2 * near * far / (near - far);

		view2proj = viewproj;
	}

	void SetLookat(FloatVec3 eye, FloatVec3 center, FloatVec3 up)
	{
		FloatMat4 worldview;

		FloatVec3 f = center - eye;

		f.Normalize();
		up.Normalize();

		FloatVec3 s = f.Cross(up);
		FloatVec3 u = s.Cross(f);

		worldview[0] = FloatVec4(s[0], u[0], -f[0], 0);
		worldview[1] = FloatVec4(s[1], u[1], -f[1], 0);
		worldview[2] = FloatVec4(s[2], u[2], -f[2], 0);
		worldview[3] = FloatVec4(0, 0, 0, 1.f);

		FloatMat4 m;
		m.Identity();
		m[3] = FloatVec4(-eye[0], -eye[1], -eye[2], 1.f);
		worldview = worldview * m;

		world2view = worldview;
	}

	void multByTranslation(FloatVec3 v)
	{
		FloatMat4 m;

		m[0] = FloatVec4(1.f, 0, 0, 0);
		m[1] = FloatVec4(0, 1.f, 0, 0);
		m[2] = FloatVec4(0, 0, 1.f, 0);
		m[3] = FloatVec4(v.x, v.y, v.z, 1);

		obj2world = m * obj2world;
	}

	void multByScale(FloatVec3 v)
	{
		FloatMat4 m;

		m[0] = FloatVec4(v.x, 0, 0, 0);
		m[1] = FloatVec4(0, v.y, 0, 0);
		m[2] = FloatVec4(0, 0, v.z, 0);
		m[3] = FloatVec4(0, 0, 0, 1);

		obj2world = m * obj2world;
	}

	void multByRotation(float a, FloatVec3 vec)
	{
		float s = sin(a * static_cast<float>(M_PI) / 180.f);
		float c = cos(a * static_cast<float>(M_PI) / 180.f);

		FloatVec3 v = vec.Normalized();

		FloatMat4 m;

		m[0] = FloatVec4(v.x * v.x * (1 - c) + c, v.y * v.x * (1 - c) + v.z * s, v.x * v.z * (1 - c) - v.y * s, 0.f);
		m[1] = FloatVec4(v.x * v.y * (1 - c) - v.z * s, v.y * v.y * (1 - c) + c, v.y * v.z * (1 - c) + v.x * s, 0);
		m[2] = FloatVec4(v.x * v.z * (1 - c) + v.y * s, v.y * v.z * (1 - c) - v.x * s, v.z * v.z * (1 - c) + c, 0);
		m[3] = FloatVec4(0, 0, 0, 1);

		obj2world = m * obj2world;
		rotation = m * rotation;
	}
};

