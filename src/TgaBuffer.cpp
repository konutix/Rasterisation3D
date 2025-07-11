#include "TgaBuffer.h"
#include <cstdio>
#include <stdlib.h>
#include <algorithm>
#include <array>

#include "VertexProcessor.h"

TgaBuffer::TgaBuffer(int w, int h)
{
	width = w;
	height = h;

	len = w * h;
	color.resize(len);
	depth.resize(len);
}

TgaBuffer::~TgaBuffer() = default;

int TgaBuffer::ClearColor(unsigned int col)
{
	for(int i = 0; i < len; i++)
	{
		color[i] = col;
	}

	return 0;
}

int TgaBuffer::ClearDepth(float d)
{
	for (int i = 0; i < len; i++)
	{
		depth[i] = d;
	}

	return 0;
}

bool CheckFill(float dx, float y, float yn, float dy, float x, float xn, bool geq)
{
	if (geq)
	{
		return dx * (y - yn) - dy * (x - xn) >= 0;
	}

	return dx * (y - yn) - dy * (x - xn) > 0;
}

void TgaBuffer::DrawTriangle(bool light, TgaBuffer& texBuffer, Triangle texCo, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, FloatVec3 trc, Triangle trn, Triangle trview, VertexProcessor& vp, bool pixLight, bool pLight, bool dLight)
{
	//point light center
	FloatVec3 plightc(0.f, 10.f, 25.f);
	plightc = plightc * vp.world2view;

	//dlight direction
	FloatVec3 dlight(1.f, 0.f, -0.5f);
	dlight.Normalize();
	dlight = dlight * vp.world2view;
	dlight.Normalize();

	x1 = (x1 + 1.0f) * static_cast<float>(width) * 0.5f;
	y1 = (y1 + 1.0f) * static_cast<float>(height) * 0.5f;
	x2 = (x2 + 1.0f) * static_cast<float>(width) * 0.5f;
	y2 = (y2 + 1.0f) * static_cast<float>(height) * 0.5f;
	x3 = (x3 + 1.0f) * static_cast<float>(width) * 0.5f;
	y3 = (y3 + 1.0f) * static_cast<float>(height) * 0.5f;

	float dx12 = x1 - x2;
	float dx23 = x2 - x3;
	float dx31 = x3 - x1;
	float dy12 = y1 - y2;
	float dy23 = y2 - y3;
	float dy31 = y3 - y1;

	float dx32 = x3 - x2;
	float dx13 = x1 - x3;
	float dy13 = y1 - y3;

	auto minx = static_cast<int>(std::min(x1, std::min(x2, x3)));
	auto maxx = static_cast<int>(std::max(x1, std::max(x2, x3)));
	auto miny = static_cast<int>(std::min(y1, std::min(y2, y3)));
	auto maxy = static_cast<int>(std::max(y1, std::max(y2, y3)));

	minx = std::max(minx, 0);
	maxx = std::min(maxx, width - 1);
	miny = std::max(miny, 0);
	maxy = std::min(maxy, height - 1);

	//konwencja wype³niania
	bool tl1 = false;
	bool tl2 = false;
	bool tl3 = false;
	if (dy12 < 0 || (dy12 == 0 && dx12 > 0)) { tl1 = true; }
	if (dy23 < 0 || (dy23 == 0 && dx23 > 0)) { tl2 = true; }
	if (dy31 < 0 || (dy31 == 0 && dx31 > 0)) { tl3 = true; }

	for (int x = minx; x <= maxx; x++)
	{
		for (int y = miny; y <= maxy; y++)
		{
			if (CheckFill(dx12, static_cast<float>(y), y1, dy12, static_cast<float>(x), x1, tl1) &&
				CheckFill(dx23, static_cast<float>(y), y2, dy23, static_cast<float>(x), x2, tl2) &&
				CheckFill(dx31, static_cast<float>(y), y3, dy31, static_cast<float>(x), x3, tl3))
			{
				float lam1 = (dy23 * (static_cast<float>(x) - x3) + dx32 * (static_cast<float>(y) - y3)) / (dy23 * dx13 + dx32 * dy13);
				float lam2 = (dy31 * (static_cast<float>(x) - x3) + dx13 * (static_cast<float>(y) - y3)) / (dy31 * dx23 + dx13 * dy23);
				float lam3 = 1 - lam1 - lam2;


				//calc light per fragment
				float plightint = 0;
				float dlightint = 0;
				float attenuation = 1.0f;

				FloatVec3 normal = trn.a * lam1 + trn.b * lam2 + trn.c * lam3;// normal interpolation

				FloatVec3 pixpos;

				pixpos = trview.a * lam1 + trview.b * lam2 + trview.c * lam3;

				if (pixLight && pLight)
				{
					plightint = std::max(0.0f, (pixpos - plightc).Normalized().Dot(FloatVec3(0, 0, 0) - normal));

					attenuation = 1.0f / (1.0f + 0.027f * (pixpos - plightc).Magnitude() + 0.00028f * (pixpos - plightc).Magnitude() * (pixpos - plightc).Magnitude());
				}

				if (pixLight && dLight)
				{
					dlightint = std::max(0.0f, (FloatVec3(0, 0, 0) - normal).Dot(dlight));
				}

				unsigned int col1 = 0;
				unsigned int col2 = 0;
				unsigned int col3 = 0;

				if (!pixLight)
				{
					//intensity of light on different verteces
					if (light) {
						col1 = (int)(lam1 * trc.x * 255.f) * 0x00000100;
						col2 = (int)(lam2 * trc.y * 255.f) * 0x00000100;
						col3 = (int)(lam3 * trc.z * 255.f) * 0x00000100;
					}
				}
				else
				{

					float int1col = dlightint + plightint * attenuation;

					//ambient light
					float aLight = 0.2f;
					int1col = std::min(int1col + aLight, 1.0f);

					if (!light)
						int1col = 1.0f;

					float texx = texCo.a.x * lam1 + texCo.b.x * lam2 + texCo.c.x * lam3;
					float texy = texCo.a.y * lam1 + texCo.b.y * lam2 + texCo.c.y * lam3;

					auto index = static_cast<int>(floor((1.0f - texy) * 511.0f) * 512.0f + texx * 511.0f);

					unsigned int red = ((texBuffer.color[index]) & 0x00ff0000);
					unsigned int green = ((texBuffer.color[index]) & 0x0000ff00);
					unsigned int blue = ((texBuffer.color[index]) & 0x000000ff);

					red = (((unsigned int)((float)red * int1col)) & 0x00ff0000);
					green = (((unsigned int)((float)green * int1col)) & 0x0000ff00);
					blue = (((unsigned int)((float)blue * int1col)) & 0x000000ff);

					col1 = red + green + blue;

				}

				float dep = -(lam1 * z1 + lam2 * z2 + lam3 * z3);

				if (dep > depth[x + y * width])
				{
					depth[x + y * width] = dep;
					color[x + y * width] = 0xff000000 + col1 + col2 + col3;
				}
			}
		}
	}
}

void TgaBuffer::DrawTriangle(bool light, TgaBuffer& texBuffer, Triangle texCo, Triangle t, FloatVec3 trc, Triangle trn, Triangle trview, VertexProcessor& vp, bool pixLight, bool pLight, bool dLight)
{
	DrawTriangle(light, texBuffer, texCo, t.a.x, t.a.y, t.a.z, t.b.x, t.b.y, t.b.z, t.c.x, t.c.y, t.c.z, trc, trn, trview, vp, pixLight, pLight, dLight);
}

void TgaBuffer::DrawSphereOrtho(Sphere sph1, Sphere sph2, int swidth, int sheight)
{
	float widthPixel = 2.0f / (float)swidth;
	float heightPixel = 2.0f / (float)sheight;

	for (int x = 0; x < swidth; x++)
	{
		for (int y = 0; y < sheight; y++)
		{
			float srodekX = -1.0f + (x + 0.5f) * widthPixel;
			float srodekY = 1.0f - (y + 0.5f) * heightPixel;

			Ray ray(FloatVec3(srodekX, srodekY, 0), FloatVec3(0, 0, 1));

			FloatVec3 intersetiona;
			FloatVec3 intersetionb;

			//first sphere
			int hit = sph1.IntersectRay(ray, 0.1f, 1000.0f, intersetiona, intersetionb);

			if (hit != 0)
			{
				color[x + y * swidth] = 0xff00aa00;
				depth[x + y * swidth] = intersetiona.Magnitude();
			}

			//second sphere
			hit = sph2.IntersectRay(ray, 0.1f, 1000.0f, intersetiona, intersetionb); ///////////////////NASZTYWNO

			if (hit != 0 && depth[x + y * swidth] > intersetiona.Magnitude())
			{
				color[x + y * swidth] = 0xffaa0000;
				depth[x + y * swidth] = intersetiona.Magnitude();
			}

			//bg plain
			Plane plane(FloatVec3(0.f, 0.f, 500.f), FloatVec3(0.f, 0.f, 1.f));
			hit = plane.IntersectRay(ray, intersetiona); ///////////////////NASZTYWNO

			if (hit == 1 && depth[x + y * swidth] > intersetiona.Magnitude())
			{
				color[x + y * swidth] = 0xff0000aa;
				depth[x + y * swidth] = intersetiona.Magnitude();
			}
		}
	}
}

inline FloatVec3 probe(
	float srodekX, float srodekY, Sphere sph1, Sphere sph2, Mesh* mesh, float widthPixel, float heightPixel, FloatVec3 cap, FloatVec3 w, FloatVec3 u, FloatVec3 v, int depth)
{
	std::array<FloatVec3, 5> colors;
	widthPixel /= 2.0f;
	heightPixel /= 2.0f;

	for (int i = 0; i < 5; i++)
	{
		float wfov = tan(static_cast<float>(M_PI_4));
		float hfov = tan(static_cast<float>(M_PI_4)); //vertical fov leave alone

		FloatVec3 d = u * wfov * srodekX + v * hfov * srodekY + w;

		switch(i)
		{
		case 1:
			d = u * (srodekX-widthPixel)+v * (srodekY+heightPixel)+w;
			break;
		case 2:
			d = u * (srodekX+widthPixel)+v * (srodekY+heightPixel)+w;
			break;
		case 3:
			d = u * (srodekX-widthPixel)+v * (srodekY-heightPixel)+w;
			break;
		case 4:
			d = u * (srodekX+widthPixel)+v * (srodekY-heightPixel)+w;
			break;
		default:	
			break;
		}

		Ray ray(cap, d);

		FloatVec3 intersetiona;
		FloatVec3 intersetionb;

		FloatVec3 col(170.f, 170.f, 170.f);
		float dep = 99999.0f;
		int hit = 0;

		//first sphere
		
		hit = sph1.IntersectRay(ray, 0.1f, 1000.0f, intersetiona, intersetionb);

		if (hit != 0)
		{
			col = FloatVec3(0.f,170.f,0.f);
			dep = (intersetiona - cap).Magnitude();
		}

		//second sphere
		hit = sph2.IntersectRay(ray, 0.1f, 1000.0f, intersetiona, intersetionb);

		if (hit != 0 && dep > (intersetiona - cap).Magnitude())
		{
			col = FloatVec3(170.f, 0.f, 0.f);
			dep = (intersetiona - cap).Magnitude();
		}

		//bg plain
		Plane plane(FloatVec3(0.f, 0.f, -10000.f), FloatVec3(0.f, 0.f, 1.f));
		hit = plane.IntersectRay(ray, intersetiona);

		if (hit == 1 && dep > (intersetiona - cap).Magnitude())
		{
			col = FloatVec3(0.f, 0.f, 170.f);
			dep = (intersetiona - cap).Magnitude();
		}

		//mesh
		hit = mesh->IntersectRay(ray, intersetiona);

		if (hit == 1 && dep > (intersetiona - cap).Magnitude())
		{
			col = FloatVec3(0.f, 170.f, 0.f);
		}

		colors[i] = col;
	}

	if (depth < 2)
	{
		if (colors[0] != colors[1])
		{
			colors[1] = probe((srodekX - widthPixel/2.f), (srodekY + heightPixel/2.f), sph1, sph2, mesh, widthPixel, heightPixel, cap, w, u, v, depth + 1);
		}

		if (colors[0] != colors[2])
		{
			colors[2] = probe((srodekX + widthPixel / 2.f), (srodekY + heightPixel / 2.f), sph1, sph2, mesh, widthPixel, heightPixel, cap, w, u, v, depth + 1);
		}

		if (colors[0] != colors[3])
		{
			colors[3] = probe((srodekX - widthPixel / 2.f), (srodekY - heightPixel / 2.f), sph1, sph2, mesh, widthPixel, heightPixel, cap, w, u, v, depth + 1);
		}

		if (colors[0] != colors[4])
		{
			colors[4] = probe((srodekX + widthPixel / 2.f), (srodekY - heightPixel / 2.f), sph1, sph2, mesh, widthPixel, heightPixel, cap, w, u, v, depth + 1);
		}
	}

	return (colors[1] + colors[2] + colors[3] + colors[4]) / 4.0f;
}

void TgaBuffer::DrawSpherePersp(Sphere sph1, Sphere sph2, Mesh* mesh, int swidth, int sheight, FloatVec3 cap, FloatVec3 cad, FloatVec3 vup)
{
	float widthPixel = 2.0f / (float)swidth;
	float heightPixel = 2.0f / (float)sheight;

	FloatVec3 w = (cad / (cad.Magnitude()));
	FloatVec3 u = FloatVec3(0, 0, 0) - (vup.Cross(w) / (vup.Cross(w)).Magnitude());
	FloatVec3 v = w.Cross(u);

	for (int x = 0; x < swidth; x++)
	{
		for (int y = 0; y < sheight; y++)
		{
			float srodekX = -1.0f + (static_cast<float>(x) + 0.5f) * widthPixel;
			float srodekY = 1.0f - (static_cast<float>(y) + 0.5f) * heightPixel;

			FloatVec3 colvec = probe(srodekX, srodekY, sph1, sph2, mesh, widthPixel, heightPixel, cap, w, u, v, 0);

			unsigned int red = 0x00000100 * (int)colvec.x;
			unsigned int green = 0x00010000 * (int)colvec.y;
			unsigned int blue = 0x00000001 * (int)colvec.z;

			color[x + y * swidth] = 0xff000000 + red + green + blue;
		}
	}
}

int TgaBuffer::Save()
{
	std::array<unsigned short, 9> header = {
		0x0000, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0100, 0x0100,
		 0x0820
	};

	FILE* f;
	errno_t err = fopen_s(&f, "render.tga", "wb+");

	if (f == nullptr) {
		printf("Error: %i opening file %s\n", err, "render.tga");
		return -1;
	}

	header[6] = static_cast<unsigned short>(width);
	header[7] = static_cast<unsigned short>(height);

	fwrite(header.data(), 2, 9, f);

	fwrite(color.data(), 4, len, f);

	fclose(f);

	return 0;
}

int TgaBuffer::Load(std::string const &file)
{
	std::array<unsigned short, 9> header;

	FILE* f;
	errno_t err = fopen_s(&f, file.c_str(), "rb");

	if (f == nullptr) {
		printf("Error: %i opening file %s\n", err, file.c_str());
		return -1;
	}

	fread(header.data(), 2, 9, f);

	fread(color.data(), 4, len, f);

	fclose(f);

	return 0;
}
