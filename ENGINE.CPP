#include "engine.h"
#include "3dtypes.h"
#include "drawing.h"
#include "time.h"

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

void CEngine::CalcPoint(CPoint3D* Source, CPoint3D& Ready, CPoint2D& Rendered)
{
    float       xc, xd, xe;
    float       yc, yd, ye;
    float       zc, zd, ze;

    CPoint3D    p   = *Source;

    xc = p.x * cz - p.y * sz;
    yc = p.x * sz + p.y * cz;
    zc = p.z;

    xd = xc * cy - zc * sy;
    yd = yc;
    zd = xc * sy + zc * cy;

    xe = xd;
    ye = yd * cx - zd * sx;
    ze = yd * sx + zd * cx;

    Ready.x = xe;
    Ready.y = ye;
    Ready.z = ze;

    p.x = (vd - pd) * xe / (vd + ye) + 160;
    p.y = (200 - ((pd + ye) * (vh - ze) / (vd + ye) + ze)) - 100;

    Rendered.x = p.x;
    Rendered.y = p.y;
}

void CEngine::SortTriangles()
{
    if (TotalTriangles < 2)
    {
        return;
    }
    int flag    = 1;
    while (flag)
    {
        flag = 0;
        for (int a = 0; a < TotalTriangles - 1; a++)
        {
            if (Triangles[a]->ZOrder < Triangles[a + 1]->ZOrder)
            {
                CTriangle*  z   = Triangles[a];
                Triangles[a] = Triangles[a + 1];
                Triangles[a + 1] = z;
                flag = 1;
            }
        }
    }
}

void CEngine::SetObjectRotation(float x, float y, float z)
{
    cx = cos(pi / 180.0 * x);
    cy = cos(pi / 180.0 * y);
    cz = cos(pi / 180.0 * z);
    sx = sin(pi / 180.0 * x);
    sy = sin(pi / 180.0 * y);
    sz = sin(pi / 180.0 * z);
}

void CEngine::SetEyeRotation(float v, float w)
{
    sv = sin(pi / 180.0 * v);
    sw = sin(pi / 180.0 * w);
    cv = cos(pi / 180.0 * v);
    sw = sin(pi / 180.0 * w);
}

void CEngine::CEngine()
{
    randomize();
    Mode(1);
    vd = 1200;
    rv = 0;
    vh = 0;
    pd = 600;
    rw = 0;
    pi = 3.141592654;
    TotalTriangles = 0;
    SetObjectRotation(0, 0, 0);
    SetEyeRotation(0, 0);
    for (int a = 0; a < 1000; a++)
    {
        Triangles[a] = 0;
    }
    fps = 0;
}

CEngine::~CEngine()
{
    Mode(0);
    for (int a = 0; a < 1000; a++)
    {
        if (Triangles[a] != 0)
        {
            delete Triangles[a];
            Triangles[a] = 0;
        }
    }
}

void CEngine::AddTriangle(CTriangle* Triangle)
{
    Triangles[TotalTriangles] = new CTriangle;
    *Triangles[TotalTriangles] = *Triangle;
    TotalTriangles++;
}

void CEngine::Render()
{
    Clear();
    SortTriangles();
    for (int a = 0; a < TotalTriangles; a++)
    {
        CalcPoint(&Triangles[a]->Original[0], Triangles[a]->Ready[0], Triangles[a]->Rendered[0]);
        CalcPoint(&Triangles[a]->Original[1], Triangles[a]->Ready[1], Triangles[a]->Rendered[1]);
        CalcPoint(&Triangles[a]->Original[2], Triangles[a]->Ready[2], Triangles[a]->Rendered[2]);
        Triangles[a]->ZOrder = Triangles[a]->Ready[0].y;
        Triangles[a]->ZOrder += Triangles[a]->Ready[1].y;
        Triangles[a]->ZOrder += Triangles[a]->Ready[2].y;
    }
    for (a = 0; a < TotalTriangles; a++)
    {
        FillTriangle(Triangles[a]);
    }
}

void CEngine::Blit()
{
    BlitSurface();
    if (fps)
    {
        static int  frames;
        static int  fpsVal  = -1;
        frames++;
        static clock_t  start;
        static clock_t  end;
        static int      init    = 1;
        if (init)
        {
            init = 0;
            frames = 0;
            start = clock();
        }
        end = clock();
        if (((end - start) / CLK_TCK) >= 1)
        {
            start = clock();
            fpsVal = frames;
            frames = 0;
        }
        gotoxy(1, 1);
        if (fpsVal != -1)
        {
            printf("%d FPS. %d triangles in scene.", fpsVal, TotalTriangles);
        }
        else
        {
            printf("Calculating FPS", fpsVal);
        }
    }
}

void CEngine::ShowFPS()
{
    fps = 1;
}

void CEngine::HideFPS()
{
    fps = 0;
}

