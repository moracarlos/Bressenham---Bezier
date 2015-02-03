#pragma once

#include "Figure.h"

class CTriangle : public CFigure
{
public:
    CTriangle();
    ~CTriangle();
    void display();
    void line(int x1, int x2, int y1, int y2, std::vector<pointPair>& points);
    void hLine (int xmin, int ymin, int xmax);
};
