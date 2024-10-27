#include "Triangle.h"
#include "tgaBuffer.h"
#include "VertexProcessor.h"

using namespace std;

void Mesh::Draw(bool light, TgaBuffer& buffer, VertexProcessor& vp, TgaBuffer& texBuffer)
{
	FloatVec3 plightc(0.f, 10.f, 25.f);

	FloatVec3 dlight(1.f, 0.f, -0.5f);

	dlight.Normalize();

	calcNormals();

	for(int i = 0; i < tSize; i++)
	{
		Triangle tr(vertices[indices[i].a], vertices[indices[i].b], vertices[indices[i].c]);
		Triangle trn(vertNormals[indices[i].a], vertNormals[indices[i].b], vertNormals[indices[i].c]);
		Triangle texCo(vertTex[indices[i].a], vertTex[indices[i].b], vertTex[indices[i].c]);

		tr.Transform(vp.obj2world);

		trn.Transform(vp.rotation);

		//calc light per vertex
		float plightint1 = 0;
		float plightint2 = 0;
		float plightint3 = 0;

		float dlightintl = 0;
		float dlightint2 = 0;
		float dlightint3 = 0;

		if (true)
		{
			plightint1 = max(0.0f, (tr.a - plightc).Normalized().Dot(FloatVec3(0, 0, 0) - trn.a));
			plightint2 = max(0.0f, (tr.b - plightc).Normalized().Dot(FloatVec3(0, 0, 0) - trn.b));
			plightint3 = max(0.0f, (tr.c - plightc).Normalized().Dot(FloatVec3(0, 0, 0) - trn.c));
		}

		float attenuation1 = 1.0f / (1.0f + 0.027f * (tr.a - plightc).Magnitude() + 0.00028f * (tr.a - plightc).Magnitude() * (tr.a - plightc).Magnitude());
		float attenuation2 = 1.0f / (1.0f + 0.027f * (tr.b - plightc).Magnitude() + 0.00028f * (tr.b - plightc).Magnitude() * (tr.b - plightc).Magnitude());
		float attenuation3 = 1.0f / (1.0f + 0.027f * (tr.c - plightc).Magnitude() + 0.00028f * (tr.c - plightc).Magnitude() * (tr.c - plightc).Magnitude());

		if (false)
		{
			dlightintl = max(0.0f, (FloatVec3(0, 0, 0) - trn.a).Dot(dlight));
			dlightint2 = max(0.0f, (FloatVec3(0, 0, 0) - trn.b).Dot(dlight));
			dlightint3 = max(0.0f, (FloatVec3(0, 0, 0) - trn.c).Dot(dlight));
		}

		float vert1col = dlightintl + plightint1 * attenuation1;
		float vert2col = dlightint2 + plightint2 * attenuation2;
		float vert3col = dlightint3 + plightint3 * attenuation3;

		FloatVec3 trc;

		trc.x = min(1.0f, vert1col);
		trc.y = min(1.0f, vert2col);
		trc.z = min(1.0f, vert3col);


		tr.Transform(vp.world2view);

		Triangle trview = tr;

		tr.Transform(vp.view2proj);

		buffer.DrawTriangle(light, texBuffer, texCo, tr, trc, trn, trview, vp);
	}
}
