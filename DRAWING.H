class   CTraingle;

#include "3dtypes.h"

void    Line(CPoint2D* points, TColour col);
void    HollowTriangle(CPoint2D* points, TColour col);
void    FillTriangle(class CTriangle* Triangle);
void    Clear();
void    PutPixel(int x, int y, TColour col);
void    DrawBackground();
void    VLine(int x, int y, int l, TColour c);
void    HLine(int x, int y, int l, TColour c);
void    Mode(int);
void    BlitSurface();
void    WaitRetrace();