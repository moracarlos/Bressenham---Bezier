#pragma once

#include "Figure.h"


class CCircle : public CFigure
{
	public:
		CCircle();
		~CCircle();
		void display();
        void hLine (int xmin, int ymin, int xmax);

};
