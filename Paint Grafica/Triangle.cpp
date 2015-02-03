#include "Triangle.h"

CTriangle::CTriangle()
{
    mType = TRIANGLE;
    bBox=false;
    fFigure= false;
    degree=0;
    mVertices = new float*[3];
    for (int i = 0; i < 3; ++i)
        mVertices[i] = new float[2];
}

CTriangle::~CTriangle()
{
    for (int i = 0; i < 3; ++i)
        delete[] mVertices[i];
}

int compareTriangle (pointPair a, pointPair b) { return (a.y>b.y); }

void CTriangle::display()
{
    std::vector <pointPair> points;
    line(mVertices[0][0], mVertices[1][0], mVertices[0][1], mVertices[1][1], points);
    line(mVertices[0][0], mVertices[1][0], mVertices[0][1], mVertices[1][1], points);
    line(mVertices[1][0], mVertices[2][0], mVertices[1][1], mVertices[2][1], points);
    line(mVertices[2][0], mVertices[0][0], mVertices[2][1], mVertices[0][1], points);
    glBegin(GL_POINTS);
    if (fFigure){
        //Ordenarlos
        std::sort(points.begin(), points.end(), compareTriangle);
        //Eliminar los repetidos quedandonos con el que tiene mayor coordenada x
        int maximo, minimo;
        for (int i=1; i<points.size(); i++){
            maximo = points[i-1].x; //maximo tiene el mayor punto en x
            minimo = points[i-1].x;
            while (points[i-1].y==points[i].y){
                if (points[i].x>maximo)
                    maximo = points[i].x;
                if (points[i].x<minimo)
                    minimo = points[i].x;
                ++i;
            }
            hLine(minimo, points[i-1].y, maximo);
        }
    }
    glColor3f(mColor[0],mColor[1], mColor[2]);
    for (int i=0; i<points.size(); i++)
        glVertex2d(points[i].x, points[i].y);
    glEnd();
    points.clear();

}

void CTriangle::line(int x1, int x2, int y1, int y2, std::vector<pointPair>& points)
{
    pointPair point;
    glBegin(GL_POINTS);
    glColor3f(mColor[0],mColor[1], mColor[2]);
    int dx, dy, d, incE, incNE, incY, inv;
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    int a;
    inv=0;
    if (dy>dx)
    {
        inv=1;
        a=x1; x1=y1; y1=a;
        a=x2; x2=y2; y2=a;
        a=dx; dx=dy; dy=a;
        inv=1;
    }
    if (x1>x2)
    {
        a=x1; x1=x2; x2=a;
        a=y1; y1=y2; y2=a;
    }
    if (y1>y2)
        incY=-1;
    else
        incY=1;
    d =2*dy-dx;
    incE = 2*dy;
    incNE = 2* (dy-dx);
    if (!inv){
        //glVertex2d(x1, y1);
        point.x=x1;
        point.y=y1;
        points.push_back(point);
    }else{
        //glVertex2d(y1, x1);
        point.x=y1;
        point.y=x1;
        points.push_back(point);
    }
    while (x1<x2){
        if (d<=0){
            d += incE;
        }else{
            d += incNE;
            y1+=incY;
        }
        ++x1;
        if (inv){
            //glVertex2f(y1, x1);
            point.x=y1;
            point.y=x1;
            points.push_back(point);
        }else{
            //glVertex2f(x1, y1);
            point.x=x1;
            point.y=y1;
            points.push_back(point);
        }
    }
    
    //Cslcula el bounding box
    int xmin, xmax, ymin, ymax;
    x1=mVertices[0][0];
    y1=mVertices[0][1];
    xmin=fmin(mVertices[0][0], fmin(mVertices[1][0], mVertices[2][0]));
    ymin=fmin(mVertices[0][1], fmin(mVertices[1][1], mVertices[2][1]));
    xmax=fmax(mVertices[0][0], fmax(mVertices[1][0], mVertices[2][0]));
    ymax=fmax(mVertices[0][1], fmax(mVertices[1][1], mVertices[2][1]));
    setXs(xmin, xmax);
    setYs(ymin, ymax);
    if (bBox){ //Muestra el bounding box
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int i=xmin; i<=xmax; i++){
            glVertex2d(i, ymin);
            glVertex2d(i, ymax);
        }
        for (int i=ymin+1; i<=ymax-1; i++){
            glVertex2d(xmin, i);
            glVertex2d(xmax, i);
        }
    }
    glEnd();
}

void CTriangle::hLine (int xmin, int ymin, int xmax)
{
    glColor3f(fColor[0],fColor[1], fColor[2]);
    for (int i= xmin; i<xmax; i++){
        glVertex2d(i, ymin);
    }
    glColor3f(mColor[0],mColor[1], mColor[2]);
}