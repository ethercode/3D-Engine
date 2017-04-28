#include <stdlib.h>
#include "engine.h"
#include "drawing.h"
#include "triangle.h"
#include <math.h>
#include "mouse.h"

#include <conio.h>

void main(void)
{
    CEngine*Engine  = new CEngine;

    TColour redCol, blackCol;

    redCol.r = 255;
    redCol.g = 0;
    redCol.b = 0;

    blackCol.r = 0;
    blackCol.g = 0;
    blackCol.b = 0;
    TMouse      m;
    CTriangle   Tri;

    int         Sides   = 40;
    float       Side    = 1;
    float       Inc     = 360 / Sides;
    float       scale;

    for (int ab = 0; ab < 360; ab += Inc)
    {
        float   a   = ab*(3.141592654 / 180.0);
        float   b   = (ab + Inc) * (3.141592654 / 180.0);
        Tri.Original[0].y = 0;
        Tri.Original[1].x = 0;
        Tri.Original[1].y = 0;
        Tri.Original[1].z = 0;
        Tri.Original[2].y = 0;
        Tri.Col.r = random(256);
        Tri.Col.g = random(256);
        Tri.Col.b = random(256);
        Tri.Alpha = 100;
        Side++;
        scale = 1;
        for (int e = 150; e >= -150; e -= 30)
        {
            Tri.Original[2].x = (140 * cos(b) - 140 * sin(b)) * scale;
            Tri.Original[0].x = (140 * cos(a) - 140 * sin(a)) * scale;
            Tri.Original[0].z = (140 * sin(a) + 140 * cos(a)) * scale;
            Tri.Original[2].z = (140 * sin(b) + 140 * cos(b)) * scale;
            Tri.Original[0].y = e;
            Tri.Original[1].y = e;
            Tri.Original[2].y = e;
            scale *= 0.9;

            Engine->AddTriangle(&Tri);
        }
    }

    float   rx = 90, ry = 0, rz = 0;
    int     exit    = 0;

    Engine->ShowFPS();

    while (!exit)
    {
        if (kbhit())
        {
            char    t   = getch();
            if (t == ' ')
            {
                exit = 1;
            }
            if (t == 'f')
            {
                static int  fps = 1;
                if (fps == 0)
                {
                    Engine->ShowFPS();
                    fps = 1;
                }
                else
                {
                    Engine->HideFPS();
                    fps = 0;
                }
            }
        }
        m.update();
        rz = m.x / 640.0 * 180.0;
        rx = m.y / 200.0 * 180.0;
        if (m.leftDown)
        {
            ry++;
        }
        if (m.rightDown)
        {
            ry--;
        }
        Engine->SetObjectRotation(rx, ry, rz);
        Engine->Render();
        int ox  = m.x / 2;
        int oy  = m.y;
        PutPixel(ox - 2, oy, redCol);
        PutPixel(ox - 1, oy, redCol);
        PutPixel(ox, oy - 2, redCol);
        PutPixel(ox, oy - 1, redCol);
        PutPixel(ox, oy + 1, redCol);
        PutPixel(ox, oy + 2, redCol);
        PutPixel(ox, oy, blackCol);
        PutPixel(ox + 1, oy, redCol);
        PutPixel(ox + 2, oy, redCol);
        Engine->Blit();
    }

    delete Engine;
}


