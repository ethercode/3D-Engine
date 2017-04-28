#include <time.h>
#include "globals.h"
#include "funcs.h"
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

void BlitTest(void)
{
    clock_t start, end;
    start = clock();

    Mode(1);
    Surface[0][0] = 1;

    int         exit    = 0;
    long int    blits   = 0;

    int         secs    = 5;

    while (!exit)
    {
        end = clock();
        Blit();
        blits++;
        if (((end - start) / CLK_TCK) >= secs)
        {
            exit = 1;
        }
    }

    float   bps = blits / (float) secs;

    Mode(0);

    printf("Blits per second: %f", bps);
    getch();
}

void TriangleTest()
{
    clock_t start, end;
    start = clock();

    Mode(1);

    int     exit    = 0;
    int     tris    = 0;

    int     secs    = 5;

    Point2D MyPoints[3];

    while (!exit)
    {
        end = clock();

        MyPoints[0].x = 0;
        MyPoints[0].y = 100;
        MyPoints[1].x = 200;
        MyPoints[1].y = 100;
        MyPoints[2].x = 100;
        MyPoints[2].y = 10;
        FillTriangle(MyPoints, 1);

        tris++;

        if (((end - start) / CLK_TCK) >= secs)
        {
            exit = 1;
        }
    }

    float   tps = tris / (float) secs;

    Mode(0);

    printf("Triangles per second: %f", tps);
    getch();
}
