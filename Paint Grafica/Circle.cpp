#include "Circle.h"
CCircle::CCircle()
{
    degree=0;
    mType = CIRCLE;
    fFigure=false;
    bBox=false;
    degree=0;
	mVertices = new float*[2];
	for (int i = 0; i < 2; ++i)
		mVertices[i] = new float[2];
}

CCircle::~CCircle()
{
    for (int i = 0; i < 2; ++i){
		delete[] mVertices[i];
    }
}

int compareCircle (pointPair a, pointPair b) { return (a.y>b.y); }


void CCircle::display()
{
	glBegin(GL_POINTS);
        glColor3f(mColor[0],mColor[1], mColor[2]);
        int x1, y1, x2, y2, r, x, y, d, mx, my;
        std::vector < pointPair > points; //Vector de pares de puntos
        pointPair point;
        x1=mVertices[0][0];
        y1=mVertices[0][1];
		x2=mVertices[1][0];
		y2=mVertices[1][1];
		r = sqrt(pow((x2-x1),2) + pow((y2-y1), 2));
		x=0;
		y=r;
		d=1-r;
		mx= -1*x;
		my= -1*y;
    //points tiene los puntos a ordenar para evitar que se dibujen varias veces la misma linea
        point.x = x+x1;
        point.y=y+y1;
        points.push_back(point);
        point.x = x+x1;
        point.y=my+y1;
        points.push_back(point);
        point.x = y+x1;
        point.y=x+y1;
        points.push_back(point);
        point.x = y+x1;
        point.y=mx+y1;
        points.push_back(point);
		while (y>x)
		{
			if (d<0){
				d+=(x<<1)+3;
			}else{
				d+=((x-y)<<1)+5;
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
            point.x = y+x1;
            point.y=x+y1;
            points.push_back(point);
            point.x = y+x1;
            point.y=mx+y1;
            points.push_back(point);
		}
        int xmin, xmax, ymin, ymax;
        xmin=fmin(mVertices[0][0]-r, mVertices[0][0]+r);
        ymin=fmin(mVertices[0][1]-r,mVertices[0][1]+r);
        xmax=fmax(mVertices[0][0]-r, mVertices[0][0]+r);
        ymax=fmax(mVertices[0][1]-r,mVertices[0][1]+r);
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
            glColor3f(mColor[0],mColor[1], mColor[2]);
        }
    if (fFigure){
        //Ordenarlos
        std::sort(points.begin(), points.end(), compareCircle);
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

void CCircle::hLine (int xmin, int ymin, int xmax)
{
    glColor3f(fColor[0],fColor[1], fColor[2]);
    for (int i= xmin; i<xmax; i++){
        glVertex2d(i, ymin);
    }
    glColor3f(mColor[0],mColor[1], mColor[2]);
}
