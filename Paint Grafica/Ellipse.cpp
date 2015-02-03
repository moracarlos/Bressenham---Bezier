#include "Ellipse.h"
CEllipse::CEllipse()
{
    degree=0;
    mType = ELLIPSE;
    bBox=false;
    fFigure=false;
    degree=0;
	mVertices = new float*[2];
	for (int i = 0; i < 2; ++i)
		mVertices[i] = new float[2];
}

CEllipse::~CEllipse()
{
    for (int i = 0; i < 2; ++i){
		delete[] mVertices[i];
    }
}

int compareEllipse (pointPair a, pointPair b) { return (a.y>b.y); }

void CEllipse::display()
{
	glBegin(GL_POINTS);
    std::vector < pointPair > points; //Vector de pares de puntos
    pointPair point;
    glColor3f(mColor[0],mColor[1], mColor[2]);
    int x1, y1, x2, y2, x, y, d, mx, my, a, b, aa, bb;
    x1=mVertices[0][0];
    y1=mVertices[0][1];
    x2=mVertices[1][0];
    y2=mVertices[1][1];
    a = abs(x2-x1);
    b = abs(y2-y1);
    aa=a*a;
    bb=b*b;
    x=0;
    y=b;
    d=b*(4*b - 4*a*a) + a*a;
    mx= -1*x;
    my= -1*y;
    point.x = x+x1;
    point.y=y+y1;
    points.push_back(point);
    point.x = x+x1;
    point.y=my+y1;
    points.push_back(point);
    while ((bb<<1)*(x+1) < aa*((y<<1)-1)){
        if (d<0){
            d+= (bb<<2)*((x<<1)+3);
        }else{
            d+= (bb*((x<<1)+3)+aa*(-2*y+2))<<2;
            --y;
        }
        ++x;
        mx=-1*x;
        my=-1*y;
        point.x = x+x1;
        point.y=y+y1;
        points.push_back(point);
        point.x = x+x1;
        point.y=my+y1;
        points.push_back(point);
    }
    d= bb*(((x*x)<<2)+(x<<2)+1) + aa*(((y*y)<<2)-(y<<3)+4) - ((aa*bb)<<2);
    while (y>0){
        if (d<0){
            d += ((bb*((x<<1)+2)+aa*(-2*y+3))<<2);
            ++x;
        }else{
            d+= (aa<<2)*(-2*y+3);
        }
        --y;
        mx=-1*x;
        my=-1*y;
        point.x = x+x1;
        point.y=y+y1;
        points.push_back(point);
        point.x = x+x1;
        point.y=my+y1;
        points.push_back(point);
    }
    int xmin, xmax, ymin, ymax;
    xmin=fmin(mVertices[0][0]-a, mVertices[0][0]+a);
    ymin=fmin(mVertices[0][1]-b,mVertices[0][1]+b);
    xmax=fmax(mVertices[0][0]-a, mVertices[0][0]+a);
    ymax=fmax(mVertices[0][1]-b,mVertices[0][1]+b);
    setXs(xmin, xmax);
    setYs(ymin, ymax);
    if (bBox){ //Muestra el bounding box
        glColor3f(1.0f, 1.0f, 1.0f);
        int xmin, xmax, ymin, ymax;
        xmin=fmin(mVertices[0][0]-a, mVertices[0][0]+a);
        ymin=fmin(mVertices[0][1]-b,mVertices[0][1]+b);
        xmax=fmax(mVertices[0][0]-a, mVertices[0][0]+a);
        ymax=fmax(mVertices[0][1]-b,mVertices[0][1]+b);
        for (int i=xmin; i<=xmax; i++){
            glVertex2d(i, ymin);
            glVertex2d(i, ymax);
        }
        for (int i=ymin+1; i<=ymax-1; i++){
            glVertex2d(xmin, i);
            glVertex2d(xmax, i);
        }
        glColor3f(mColor[0],mColor[1], mColor[2]);
    }
    
    if (fFigure){
        //Ordenarlos
        std::sort(points.begin(), points.end(), compareEllipse);
        //Eliminar los repetidos quedandonos con el que tiene mayor coordenada x
        int coord;
        for (int i=1; i<points.size(); i++){
            coord = points[i-1].x; //Coord tiene el mayor punto en x
            while (points[i-1].y==points[i].y){
                if (points[i].x>coord)
                    coord = points[i].x;
                ++i;
            }
            hLine((x1<<1)-coord+1, points[i-1].y, coord);
        }
    }
    for (int i=0; i<points.size(); i++){
        glVertex2d(points[i].x, points[i].y);
        glVertex2d((points[i].x)*-1+(x1<<1), points[i].y);
    }
	glEnd();
    points.clear();
}

void CEllipse::hLine (int xmin, int ymin, int xmax)
{
    glColor3f(fColor[0],fColor[1], fColor[2]);
    for (int i= xmin; i<xmax; i++){
        glVertex2d(i, ymin);
    }
    glColor3f(mColor[0],mColor[1], mColor[2]);
}