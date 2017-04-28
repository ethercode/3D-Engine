#ifndef _engine_h_
#define _engine_h_

#include "Triangle.h"

class CEngine
{
public:
                CEngine();
                ~CEngine();
    void        AddTriangle(CTriangle* Triangle);
    void        Render();
    void        NewScene();
    void        SetObjectRotation(float x, float y, float z);
    void        Blit();
    void        SetEyeRotation(float v, float w);
    void        ShowFPS();
    void        HideFPS();

private:
    int         fps;
    int         TotalTriangles;
    CTriangle*  Triangles[1000];
    void        SortTriangles();

    void        CalcPoint(CPoint3D* source, CPoint3D& Ready, CPoint2D& Rendered);

    float       vd; //distance of viewpoint from origin
    float       rv;   //angle of viewing distance (degrees)
    float       vh;   //height of viewpoint from origin
    float       pd;//distance of picture plane from origin
    float       rw;   //rotation of viewpoint (degrees)

    float       sx, sy, sz, cx, cy, cz; //buffered sines and cosines
    float       sw, cw, sv, cv;       //buffered sines and cosines

    float       pi;
};

#endif