#pragma once

#include "Figure.h"

class CCurve : public CFigure
{
public:
    CCurve();
    ~CCurve();
    void display();
    void line(int x1, int x2, int y1, int y2, std::vector<pointPair>& points);
    void hLine (int xmin, int ymin, int xmax);
};
