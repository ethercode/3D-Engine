#ifndef _triangle_h_
#define _triangle_h_

#include "3dtypes.h"

class CTriangle
{
public:
                CTriangle();
                ~CTriangle();
    float       ZOrder;
    void        Render();
    void        RenderWireframe();
    CPoint3D    Original[3];
    CPoint3D    Ready[3];
    CPoint2D    Rendered[3];
    TColour     Col;
    int         Alpha;
};

#endif