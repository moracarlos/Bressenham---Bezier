#pragma once

#include "Figure.h"

class CEllipse : public CFigure
{
	public:
		CEllipse();
		~CEllipse();
		void display();
        void hLine (int xmin, int ymin, int xmax);
};
