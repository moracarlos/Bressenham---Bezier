#include "Curve.h"
#include <cmath>

CCurve::CCurve()
{
    degree=0;
    mType = CURVE;
    bBox=false;
    fFigure= false;
    nVertices=0;
    mVertices = (float**) malloc (sizeof(float*)*0);
}

CCurve::~CCurve()
{
    for (int i = 0; i < nVertices; ++i)
        delete[] mVertices[i];
}

int compareCurve (float a[2], float b[2]) { return (a[0]>b[0]); }

void CCurve::display()
{
    float** cPoints= new float*[nVertices];
    pointPair nuevo;
    std::vector <pointPair> points;
    int i,j,act;
    float a,b;
    float xmin, xmax, ymin, ymax;
    xmin = mVertices[0][0];
    xmax = mVertices[0][0];
    ymin = mVertices[0][1];
    ymax = mVertices[0][1];
    act=nVertices;
    float t;
    
    memcpy(cPoints, mVertices, sizeof(float*)*nVertices);
    for (i=0; i<nVertices; i++){
        cPoints[i] = (float*) malloc (sizeof(float)*2);
        memcpy(cPoints[i], mVertices[i], sizeof(float)*2);
    }
    if (nVertices!=1){
        for (t=0; t<=1; t+=0.001){
            for (i=0; i<nVertices; i++){
                for (j=0; j<act-i-1;j++){
                    a=(1-t)*cPoints[j][0]+t*cPoints[j+1][0];
                    b=(1-t)*cPoints[j][1]+t*cPoints[j+1][1];
                    cPoints[j][0]=a;
                    cPoints[j][1]=b;
                    nuevo.x=a;
                    nuevo.y=b;
                }
            }
            points.push_back(nuevo);
            if (nuevo.x<xmin)
                xmin = nuevo.x;
            if (nuevo.x>xmax)
                xmax = nuevo.x;
            if (nuevo.y<ymin)
                ymin = nuevo.y;
            if (nuevo.y>ymax)
                ymax = nuevo.y;
        }
        for (i=0; i<points.size()-1; i++){
            line(points[i].x, points[i+1].x, points[i].y, points[i+1].y, points);
        }
        //Este dibuja la envolvente convexa
        glBegin(GL_LINES);
        glColor3f(0, 0, 1);
        for (i=0; i<nVertices-1; i++){
            glVertex3f(mVertices[i][0], mVertices[i][1], 0);
            glVertex3f(mVertices[i+1][0], mVertices[i+1][1], 0);
        }
        glEnd();
    }else{
        glBegin(GL_POINTS);
        glVertex2d(mVertices[0][0], mVertices[0][1]);
        glColor3f(mColor[0],mColor[1], mColor[2]);
        glEnd();
        if (mVertices[0][0]<xmin)
            xmin = mVertices[0][0];
        if (mVertices[0][0]>xmax)
            xmax = mVertices[0][0];
        if (mVertices[0][1]<ymin)
            ymin = mVertices[0][1];
        if (mVertices[0][1]>ymax)
            ymax = mVertices[0][1];
    }
    setXs(xmin, xmax);
    setYs(ymin, ymax);
    for (i=0; i<nVertices; i++){
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2d(mVertices[i][0], mVertices[i][1]);
        glPointSize(1.0);
        glEnd();
        glPointSize(1);

    }
    
    if (bBox){ //Muestra el bounding box
        glBegin(GL_POINTS);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int i=xmin; i<=xmax; i++){
            glVertex2d(i, ymin);
            glVertex2d(i, ymax);
        }
        for (int i=ymin+1; i<=ymax-1; i++){
            glVertex2d(xmin, i);
            glVertex2d(xmax, i);
        }
        glEnd();
    }
}

void CCurve::line(int x1, int x2, int y1, int y2, std::vector<pointPair> &points)
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
        glVertex2d(x1, y1);
        point.x=x1;
        point.y=y1;
    }else{
        glVertex2d(y1, x1);
        point.x=y1;
        point.y=x1;
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
            glVertex2f(y1, x1);
            point.x=y1;
            point.y=x1;
        }else{
            glVertex2f(x1, y1);
            point.x=x1;
            point.y=y1;
        }
    }
    glEnd();
}

void CCurve::hLine (int xmin, int ymin, int xmax)
{
    glColor3f(fColor[0],fColor[1], fColor[2]);
    for (int i= xmin; i<xmax; i++){
        glVertex2d(i, ymin);
    }
    glColor3f(mColor[0],mColor[1], mColor[2]);
}

