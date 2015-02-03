#pragma once

#include "Figure.h"

class CQuad : public CFigure
{
	public:
		CQuad();
		~CQuad();
		void display();
        void hLine (int xmin, int ymin, int xmax);

};

