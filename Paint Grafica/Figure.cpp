#include "Figure.h"

CFigure::CFigure()
{
	mColor[0] = 1.0f;
	mColor[1] = 1.0f;
	mColor[2] = 1.0f;
    fColor[0] = 1.0f;
    fColor[1] = 0.0f;
    fColor[2] = 0.0f;
}

CFigure::~CFigure()
{
	delete[] mVertices;
}

void CFigure::setVertex(int id, float x, float y)
{
	mVertices[id][0] = x; 
	mVertices[id][1] = y;
}

void CFigure::setColor(float r, float g, float b)
{
	mColor[0] = r;
	mColor[1] = g;
	mColor[2] = b;
}

void CFigure::viewBox(bool option)
{
        bBox=option;
}

bool CFigure::viewBox(){
    return bBox;
}


void CFigure::setFillColor(float r, float g, float b)
{
    fColor[0] = r;
    fColor[1] = g;
    fColor[2] = b;
}

void CFigure::fillFigure(bool option)
{
    fFigure=option;
}

bool CFigure::fillFigure(){
    return fFigure;
}

float* CFigure::getVertex(int id)
{
    return mVertices[id];
}

float* CFigure::getFillColor()
{
    return fColor;
}

int CFigure::getType()
{
    return mType;
}

float* CFigure::getBorderColor()
{
    return mColor;
}

float* CFigure::getXs()
{
    return xs;
}

float* CFigure::getYs()
{
    return ys;
}

void CFigure::setXs(float x0, float x1){
    xs[0]=x0;
    xs[1]=x1;
}

void CFigure::setYs(float y0, float y1){
    ys[0]=y0;
    ys[1]=y1;
}

void CFigure::setBoolFill(bool option){
    fFigure = option;
}

void CFigure::addVertex(float x, float y)
{
    nVertices++;
    mVertices = (float**) realloc (mVertices, sizeof(float*)*nVertices);
    mVertices[nVertices-1] = new float[2];
    mVertices[nVertices-1][0]=x;
    mVertices[nVertices-1][1]=y;
}

int CFigure::getnVertices(){
    return nVertices;
}

float CFigure::getDegree(){
    return degree;
}

void CFigure::setDegree(float d){
    if (mType==CURVE)
        degree=d;
}

void CFigure::elevateDegree()
{
    int i;
    double antx, anty, x,y;
    antx=mVertices[0][0];
    anty=mVertices[0][1];
    for (i=1; i<nVertices; i++)
    {
        x = (float(i)/float(nVertices))*antx + mVertices[i][0]*(1-(float(i)/float(nVertices)));
        y = (float(i)/float(nVertices))*anty + mVertices[i][1]*(1-(float(i)/float(nVertices)));
        antx=mVertices[i][0];
        anty=mVertices[i][1];
        setVertex(i, x, y);
    }
    addVertex(antx, anty);
}

void CFigure::deletePoint (int p)
{
    float ** newV;
    int i;
    newV = (float**) malloc (sizeof(float*)*nVertices-1);
    for (i=0; i<p; i++){
        newV[i]=(float*) malloc (sizeof(float)*2);
        memcpy(newV[i], mVertices[i], sizeof(float)*2);
    }
    i++;
    for (int j=i; j<nVertices; j++)
    {
        newV[j-1] = (float*) malloc (sizeof(float)*2);
        memcpy(newV[j-1], mVertices[j], sizeof(float)*2);
    }
    nVertices--;
    mVertices=newV;
}

float** CFigure::getmVertices()
{
    return mVertices;
}

