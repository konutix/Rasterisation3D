#include <iostream>
#include "TgaBuffer.h"
#include "VertexProcessor.h"

using namespace std;

void test();

const int width = 512;
const int height = 512;

int realmain() //fotoreal
{
    TgaBuffer buffer(width, height);

    buffer.ClearColor(0xffaaaaaa);
    buffer.ClearDepth(99999.0f);

    //Loaded mesh
    Mesh* m = new Mesh();
    if (m->LoadObj("dodecahedron.obj") == -1)
        return -1;

    //Sphere sph(FloatVec3(0.f, 0, 30.0f), 0.3f);
    //Sphere sph2(FloatVec3(0.35f, 0, 10.0f), 0.2f);

    //buffer.DrawSphereOrtho(sph, sph2, width, height);

    Sphere sph(FloatVec3(10.f, 0.f, 0.0f), 10.0f);
    Sphere sph2(FloatVec3(-10.0f, 0.f, 0.0f), 5.0f);

    buffer.DrawSpherePersp(sph, sph2, m, width, height, FloatVec3(0.0f,0.0f,80.0f), FloatVec3(0.0f,0.f,-1.f), FloatVec3(0.0f,1.f,0.0f));

    buffer.Save();

    delete m;

    return 0;
}

int main() //algorytmy
{
    TgaBuffer texBuffer(512, 512);

    texBuffer.ClearColor(0xffaaaaaa);

    texBuffer.Load("lightbrick.tga");

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

    Mesh m;

    //Loaded mesh
    vp.obj2world.Identity();
    vp.rotation.Identity();
    //vp.multByScale(FloatVec3(0.05f, 0.05f, 0.05f));
    vp.multByTranslation(FloatVec3(0.f, 0.f, -50.0f));

    if (m.LoadObj("teapot.obj") == -1)
        return -1;

    m.Draw(true, buffer, vp, texBuffer);

    //pyramid
    vp.obj2world.Identity();
    vp.rotation.Identity();
    vp.multByScale(FloatVec3(20.0f, 20.0f, 20.0f));
    //vp.multByRotation(-40.0f, FloatVec3(1.f, 0.f, 0.0f));
    vp.multByTranslation(FloatVec3(50.f, 0.f, 50.0f));

    m.pyramid(2.0f, 1.5f, 8);
    m.Draw(false, buffer, vp, texBuffer);

    //cylinder
    vp.obj2world.Identity();
    vp.rotation.Identity();
    vp.multByScale(FloatVec3(10.0f, 10.0f, 10.0f));
    //vp.multByRotation(45.0f, FloatVec3(0.f, 0.f, 1.0f));
    vp.multByTranslation(FloatVec3(-50.f, 0.f, 50.0f));

    m.cylinder(3.0f, 2.f, 8, 5);
    m.Draw(true, buffer, vp, texBuffer);

    buffer.Save();

    return 0;
}

//matrix multip test
void test()
{
    FloatMat4 proj;
    FloatMat4 mat2;

    proj[0][0] = 34; proj[1][0] = 67; proj[2][0] = 12; proj[3][0] = 76;
    proj[0][1] = 12; proj[1][1] = 54; proj[2][1] = 3;  proj[3][1] = 32;
    proj[0][2] = 33; proj[1][2] = 45; proj[2][2] = 87; proj[3][2] = 90;
    proj[0][3] = 76; proj[1][3] = 16; proj[2][3] = 71; proj[3][3] = 66;

    mat2[0][0] = 13; mat2[1][0] = 24; mat2[2][0] = 35; mat2[3][0] = 46;
    mat2[0][1] = 57; mat2[1][1] = 86; mat2[2][1] = 75; mat2[3][1] = 64;
    mat2[0][2] = 55; mat2[1][2] = 88; mat2[2][2] = 34; mat2[3][2] = 17;
    mat2[0][3] = 82; mat2[1][3] = 88; mat2[2][3] = 11; mat2[3][3] = 6;

    FloatMat4 mat3 = proj * mat2;

    std::cout << std::endl;

    std::cout << mat3[0][0] << " " << mat3[1][0] << " " << mat3[2][0] << " " << mat3[3][0] << std::endl;
    std::cout << mat3[0][1] << " " << mat3[1][1] << " " << mat3[2][1] << " " << mat3[3][1] << std::endl;
    std::cout << mat3[0][2] << " " << mat3[1][2] << " " << mat3[2][2] << " " << mat3[3][2] << std::endl;
    std::cout << mat3[0][3] << " " << mat3[1][3] << " " << mat3[2][3] << " " << mat3[3][3] << std::endl;

    std::cout << std::endl;
}

/*

Test lines

    std::cout << std::endl;

    std::cout << vp.world2view[0][0] << " " << vp.world2view[1][0] << " " << vp.world2view[2][0] << " " << vp.world2view[3][0] << std::endl;
    std::cout << vp.world2view[0][1] << " " << vp.world2view[1][1] << " " << vp.world2view[2][1] << " " << vp.world2view[3][1] << std::endl;
    std::cout << vp.world2view[0][2] << " " << vp.world2view[1][2] << " " << vp.world2view[2][2] << " " << vp.world2view[3][2] << std::endl;
    std::cout << vp.world2view[0][3] << " " << vp.world2view[1][3] << " " << vp.world2view[2][3] << " " << vp.world2view[3][3] << std::endl;

    std::cout << std::endl;

    ta1 = ta1 * vp.world2view;
    ta2 = ta2 * vp.world2view;
    ta3 = ta3 * vp.world2view;

    cout << "x: " << ta1.x << "  y: " << ta1.y << "  z: " << ta1.z << endl;
    cout << "x: " << ta2.x << "  y: " << ta2.y << "  z: " << ta2.z << endl;
    cout << "x: " << ta3.x << "  y: " << ta3.y << "  z: " << ta3.z << endl;

    ta1 = ta1 * vp.view2proj;
    ta2 = ta2 * vp.view2proj;
    ta3 = ta3 * vp.view2proj;

    cout << "x: " << ta1.x << "  y: " << ta1.y << "  z: " << ta1.z << endl;
    cout << "x: " << ta2.x << "  y: " << ta2.y << "  z: " << ta2.z << endl;
    cout << "x: " << ta3.x << "  y: " << ta3.y << "  z: " << ta3.z << endl;




    FloatVec3 ta1(0.0f, 0.0f, -10.0f);
    FloatVec3 ta2(0.0f, 10.0f, -10.0f);
    FloatVec3 ta3(10.0f, 0.0f, -10.0f);

    Triangle tr1(ta1, ta2, ta3);

    tr1.Transform(vp.obj2world);
    tr1.Transform(vp.world2view);
    tr1.Transform(vp.view2proj);

    buffer.DrawTriangle(tr1);

    FloatVec3 tb1(0.f, 5.f, 0.0f);
    FloatVec3 tb2(2.f, 0.f, 0.0f);
    FloatVec3 tb3(-2.f, 0.f, 0.0f);

    vp.obj2world.Identity();

    vp.multByScale(FloatVec3(1.f, 1.f, 1.f));
    vp.multByRotation(45.0f, FloatVec3(0.f, 1.f, 0.f));
    vp.multByRotation(90.0f, FloatVec3(0.f, 0.f, 1.f));
    vp.multByTranslation(FloatVec3(2.f, -3.f, 5.0f));

    Triangle tr2(tb1, tb2, tb3);

    tr2.Transform(vp.obj2world);
    tr2.Transform(vp.world2view);
    tr2.Transform(vp.view2proj);

    buffer.DrawTriangle(tr2);


        //cube
    vp.multByScale(FloatVec3(1.2f, 1.2f, 1.2f));
    vp.multByRotation(60.0f, FloatVec3(0.f, 0.f, 1.0f));
    vp.multByRotation(20.0f, FloatVec3(1.f, 0.f, 0.0f));
    vp.multByTranslation(FloatVec3(-2.f, 0.f, 5.0f));

    m.Cube(0.5f);
    m.Draw(buffer, vp);
*/