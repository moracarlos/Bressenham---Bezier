#include "Line.h"
#include <iostream>

CLine::CLine()
{
    degree=0;
    mType = LINE;
    bBox=false;
	mVertices = new float*[2];
    degree=0;
	for (int i = 0; i < 2; ++i)
		mVertices[i] = new float[2];
}

CLine::~CLine()
{
    for (int i = 0; i < 2; ++i){
		delete[] mVertices[i];
    }
}

void CLine::display()
{
    glBegin(GL_POINTS);
        glColor3f(mColor[0],mColor[1], mColor[2]);
        int dx, dy, d, incE, incNE, x1, y1,x2,y2, incY, inv;
        x1=mVertices[0][0];
        y1=mVertices[0][1];
		x2=mVertices[1][0];
		y2=mVertices[1][1];
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
        if (!inv)
            glVertex2d(x1, y1);
        else
            glVertex2d(y1, x1);
		while (x1<x2){
			if (d<=0){
				d += incE;
			}else{
				d += incNE;
				y1+=incY;
			}
			++x1;
			if (inv)
				glVertex2f(y1, x1);
			else
				glVertex2f(x1, y1);
			}
    int xmin, xmax, ymin, ymax;
    x1=mVertices[0][0];
    y1=mVertices[0][1];
    x2=mVertices[1][0];
    y2=mVertices[1][1];
    xmin=fmin(x1, x2);
    ymin=fmin(y1,y2);
    xmax=fmax(x1, x2);
    ymax=fmax(y1, y2);
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