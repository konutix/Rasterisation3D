#pragma once
#include <list>
#include <string>
#include <vector>
#include "Triangle.h"

class Mesh
{
public:

	int vSize = 0;
	int tSize = 0;

	std::vector<FloatVec3> vertices;
	std::vector<FloatVec3> vertNormals;
	std::vector<FloatVec3> vertTex;

	std::vector<IntVec3> indices;

	Mesh() = default;

	~Mesh() = default;

	void Reset();

	void calcNormals()
	{
		for (int i = 0; i < vSize; i++)
		{
			vertNormals[i] = FloatVec3(0.f, 0.f, 0.f);
		}

		for (int i = 0; i < tSize; i++)
		{
			FloatVec3 a = vertices[indices[i].c] - vertices[indices[i].a];
			FloatVec3 b = vertices[indices[i].b] - vertices[indices[i].a];

			FloatVec3 normal = a.Cross(b);
			normal.Normalize();

			vertNormals[indices[i].a] += normal;
			vertNormals[indices[i].b] += normal;
			vertNormals[indices[i].c] += normal;
		}

		for (int i = 0; i < vSize; i++)
		{
			vertNormals[i].Normalize();
		}
	}

	void Plane(float size)
	{
		float a = size / 2.0f;

		vSize = 4;
		tSize = 2;

		Reset();

		vertices[0] = FloatVec3(-a, a, -a);
		vertices[1] = FloatVec3(-a, a, a);
		vertices[2] = FloatVec3(a, a, -a);
		vertices[3] = FloatVec3(a, a, a);

		indices[0] = IntVec3(0, 2, 1);
		indices[1] = IntVec3(1, 2, 3);

		vertTex[0] = FloatVec3(0.0f, 0.0f, 0.0f);
		vertTex[1] = FloatVec3(0.0f, 1.0f, 0.0f);
		vertTex[2] = FloatVec3(1.0f, 0.0f, 0.0f);
		vertTex[3] = FloatVec3(1.0f, 1.0f, 0.0f);
	}

	void Cube(float size)
	{
		float a = size / 2.0f;

		vSize = 8;
		tSize = 12;

		Reset();

		vertices[0] = FloatVec3(-a, a, -a);
		vertices[1] = FloatVec3(-a, a, a);
		vertices[2] = FloatVec3(-a, -a, -a);
		vertices[3] = FloatVec3(-a, -a, a);
		vertices[4] = FloatVec3(a, a, -a);
		vertices[5] = FloatVec3(a, a, a);
		vertices[6] = FloatVec3(a, -a, -a);
		vertices[7] = FloatVec3(a, -a, a);

		indices[0] = IntVec3(0, 4, 1);
		indices[1] = IntVec3(1, 4, 5);
		indices[2] = IntVec3(1, 5, 3);
		indices[3] = IntVec3(3, 5, 7);
		indices[4] = IntVec3(0, 1, 3);
		indices[5] = IntVec3(2, 0, 3);
		indices[6] = IntVec3(3, 7, 2);
		indices[7] = IntVec3(2, 7, 6);
		indices[8] = IntVec3(2, 6, 4);
		indices[9] = IntVec3(2, 4, 0);
		indices[10] = IntVec3(6, 7, 4);
		indices[11] = IntVec3(4, 7, 5);

		vertTex[0] = FloatVec3(0.0f, 0.0f, 0.0f);
		vertTex[1] = FloatVec3(0.0f, 1.0f, 0.0f);
		vertTex[2] = FloatVec3(1.0f, 0.0f, 0.0f);
		vertTex[3] = FloatVec3(1.0f, 1.0f, 0.0f);
		vertTex[4] = FloatVec3(1.0f, 0.0f, 0.0f);
		vertTex[5] = FloatVec3(1.0f, 1.0f, 0.0f);
		vertTex[6] = FloatVec3(1.0f, 0.0f, 0.0f);
		vertTex[7] = FloatVec3(0.0f, 1.0f, 0.0f);
	}

	void pyramid(float h, float r, int n)
	{
		float p2 = 2.0f * static_cast<float>(M_PI);
		float step = p2 / static_cast<float>(n);

		tSize = n * 2;
		vSize = n + 2;

		Reset();

		vertices[0] = FloatVec3(0, 0, 0);
		vertices[1] = FloatVec3(0, h, 0);

		float t = 0.0f;

		for (int i = 2; i < vSize; i++)
		{
			vertices[i] = FloatVec3(r * cos(t), 0.0f, r * sin(t));
			t += step;
		}

		for (int i = 0; i < n - 1; i++)
		{
			indices[i] = IntVec3(0, i + 3, i + 2);
			indices[n + i] = IntVec3(1, i + 2, i + 3);
		}

		indices[n - 1] = IntVec3(0, 2, n + 1);
		indices[n + n - 1] = IntVec3(1, n + 1, 2);
	}

	void cylinder(float h, float r, int n, int m)
	{
		float p2 = 2.0f * static_cast<float>(M_PI);
		float vstep = p2 / static_cast<float>(n);
		float hstep = h / static_cast<float>(m - 1);

		tSize = 2 * n * m;
		vSize = n * m + 2;

		Reset();

		vertices[0] = FloatVec3(0, 0, 0);
		vertices[1] = FloatVec3(0, h, 0);

		float vt = 0.0f;
		float ht = 0.0f;

		for (int j = 0; j < m; j++)
		{
			for (int i = 0; i < n; i++)
			{
				vertices[j * n + i + 2] = FloatVec3(r * cos(vt), ht, r * sin(vt));

				vt += vstep;
			}

			ht += hstep;
		}

		for (int i = 0; i < n - 1; i++)
		{
			indices[i] = IntVec3(0, i + 3, i + 2);

			for (int j = 0; j < m - 1; j++)
			{
				indices[n + j * n + i] = IntVec3(2 + n + i + j * n, i + 2 + j * n, i + 3 + j * n);

				indices[(m - 1) * n + n + j * n + i] = IntVec3(2 + n + i + j * n, 1 + n + i + j * n, i + 2 + j * n);
			}


			indices[2 * n * (m - 1) + i + n] = IntVec3(1, (m - 1) * n + 2 + i, (m - 1) * n + 3 + i);
		}

		indices[n - 1] = IntVec3(0, 2, n + 1);

		for (int j = 0; j < m - 1; j++)
		{
			indices[2 * n - 1 + j * n] = IntVec3(2 * n + j * n + 1, n + j * n + 1, j * n + 2 + n);

			indices[(m - 1) * n + 2 * n - 1 + j * n] = IntVec3(2 * n + j * n + 1, n + j * n + n, j * n + 1 + n);
		}

		indices[tSize - 1] = IntVec3(1, n * m + 1, (m - 1) * n + 2);

	}

	int LoadObj(std::string const& fname)
	{
		std::list<float> verts;
		std::list<int> faces;

		FILE* f;
		errno_t err = fopen_s(&f, fname.c_str(), "r");

		if (f == nullptr)
			return -1;

		char singleLine[256];

		char* data = new char[256]();
		int size = 0;

		bool newline = false;

		while (!feof(f))
		{
			fgets(singleLine, 256, f);

			if (singleLine[0] == 'v')
			{
				for (int i = 1; !newline; i++)
				{
					newline = singleLine[i] == '\n';

					if (singleLine[i] == ' ' || newline)
					{
						if (size > 0)
						{
							std::string word(data, size);

							float vert = stof(word);

							verts.push_back(vert);

							delete[] data;
							data = new char[256]();
						}

						size = 0;
					}
					else
					{
						data[size] = singleLine[i];
						size++;
					}
				}
				newline = false;
			}
			else if (singleLine[0] == 'f')
			{
				for (int i = 1; !newline; i++)
				{
					newline = singleLine[i] == '\n';

					if (singleLine[i] == ' ' || newline)
					{
						if (size > 0)
						{
							std::string word(data, size);

							int face = stoi(word) - 1;

							faces.push_back(face);

							delete[] data;
							data = new char[256]();
						}

						size = 0;
					}
					else
					{
						data[size] = singleLine[i];
						size++;
					}
				}
				newline = false;
			}
		}

		delete[] data;
		fclose(f);

		faces.pop_back();

		tSize = static_cast<int>(faces.size()) / 3;
		vSize = static_cast<int>(verts.size()) / 3;

		Reset();

		//vertices assign
		int vnum = 0;
		float a, b, c;

		a = b = c = 0.0f;

		for (float vertice : verts)
		{
			if (vnum % 3 == 0)
			{
				if (vnum != 0)
				{
					vertices[vnum / 3 - 1] = FloatVec3(a, b, c);
				}

				a = vertice;
			}
			if (vnum % 3 == 1)
				b = vertice;
			if (vnum % 3 == 2)
				c = vertice;

			vnum++;
		}
		//last
		vertices[vnum / 3 - 1] = FloatVec3(a, b, c);

		//faces assign
		int fnum = 0;
		int x, y, z;

		x = y = z = 0;

		for (int tri : faces)
		{
			if (fnum % 3 == 0)
			{
				if (fnum != 0)
				{
					indices[fnum / 3 - 1] = IntVec3(x, y, z);
				}

				x = tri;
			}
			if (fnum % 3 == 1)
				y = tri;
			if (fnum % 3 == 2)
				z = tri;

			fnum++;
		}
		//last
		indices[fnum / 3 - 1] = IntVec3(x, y, z);

		return 0;
	}

	int IntersectRay(Ray ray, FloatVec3& pa)
	{
		float depth = 99999.0f;
		int ifint = 0;
		int ifintfinal = 0;
		FloatVec3 intp;

		for (int i = 0; i < tSize; i++)
		{
			Triangle tri(vertices[indices[i].a], vertices[indices[i].b], vertices[indices[i].c]);

			FloatVec3 ref;
			ifint = tri.IntersectRay(ray, ref);

			if (ifint == 1 && (ray.point - ref).Magnitude() < depth)
			{
				depth = (ray.point - ref).Magnitude();
				ifintfinal = 1;
				intp = ref;
			}
		}

		pa = intp;
		return ifintfinal;
	}

	void Draw(bool light, TgaBuffer& buff, VertexProcessor& vp, TgaBuffer& texBuff, bool pixLight, bool pLight, bool dLight);
};
