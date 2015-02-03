#include "Quad.h"

CQuad::CQuad()
{
    degree=0;
    mType = QUAD;
    bBox=false;
    fFigure=false;
    degree=0;
	mVertices = new float*[2];
	for (int i = 0; i < 2; ++i)
		mVertices[i] = new float[2];
}

CQuad::~CQuad()
{
	for (int i = 0; i < 2; ++i)
    {
		delete[] mVertices[i];
    }
}

void CQuad::display()
{
	glBegin(GL_POINTS);
    if (!bBox)
        glColor3f(mColor[0],mColor[1], mColor[2]);
    else
        glColor3f(1.0f,1.0f,1.0f);
 
    int x1,x2,y1,y2,xmin,xmax,ymin,ymax;
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
    
    if (fFigure){
        hLine(xmin, ymin, xmax);
        hLine(xmin, ymax, xmax);
        for (int i=ymin+1; i<=ymax-1; i++){
            glVertex2d(xmin, i);
            hLine(xmin+1, i, xmax);
            glVertex2d(xmax, i);
        }
    }
    
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

void CQuad::hLine (int xmin, int ymin, int xmax)
{
    glColor3f(fColor[0],fColor[1], fColor[2]);
    for (int i= xmin; i<xmax; i++){
        glVertex2d(i, ymin);
    }
    if (!bBox)
        glColor3f(mColor[0],mColor[1], mColor[2]);
    else
        glColor3f(1.0f,1.0f,1.0f);
}