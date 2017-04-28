#include "3dTypes.h"
#include "dos.h"
#include "mem.h"
#include "drawing.h"
#include "triangle.h"

#include <math.h>
#include <stdlib.h>

unsigned char   Surface[200][320];

unsigned int    Tri1[320];
unsigned int    Tri2[320];

char            curBank = 0;
unsigned long   s1;
char            bank;
int             x, y;

TColour         c;

void swap(int* a, int* b)
{
    int v   = *a;
    *a = *b;
    *b = v;
}

void Clear()
{
    /*  void *m=((void*)(0xA0000000L));
        for (curBank=0;curBank<48;curBank++)
        {
            asm {
                mov ax,0x4F05
                mov bx,0
                mov dl,curBank
                int 0x10
            }
            memset(m,0,65534U);
        }
        curBank=0;
        asm {
            mov ax,0x4F05
            mov bx,0
            mov dl,curBank
            int 0x10
        }
        */
    memset(Surface, 0, 64000U);
}

void Line(CPoint2D* points, TColour col)
{
    int x1, y1, x2, y2;
    if (points[0].x <= points[1].x)
    {
        x1 = points[0].x;
        y1 = points[0].y;
        x2 = points[1].x;
        y2 = points[1].y;
    }
    else
    {
        x2 = points[0].x;
        y2 = points[0].y;
        x1 = points[1].x;
        y1 = points[1].y;
    }
    if ((x1 == x2) && (y1 == y2))
    {
        return;
    }
    float   dx  = x2 - x1;
    float   dy  = y2 - y1;
    if (fabs(dx) > fabs(dy))
    {
        float   iy  = dy / dx;
        float   cy  = y1;
        if (x1 > x2)
        {
            swap(&x1, &x2);
        }
        for (int x = x1; x <= x2; x++)
        {
            PutPixel(x, int(cy + 0.5), col);
            cy += iy;
        }
    }
    else
    {
        float   ix  = fabs(dx / dy);
        float   cx  = x1;
        if (y1 > y2)
        {
            swap(&y1, &y2);
            ix = -ix;
            cx = x2;
        }
        for (int y = y1; y <= y2; y++)
        {
            PutPixel(int(cx + 0.5), y, col);
            cx += ix;
        }
    }
}

void HollowTriangle(CPoint2D* points, TColour col)
{
    Line(points, col);
    Line(points + 1, col);
    CPoint2D    z[2];
    z[0] = points[0];
    z[1] = points[2];
    Line(z, col);
}

void FillTriangle(CTriangle* Triangle)
{
    int         x1, y1, x2, y2, x3, y3;
    int         SortDone    = 0;

    CPoint2D    points[3];
    points[0] = Triangle->Rendered[0];
    points[1] = Triangle->Rendered[1];
    points[2] = Triangle->Rendered[2];
    TColour col     = Triangle->Col;
    int     alpha   = Triangle->Alpha;
    x1 = points[0].x;
    x2 = points[1].x;
    x3 = points[2].x;
    y1 = points[0].y;
    y2 = points[1].y;
    y3 = points[2].y;

    while (!SortDone)
    {
        SortDone = 1;
        if (x1 > x2)
        {
            swap(&x1, &x2);
            swap(&y1, &y2);
            SortDone = 0;
        }
        if (x2 > x3)
        {
            swap(&x2, &x3);
            swap(&y2, &y3);
            SortDone = 0;
        }
        if (x1 > x3)
        {
            swap(&x1, &x3);
            swap(&y1, &y3);
            SortDone = 0;
        }
    }

    float   dx1 = x2 - x1;
    float   dy1 = y2 - y1;
    float   dx2 = x3 - x2;
    float   dy2 = y3 - y2;
    float   dx3 = x3 - x1;
    float   dy3 = y3 - y1;
    if (abs(dx1) < 0.000001)
    {
        dx1 = 0.00001;
    }
    if (abs(dx2) < 0.000001)
    {
        dx2 = 0.00001;
    }
    if (abs(dx3) < 0.000001)
    {
        dx3 = 0.00001;
    }
    if (abs(dy1) < 0.000001)
    {
        dy1 = 0.00001;
    }
    if (abs(dy2) < 0.000001)
    {
        dy2 = 0.00001;
    }
    if (abs(dy3) < 0.000001)
    {
        dy3 = 0.00001;
    }

    int restorex    = 0;
    int restorey    = 0;
    if (fabs(dx1) > fabs(dy1))
    {
        float   iy  = dy1 / dx1;
        float   cy  = y1;
        if (x1 > x2)
        {
            swap(&x1, &x2);
            restorex = 1;
        }
        for (int x = x1; x <= x2; x++)
        {
            Tri1[x] = int(cy + 0.5);
            cy += iy;
        }
    }
    else
    {
        float   ix  = fabs(dx1 / dy1);
        float   cx  = x1;
        if (y1 > y2)
        {
            swap(&y1, &y2);
            ix = -ix;
            cx = x2;
            restorey = 1;
        }
        for (int y = y1; y <= y2; y++)
        {
            Tri1[int(cx + 0.5)] = y;
            cx += ix;
        }
    }
    if (restorex)
    {
        swap(&x1, &x2);
    }
    if (restorey)
    {
        swap(&y1, &y2);
    }

    restorex = 0;
    restorey = 0;
    if (fabs(dx2) > fabs(dy2))
    {
        float   iy  = dy2 / dx2;
        float   cy  = y2;
        if (x2 > x3)
        {
            swap(&x2, &x3);
            restorex = 1;
        }
        for (int x = x2; x <= x3; x++)
        {
            Tri1[x] = int(cy + 0.5);
            cy += iy;
        }
    }
    else
    {
        float   ix  = fabs(dx2 / dy2);
        float   cx  = x2;
        if (y2 > y3)
        {
            swap(&y2, &y3);
            ix = -ix;
            cx = x3;
            restorey = 1;
        }
        for (int y = y2; y <= y3; y++)
        {
            Tri1[int(cx + 0.5)] = y;
            cx += ix;
        }
    }
    if (restorex)
    {
        swap(&x2, &x3);
    }
    if (restorey)
    {
        swap(&y2, &y3);
    }

    restorex = 0;
    restorey = 0;
    if (fabs(dx3) > fabs(dy3))
    {
        float   iy  = dy3 / dx3;
        float   cy  = y1;
        if (x1 > x3)
        {
            swap(&x1, &x3);
            restorex = 1;
        }
        for (int x = x1; x <= x3; x++)
        {
            Tri2[x] = int(cy + 0.5);
            cy += iy;
        }
    }
    else
    {
        float   ix  = fabs(dx3 / dy3);
        float   cx  = x1;
        if (y1 > y3)
        {
            swap(&y1, &y3);
            ix = -ix;
            cx = x3;
            restorey = 1;
        }
        for (int y = y1; y <= y3; y++)
        {
            Tri2[int(cx + 0.5)] = y;
            cx += ix;
        }
    }
    if (restorex)
    {
        swap(&x1, &x3);
    }
    if (restorey)
    {
        swap(&y1, &y3);
    }

    int a, b;
    for (int x = x1; x <= x3; x++)
    {
        a = Tri1[x];
        b = Tri2[x];
        if (a > b)
        {
            swap(&a, &b);
        }
        for (int y = a; y <= b; y++)
        {
            if (alpha == 100)
            {
                PutPixel(x, y, col);
            }
            else if (alpha == 66)
            {
                if ((((x + x1) % 2) == 0) && (((y + y1) % 2) == 0))
                {
                    PutPixel(x, y, col);
                }
            }
            else if (alpha == 33)
            {
                if ((((x + x1) % 3) == 0) && (((y + y1) % 3) == 0))
                {
                    PutPixel(x, y, col);
                }
            }
        }
    }
}

void PutPixel(int x, int y, TColour col)
{
    if ((x >= 0) && (y >= 0) && (x < 320) && (y < 200))
            /*  {
                    s1=y;
                    s1=((s1<<10)+x)<<2;
                    bank=0;
                    while (s1>=65536UL)
                    {
                        bank++;
                        s1-=65536UL;
                    }
                    if (bank!=curBank)
                    {
                        asm {
                            mov ax,0x4F05
                            mov bx,0
                            mov dl,bank
                            int 0x10
                        }
                        curBank=bank;
                    }
                    pokeb(0xA000,(unsigned int)s1,col.b);
                    pokeb(0xA000,(unsigned int)s1+1,col.g);
                    pokeb(0xA000,(unsigned int)s1+2,col.r);
                }
                */
    {
        Surface[y][x] = col.r;
    }
}

void VLine(int x, int y, int l, TColour c)
{
    for (int a = y; a <= (y + l); a++)
    {
        PutPixel(x, a, c);
    }
}

void HLine(int x, int y, int l, TColour c)
{
    for (int a = x; a <= (x + l); a++)
    {
        PutPixel(a, y, c);
    }
}

void Mode(int m)
{
    if (m == 1)
    {
        asm {
            mov ax,0x13
            int 0x10
        }
    }
    else if (m == 0)
    {
        asm {
            mov ax,0x3
            int 0x10
        }
    }
}

void BlitSurface(void)
{
    memcpy(((void *) (0xA0000000L)), Surface, 64000U);
}

void WaitRetrace(void)
{
    asm {
	mov dx,0x3DA
    }
    l1:
    asm {
        in al,dx
        and al,0x08
        jnz l1
    }
    l2:
    asm {
        in al,dx
        and al,0x08
        jz  l2
    }
}