#include <iostream>
#include "TgaBuffer.h"
#include "VertexProcessor.h"

const int width = 1024;
const int height = 1024;

int rayTracer();
int meshRender();

int main()
{
    //return rayTracer();
    return meshRender();
}

int rayTracer() //fotoreal
{
    TgaBuffer buffer(width, height);

    buffer.ClearColor(0xffaaaaaa);
    buffer.ClearDepth(99999.0f);

    //Loaded mesh
    Mesh* m = new Mesh();
    if (m->LoadObj("triangle.obj") == -1)
        return -1;

    //Sphere sph(FloatVec3(0.f, 0, 30.0f), 0.3f);
    //Sphere sph2(FloatVec3(0.35f, 0, 10.0f), 0.2f);

    //buffer.DrawSphereOrtho(sph, sph2, width, height);

    Sphere sph(FloatVec3(20.f, 0.f, 0.0f), 10.0f);
    Sphere sph2(FloatVec3(-10.0f, 0.f, 0.0f), 5.0f);

    buffer.DrawSpherePersp(sph, sph2, m, width, height, FloatVec3(0.0f,0.0f,80.0f), FloatVec3(0.0f,0.f,-1.f), FloatVec3(0.0f,1.f,0.0f));

    buffer.Save();

    delete m;

    return 0;
}

int meshRender() //algorytmy
{
    //texture buffer
    TgaBuffer texBuffer(512, 512);
    texBuffer.ClearColor(0xffaaaaaa);
    std::string fileName = "lightbrick.tga";
    //string fileName = "redbrick.tga";
    texBuffer.Load(fileName);

    TgaBuffer buffer(width, height);
    VertexProcessor vp;

    buffer.ClearColor(0xff777777);
    buffer.ClearDepth(-99999.0f);

    vp.SetPerspective(90.f, 1.f, 0.1f, 1000.0f);

    //FloatVec3 eye(5.0f, 5.0f, 10.0f);
    FloatVec3 eye(0.0f, 90.0f, 90.0f);
    FloatVec3 center(0.0f, 0.0f, 0.0f);
    FloatVec3 up(0.0f, 1.0f, 0.0f);

    vp.SetLookat(eye, center, up);

    //lighting setup
    bool light = true;
    bool fragmentLight = true;
    bool pointLight = false;
    bool dirLight = true;

    Mesh m;

    //Loaded mesh
    vp.obj2world.Identity();
    vp.rotation.Identity();
    //vp.multByScale(FloatVec3(0.05f, 0.05f, 0.05f));
    vp.multByTranslation(FloatVec3(0.f, 0.f, -50.0f));

    if (m.LoadObj("teapot.obj") == -1)
        return -1;

    m.Draw(true, buffer, vp, texBuffer, fragmentLight, pointLight, dirLight);

    //pyramid
    vp.obj2world.Identity();
    vp.rotation.Identity();
    vp.multByScale(FloatVec3(20.0f, 20.0f, 20.0f));
    //vp.multByRotation(-40.0f, FloatVec3(1.f, 0.f, 0.0f));
    vp.multByTranslation(FloatVec3(50.f, 0.f, 50.0f));

    m.pyramid(2.0f, 1.5f, 8);
    m.Draw(true, buffer, vp, texBuffer, fragmentLight, pointLight, dirLight);

    //cylinder
    vp.obj2world.Identity();
    vp.rotation.Identity();
    vp.multByScale(FloatVec3(10.0f, 10.0f, 10.0f));
    //vp.multByRotation(90.0f, FloatVec3(1.f, 0.f, 0.0f));
    vp.multByTranslation(FloatVec3(-50.f, 0.f, 50.0f));

    //m.cylinder(3.0f, 2.f, 8, 5);
    m.Cube(3.0f);
    m.Draw(true, buffer, vp, texBuffer, fragmentLight, pointLight, dirLight);

    buffer.Save();

    return 0;
}
