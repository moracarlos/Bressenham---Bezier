#include "Main.h"
#include "UserInterface.h"
#include "Line.h"
#include "Quad.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Triangle.h"
#include "Curve.h"
#include <fstream>



using std::vector;
using std::stack;

struct state{
    float mVertices[3][2], mColor[3], fColor[3];
    bool bBox;
    bool fFigure;
    int mType;
    float xs[2];
    float ys[2];
    Actions action;
    unsigned long pos;
    int tipo;
};
GLFWwindow *gWindow;
int gWidth, gHeight;
bool gPress;
int tClicks;
CUserInterface * userInterface;
vector <CFigure *> figures;
FigureType figureSelected;
int picked;
int pointPicked;
int px, py;
CFigure* fPicked;
stack< state > states1;
stack< state > states2;
state figAnt;
bool drawingCurve;
int xv1, xv2, yv1, yv2, xv3, yv3;
float **moveVertices;
float moveControlPoint[2];


void pick (int x, int y)
{
    int i;
    //Verificar si hay una figura picked, si la hay buscar si hay coincidencia con un punto de control, si lo hay punto de control ejecutar el codigo. Si hay coincidencia definir el picked de punto de control y salir de esta funcion
    if (picked>-1 && figures[picked]->getType()==CURVE ){
        pointPicked = -1;
        for (i = 0; i<figures[picked]->getnVertices(); i++){
            if (x >= figures[picked]->getVertex(i)[0]-10 && x <= figures[picked]->getVertex(i)[0]+10 && y >= figures[picked]->getVertex(i)[1]-10 && y <= figures[picked]->getVertex(i)[1]+10)
            {
                pointPicked=i;
                moveControlPoint[0]=figures[picked]->getVertex(i)[0];
                moveControlPoint[1]=figures[picked]->getVertex(i)[1];
                break;
            }
        }
    }
    if (pointPicked==-1){
    picked = -1;
        userInterface->hideGroup();
    
    for (i = 0; i < figures.size(); i++)
    {
        float *xs=figures[i]->getXs();
        float *ys=figures[i]->getYs();
        float max[2];
        float min[2];
        min[0]=xs[0]; //xmin
        min[1]=ys[0]; //ymin
        max[0]=xs[1]; //xmax
        max[1]=ys[1]; //ymax
        if (x >= min[0] && x <= max[0] && y >= min[1] && y <= max[1])
        {
            picked = i;
            if (figures[picked]->getType()==CURVE){
                moveVertices = (float**) malloc (sizeof(float*)*figures[picked]->getnVertices());
                for (int k=0; k<figures[picked]->getnVertices(); k++)
                {
                    moveVertices[k]=(float*) malloc (sizeof(float)*2);
                    memcpy(moveVertices[k], figures[picked]->getVertex(k), sizeof(float)*2);
                }
            }else{
                xv1 = figures[picked]->getVertex(0)[0];
                yv1 = figures[picked]->getVertex(0)[1];
                xv2 = figures[picked]->getVertex(1)[0];
                yv2 = figures[picked]->getVertex(1)[1];
                if (figures[picked]->getType()==TRIANGLE){
                    xv3=figures[picked]->getVertex(2)[0];
                    yv3=figures[picked]->getVertex(2)[1];
                }
            }
            userInterface->setCurveDegree(figures[picked]->getDegree());
            userInterface->setFigureColor(figures[picked]->getFillColor());
            userInterface->setFigureBorder(figures[picked]->getBorderColor());
            userInterface->setmFill(figures[picked]->fillFigure());
            userInterface->setmBbox(figures[picked]->viewBox());
            userInterface->show();
            
            int type = figures[picked]->getType();
            
            if (type == LINE)
                userInterface->setFigureType("Line");
            else if (type == QUAD)
                userInterface->setFigureType("Quad");
            else if (type == CIRCLE)
                userInterface->setFigureType("Circle");
            else if (type == TRIANGLE)
                userInterface->setFigureType("Triangle");
            else if (type == ELLIPSE)
                userInterface->setFigureType("Ellipse");
            else if (type == CURVE)
                userInterface->setFigureType("Curve");
            break;
        }
    }
    }
}


void updateUserInterface()
{
    if (picked > -1)
    {
        float * color = userInterface->getFigureColor();
        figures[picked]->setFillColor(color[0], color[1], color[2]);
        color = userInterface->getFigureBorder();
        figures[picked]->setColor(color[0], color[1], color[2]);
        figures[picked]->setBoolFill(userInterface->getmFill());
        figures[picked]->viewBox(userInterface->getmBbox());
        figures[picked]->setDegree(userInterface->getDegree());
        }
    }

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < figures.size(); i++){
		figures[i]->display();
    }
}

void reshape(GLFWwindow *window, int width, int height)
{
	gWidth = width;
	gHeight = height;

	glViewport(0, 0, gWidth, gHeight);
    
    userInterface->reshape();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, gWidth, 0, gHeight, -1.0f, 1.0f);
}

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (TwEventKeyGLFW(key, action))
        return;
    
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
            case GLFW_KEY_P:
                figureSelected = NONE;
                picked=-1;
                pointPicked = -1;
                break;
			case GLFW_KEY_L:
				figureSelected = LINE;
                picked=-1;
                pointPicked = -1;
				break;
			case GLFW_KEY_Q:
				figureSelected = QUAD;
                picked=-1;
                pointPicked = -1;
				break;
			case GLFW_KEY_C:
				figureSelected = CIRCLE;
                picked=-1;
                pointPicked = -1;
                break;
            case GLFW_KEY_E:
                figureSelected = ELLIPSE;
                picked=-1;
                pointPicked = -1;
                break;
            case GLFW_KEY_T:
                figureSelected = TRIANGLE;
                picked=-1;
                pointPicked = -1;
                break;
            case GLFW_KEY_B:
                figureSelected = CURVE;
                picked=-1;
                pointPicked = -1;
                drawingCurve=false;
                break;
		}
	}
}

state toState(CFigure *fig, unsigned long pos){
    state aux;
    aux.mVertices[0][0]=fig->getVertex(0)[0];
    aux.mVertices[0][1]=fig->getVertex(0)[1];
    aux.mVertices[1][0]=fig->getVertex(1)[0];
    aux.mVertices[1][1]=fig->getVertex(1)[1];
    if (fig->getType()==TRIANGLE)
    {
        aux.mVertices[2][0]=fig->getVertex(2)[0];
        aux.mVertices[2][1]=fig->getVertex(2)[1];
    }
    aux.mColor[0]=userInterface->getFigureBorder()[0];
    aux.mColor[1]=userInterface->getFigureBorder()[1];
    aux.mColor[2]=userInterface->getFigureBorder()[2];
    aux.fColor[0]=userInterface->getFigureColor()[0];
    aux.fColor[1]=userInterface->getFigureColor()[1];
    aux.fColor[2]=userInterface->getFigureColor()[2];
    aux.mType = fig->getType();
    aux.fFigure=userInterface->getmFill();
    aux.bBox=userInterface->getmBbox();
    aux.action = ADD;
    aux.pos=pos;
    if (fig->getType()==LINE) aux.tipo =1;
    if (fig->getType()==CIRCLE) aux.tipo =2;
    if (fig->getType()==TRIANGLE) aux.tipo =3;
    if (fig->getType()==ELLIPSE) aux.tipo =4;
    if (fig->getType()==QUAD) aux.tipo =5;
    
    return aux;
}

void loadF(){
    std::fstream filePtr("/Users/moracarlos/Desktop/Paint Grafica/Paint Grafica/figuras.txt", std::fstream::in | std::fstream::out);
    figures.clear();
    CFigure *f;
    float r,g,b, v1,v2;
    int n, tipo;
    filePtr>>n;
    if (n==0) return;
    for (int i=0; i<n; i++){
        filePtr >> tipo;
        switch (tipo) {
            case LINE:
                f = new CLine();
                break;
            case QUAD:
                f = new CQuad();
                break;
            case CIRCLE:
                f = new CCircle();
                break;
            case ELLIPSE:
                f = new CEllipse();
                break;
            case TRIANGLE:
                f = new CTriangle();
                break;
            default:
                f = new CLine();
                break;
                
        }
        
        filePtr>>v1;
        filePtr>>v2;
        f->setVertex(0, v1, v2);
        filePtr>>v1;
        filePtr>>v2;
        f->setVertex(1, v1, v2);
        if (tipo==TRIANGLE) {
            filePtr>>v1;
            filePtr>>v2;
            f->setVertex(2, v1, v2);
        }

        filePtr>>v1;
        f->viewBox(v1);
        
        filePtr>>v1;
        f->fillFigure(v1);
        
        filePtr>>r>>g>>b;
        f->setColor(r, g, b);
        
        filePtr>>r>>g>>b;
        f->setFillColor(r, g, b);
        
        figures.push_back(f);
    }

}

void mouseButton(GLFWwindow* window, int button, int action, int mods) //Falta picked
{
    if (TwEventMouseButtonGLFW(button, action)){
        drawingCurve=false;
        if (button==0 && action==0){
            if (userInterface->getLast()==BOX && picked>-1){
                    figAnt=toState(figures[picked], picked);
                    figAnt.action=SHOWBBOX;
                    figAnt.bBox=userInterface->getmBbox();
                    figAnt.mType=figures[picked]->getType();
                    states1.push(figAnt);
                    while (!states2.empty())
                        states2.pop();
            }else if (userInterface->getLast()==PAINT && picked>-1){
                    figAnt=toState(figures[picked], picked);
                    figAnt.action=FILL;
                    figAnt.bBox=userInterface->getmBbox();
                    figAnt.mType=figures[picked]->getType();
                    states1.push(figAnt);
                    while (!states2.empty())
                        states2.pop();
            }else if (userInterface->getLast()==COLOR && picked>-1){
                    figAnt=toState(figures[picked], picked);
                    figAnt.action=CHANGEFILL;
                    figAnt.bBox=userInterface->getmBbox();
                    figAnt.mType=figures[picked]->getType();
                    states1.push(figAnt);
                    while (!states2.empty())
                        states2.pop();
            }else if (userInterface->getLast()==BORDER && picked>-1){
                figAnt=toState(figures[picked], picked);
                figAnt.action=CHANGEBORDER;
                figAnt.bBox=userInterface->getmBbox();
                figAnt.mType=figures[picked]->getType();
                states1.push(figAnt);
                while (!states2.empty())
                    states2.pop();
            }
        }
        return;
    }
    
    double x, y;
    glfwGetCursorPos(gWindow, &x, &y);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !gPress && tClicks==0 &&!drawingCurve)
	{
		float ax = x;
		float ay = gHeight - y;
        if (figureSelected == NONE)
        {
            gPress=true;
            px=ax;
            py=ay;
            pick(ax, ay);

        }else if (figureSelected == LINE)
		{
			CLine *line = new CLine();
			line->setVertex(0, ax, ay);
			line->setVertex(1, ax, ay);
            line->setColor(userInterface->getFigureBorder()[0], userInterface->getFigureBorder()[1], userInterface->getFigureBorder()[2]);
            line->viewBox(userInterface->getmBbox());
			figures.push_back(line);
            figAnt=toState(line, figures.size()-1);
            gPress = true;

		}
		else if (figureSelected == QUAD)
		{
			CQuad *quad = new CQuad();
			quad->setVertex(0, ax, ay);
			quad->setVertex(1, ax, ay);
            quad->setBoolFill(userInterface->getmFill());
            quad->setColor(userInterface->getFigureBorder()[0], userInterface->getFigureBorder()[1], userInterface->getFigureBorder()[2]);
            quad->setFillColor (userInterface->getFigureColor()[0], userInterface->getFigureColor()[1], userInterface->getFigureColor()[2]);
            quad->viewBox(userInterface->getmBbox());
			figures.push_back(quad);
            gPress = true;
            figAnt=toState(quad,figures.size()-1);
		}
		else if (figureSelected == CIRCLE)
		{
			CCircle *circle = new CCircle();
			circle->setVertex(0, ax, ay);
			circle->setVertex(1, ax, ay);
            circle->setBoolFill(userInterface->getmFill());
            circle->setColor(userInterface->getFigureBorder()[0], userInterface->getFigureBorder()[1], userInterface->getFigureBorder()[2]);
            circle->setFillColor (userInterface->getFigureColor()[0], userInterface->getFigureColor()[1], userInterface->getFigureColor()[2]);
            circle->viewBox(userInterface->getmBbox());
			figures.push_back(circle);
            figAnt=toState(circle, figures.size()-1);
            gPress = true;
		}
        else if (figureSelected == ELLIPSE)
        {
            CEllipse *ellipse = new CEllipse();
            ellipse->setVertex(0, ax, ay);
            ellipse->setVertex(1, ax, ay);
            ellipse->setBoolFill(userInterface->getmFill());
            ellipse->setColor(userInterface->getFigureBorder()[0], userInterface->getFigureBorder()[1], userInterface->getFigureBorder()[2]);
            ellipse->setFillColor (userInterface->getFigureColor()[0], userInterface->getFigureColor()[1], userInterface->getFigureColor()[2]);
            ellipse->viewBox(userInterface->getmBbox());
            figures.push_back(ellipse);
            figAnt=toState(ellipse, figures.size()-1);
            gPress = true;
        }
        else if (figureSelected == TRIANGLE)
        {
            tClicks=1;
            CTriangle *triangle = new CTriangle();
            triangle->setVertex(0, ax, ay);
            triangle->setVertex(1, ax, ay);
            triangle->setVertex(2, ax, ay);
            triangle->setBoolFill(userInterface->getmFill());
            triangle->setColor(userInterface->getFigureBorder()[0], userInterface->getFigureBorder()[1], userInterface->getFigureBorder()[2]);
            triangle->setFillColor (userInterface->getFigureColor()[0], userInterface->getFigureColor()[1], userInterface->getFigureColor()[2]);
            triangle->viewBox(userInterface->getmBbox());
            figures.push_back(triangle);
            figAnt=toState(triangle, figures.size()-1);
            gPress = true;
        }else if (figureSelected == CURVE)
        {
            CCurve *curve = new CCurve();
            curve->addVertex(ax, ay);
            curve->setBoolFill(userInterface->getmFill());
            curve->setDegree(0);
            curve->setColor(userInterface->getFigureBorder()[0], userInterface->getFigureBorder()[1], userInterface->getFigureBorder()[2]);
            curve->viewBox(userInterface->getmBbox());
            curve->setXs(gWidth, 0);
            curve->setYs(gHeight, 0);
            figures.push_back(curve);
            gPress = true;
            drawingCurve=true;
        }

    }else{
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !gPress && tClicks==1){
            tClicks++;
        }
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !gPress && drawingCurve){
            figures.back()->addVertex(x, gHeight-y);
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		gPress = false;
        if (figureSelected!=TRIANGLE || (figureSelected==TRIANGLE && tClicks==2)){
            
            if (picked>-1 && figures[picked]->getType()!=CURVE){
                figAnt=toState(figures[picked], picked);
                figAnt.action=MOVE;
                figAnt.mType=figures[picked]->getType();
                states1.push(figAnt);
                while (!states2.empty())
                    states2.pop();
            }else{
                states1.push(figAnt);
                while (!states2.empty())
                    states2.pop();
            }
            if (tClicks==2)
                tClicks=0;
            }
    }
}



void cursorPos(GLFWwindow* window, double x, double y)
{
    if (TwEventMousePosGLFW(x, y))
        return;
    
	if (gPress && figureSelected!=TRIANGLE && figureSelected!=NONE && figureSelected!=CURVE)
	{
		float ax = x;
		float ay = gHeight - y;

		figures.back()->setVertex(1, ax, ay);
        figAnt.mVertices[1][0]=ax;
        figAnt.mVertices[1][1]=ay;

	}
    if (gPress && figureSelected==TRIANGLE){
        float ax = x;
        float ay = gHeight - y;
        
        figures.back()->setVertex(1, ax, ay);
        
        figAnt.mVertices[1][0]=ax;
        figAnt.mVertices[1][1]=ay;
    }
    if (!gPress && figureSelected==TRIANGLE && tClicks==1){
        float ax = x;
        float ay = gHeight - y;
        
        figures.back()->setVertex(2, ax, ay);
        figAnt.mVertices[2][0]=ax;
        figAnt.mVertices[2][1]=ay;
    }
    if (gPress && figureSelected==TRIANGLE && tClicks==2){
        float ax = x;
        float ay = gHeight - y;
        
        figures.back()->setVertex(2, ax, ay);
        figAnt.mVertices[2][0]=ax;
        figAnt.mVertices[2][1]=ay;
    }
    if (gPress && picked!=-1 && figureSelected==NONE && pointPicked==-1){
        int dx, dy;
        dx=x-px;
        dy=(gHeight-y)-py;
        
        if (figures[picked]->getType()==CURVE){
            for (int w=0; w<figures[picked]->getnVertices(); w++){
                figures[picked]->setVertex(w,moveVertices[w][0]+dx, moveVertices[w][1]+dy);
            }
        }else{
            figures[picked]->setVertex(0,xv1+dx, yv1+dy);
            figures[picked]->setVertex(1,xv2+dx, yv2+dy);
            figAnt.mVertices[0][0]=xv1+dx;
            figAnt.mVertices[0][1]=yv1+dy;
            figAnt.mVertices[1][0]=xv2+dx;
            figAnt.mVertices[1][1]=yv2+dy;
        
            figAnt.action=MOVE;
            if (figures[picked]->getType()==TRIANGLE){
                figures[picked]->setVertex(2,xv3+dx, yv3+dy);
                figAnt.mVertices[2][0]=xv3+dx;
                figAnt.mVertices[2][1]=yv3+dy;
            }
        }
    }
    if (gPress && picked!=-1 && figureSelected==NONE && pointPicked>-1){
        int dx, dy;
        dx=x-px;
        dy=(gHeight-y)-py;
        
        figures[picked]->setVertex(pointPicked,moveControlPoint[0]+dx, moveControlPoint[1]+dy);
    }

}

void scroll (GLFWwindow* window, double x, double y)
{
    if (picked!=-1){
        if (figures[picked]->getType()==CURVE){
            float scaleFactor;
            if (y<0)
                scaleFactor = 1.1;
            else
                scaleFactor = 0.9;
            float px = (figures[picked]->getXs()[0]+figures[picked]->getXs()[1])/2;
            float py = (figures[picked]->getYs()[0]+figures[picked]->getYs()[1])/2;
            float nx, ny;

            for (int i=0; i<figures[picked]->getnVertices(); i++){
                nx = figures[picked]->getVertex(i)[0]*scaleFactor+px*(1-scaleFactor);
                ny = figures[picked]->getVertex(i)[1]*scaleFactor+py*(1-scaleFactor);
                figures[picked]->setVertex(i, nx, ny);
            }
        }
    }
}

void destroy()
{
	for (int i = 0; i < figures.size(); i++)
		delete figures[i];

	glfwDestroyWindow(gWindow);
	glfwTerminate();
}

void charInput(GLFWwindow* window, unsigned int scanChar)
{
    if (TwEventCharGLFW(scanChar, GLFW_PRESS))
        return;
}

bool initGlfw()
{
	if (!glfwInit())
		return false;

	gWindow = glfwCreateWindow(gWidth, gHeight, "Simple example", NULL, NULL);

	if (!gWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow);

	const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(gWindow, (vidMode->width - gWidth) >> 1, (vidMode->height - gHeight) >> 1);

	glfwSetWindowSizeCallback(gWindow, reshape);
	glfwSetKeyCallback(gWindow, keyInput);
	glfwSetMouseButtonCallback(gWindow, mouseButton);
	glfwSetCursorPosCallback(gWindow, cursorPos);
    glfwSetCharCallback(gWindow, charInput);
    glfwSetScrollCallback(gWindow, scroll);
	
	return true;
}

bool initUserInterface()
{
    if (!TwInit(TW_OPENGL, NULL))
        return false;
    userInterface = new CUserInterface();
    return true;
}

void subd(std::vector <pointPair> &curva1, std::vector<pointPair> &curva2)
{
    int j;
    CCurve *curve = new CCurve();
    for (int i=0; i<curva1.size(); i++)
    {
        curve->addVertex(curva1[i].x, curva1[i].y);
    }
        curve->setBoolFill(userInterface->getmFill());
        curve->setColor(userInterface->getFigureBorder()[0], userInterface->getFigureBorder()[1], userInterface->getFigureBorder()[2]);
        curve->setFillColor (userInterface->getFigureColor()[0], userInterface->getFigureColor()[1], userInterface->getFigureColor()[2]);
        curve->viewBox(userInterface->getmBbox());
        figures.push_back(curve);
    CCurve *curva = new CCurve();
    for (j=int(curva2.size())-1; j>=0; j--)
    {
        curva->addVertex(curva2[j].x, curva2[j].y);
    }
    curva->setBoolFill(userInterface->getmFill());
    curva->setColor(userInterface->getFigureBorder()[0], userInterface->getFigureBorder()[1], userInterface->getFigureBorder()[2]);
    curva->setFillColor (userInterface->getFigureColor()[0], userInterface->getFigureColor()[1], userInterface->getFigureColor()[2]);
    curva->viewBox(userInterface->getmBbox());
    figures.push_back(curva);
    
    
    figures.erase (figures.begin()+picked);
    picked=-1;
}


int main(void)
{
	gWidth = 1024;
	gHeight = 768;
	gPress = false;
    tClicks=0;
	figureSelected = LINE;
    picked = -1;
    pointPicked = -1;
    drawingCurve=false;

	if (!initGlfw() || !initUserInterface())
		return 1;

	reshape(gWindow, gWidth, gHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	while (!glfwWindowShouldClose(gWindow))
	{
		display();
        
        TwDraw();
        
        updateUserInterface();
        
        glfwSwapBuffers(gWindow);

		glfwPollEvents();
	}
	
	destroy();

	return 0;
}