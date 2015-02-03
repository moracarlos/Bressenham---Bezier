#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

enum FigureType
{
    NONE,
	LINE,
	QUAD,
	CIRCLE,
	TRIANGLE,
	ELLIPSE,
    CURVE
};

enum Actions
{
    ADD,
    MOVE,
    SHOWBBOX,
    FILL,
    CHANGEFILL,
    CHANGEBORDER
};

struct pointPair{
    float x,y;
};

class CFigure
{
	protected:
		float **mVertices, mColor[3], fColor[3];
        bool bBox;
        bool fFigure;
        int mType;
        float xs[2];
        float ys[2];
        int nVertices;
        float degree;

	public:
		CFigure();
		virtual ~CFigure();
		void setVertex(int id, float x, float y);
		void setColor(float r, float g, float b);
        float* getVertex(int id);
        float** getmVertices();
		virtual void display() = 0;
        void viewBox(bool option);
        bool viewBox();
        void setFillColor(float r, float g, float b);
        void fillFigure(bool option);
        float* getFillColor();
        float* getBorderColor();
        int getType();
        float* getXs();
        float* getYs();
        void setXs(float x0, float x1);
        void setYs(float y0, float y1);
        void setBoolFill(bool option);
        bool fillFigure();
        void addVertex(float x, float y);
        int getnVertices();
        float getDegree();
        void setDegree(float degree);
        void elevateDegree();
        void deletePoint(int p);

};