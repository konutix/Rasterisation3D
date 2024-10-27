#pragma once

#include "Triangle.h"

class TgaBuffer
{

public:

	unsigned int* color;

	float* depth;

	int width, height, minx, maxx, miny, maxy, len;

	TgaBuffer(int w, int h);
	~TgaBuffer();

	int ClearColor(unsigned int col);
	int ClearDepth(float d);

	void DrawTriangle(bool light, TgaBuffer& texBuffer, Triangle texCo, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, FloatVec3 trc, Triangle trn, Triangle trview, VertexProcessor& vp);
	void DrawTriangle(bool light, TgaBuffer& texBuffer, Triangle texCo, Triangle t, FloatVec3 trc, Triangle trn, Triangle trview, VertexProcessor& vp);

	void DrawSphereOrtho(Sphere sph1, Sphere sph2, int width, int height);
	void DrawSpherePersp(Sphere sph1, Sphere sph2, Mesh* mesh, int width, int height, FloatVec3 cap, FloatVec3 cad, FloatVec3 vup);

	int Save();
	int Load(string file);

};
