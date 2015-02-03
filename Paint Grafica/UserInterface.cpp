#include "UserInterface.h"
#include "Figure.h"



using std::stack;
using std::vector;

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


extern int gWidth, gHeight;
extern CUserInterface* userInterface;
extern vector <CFigure *> figures;
extern FigureType figureSelected;
extern stack< state> states1;
extern stack< state> states2;
extern vector <CFigure *> figures;
extern void loadF();
extern int picked;
extern int pointPicked;
extern void subd(std::vector <pointPair> &curva1, std::vector<pointPair> &curva2);
TwActions lastAction;

void TW_CALL bboxSet(const void *value, void *clientData)
{
    lastAction=BOX;
    userInterface->setmBbox(*(const bool*)value);

}

void TW_CALL bboxGet(void *value, void *clientData)
{
    *(bool *)value = userInterface->getmBbox();  // for instance
    
}

void TW_CALL mfillSet(const void *value, void *clientData)
{
    lastAction=PAINT;
    userInterface->setmFill(*(const bool*)value);
    
}

void TW_CALL mfillGet(void *value, void *clientData)
{
    *(bool *)value = userInterface->getmFill();
}

void TW_CALL fColorSet(const void *value, void *clientData)
{
    lastAction=COLOR;
    userInterface->setFigureColor((float*)value);
}

void TW_CALL fColorGet(void *value, void *clientData)
{
    ((GLfloat*) value)[0] = userInterface->getFigureColor()[0];
    ((GLfloat*) value)[1] = userInterface->getFigureColor()[1];
    ((GLfloat*) value)[2] = userInterface->getFigureColor()[2];
}

void TW_CALL fBorderSet(const void *value, void *clientData)
{
    lastAction=BORDER;
    userInterface->setFigureBorder((float*)value);
}

void TW_CALL fBorderGet(void *value, void *clientData)
{
    ((GLfloat*) value)[0] = userInterface->getFigureBorder()[0];
    ((GLfloat*) value)[1] = userInterface->getFigureBorder()[1];
    ((GLfloat*) value)[2] = userInterface->getFigureBorder()[2];
}

void TW_CALL degreeSet(const void *value, void *clientData)
{
    if (figures[picked]->getType()==CURVE){
        userInterface->setCurveDegree(*(const float*)value);
        float x,y,nx,ny;
        float px, py;
        px = (figures[picked]->getXs()[0]+figures[picked]->getXs()[1])/2;
        py = (figures[picked]->getYs()[0]+figures[picked]->getYs()[1])/2;
    
        float d = (*(const float*)value - figures[picked]->getDegree()) *3.14159265/180;;
        for (int i=0; i<figures[picked]->getnVertices(); i++){
            x=figures[picked]->getVertex(i)[0];
            y=figures[picked]->getVertex(i)[1];
        
            nx = px + (x-px)*cos(d)-(y-py)*sin(d);
            ny = py + (x-px)*sin(d)+(y-py)*cos(d);
            figures[picked]->setVertex(i, nx, ny);
            figures[picked]->setVertex(i, nx, ny);
        }
    }
}

void TW_CALL degreeGet(void *value, void *clientData)
{
    *(float *)value = userInterface->getDegree(); 
}

void TW_CALL save(void *args){
    picked=-1;
     pointPicked=-1;
    FILE *filePtr;
    filePtr = fopen("/Users/moracarlos/Desktop/Paint Grafica/Paint Grafica/figuras.txt", "w");
    fprintf(filePtr, "%lu\n", figures.size());
    for (int i=0; i<figures.size(); i++){
        if (figures[i]->getType()!=CURVE){
        fprintf(filePtr, "%d ", figures[i]->getType());
        
        fprintf(filePtr, "%f ", figures[i]->getVertex(0)[0]);
        fprintf(filePtr, "%f ", figures[i]->getVertex(0)[1]);
        fprintf(filePtr, "%f ", figures[i]->getVertex(1)[0]);
        fprintf(filePtr, "%f ", figures[i]->getVertex(1)[1]);
        if (figures[i]->getType()==TRIANGLE){
            fprintf(filePtr, "%f ", figures[i]->getVertex(2)[0]);
            fprintf(filePtr, "%f ", figures[i]->getVertex(2)[1]);
        }
        
        fprintf(filePtr, "%d ", figures[i]->viewBox());
        
        fprintf(filePtr, "%d ", figures[i]->fillFigure());
        
        fprintf(filePtr, "%f ", figures[i]->getBorderColor()[0]);
        fprintf(filePtr, "%f ", figures[i]->getBorderColor()[1]);
        fprintf(filePtr, "%f ", figures[i]->getBorderColor()[2]);
        
        fprintf(filePtr, "%f ", figures[i]->getFillColor()[0]);
        fprintf(filePtr, "%f ", figures[i]->getFillColor()[1]);
        fprintf(filePtr, "%f ", figures[i]->getFillColor()[2]);
        }
    }
    fclose(filePtr);
}


void TW_CALL load(void *args){
    picked=-1;
    pointPicked=-1;
    loadF();
}

void TW_CALL buttonPressed(void *args)
{
    int x = *((int *)(&args));
    if (x==1)
        figureSelected = LINE;
    else if (x==2)
        figureSelected = CIRCLE;
    else if (x==3)
        figureSelected = TRIANGLE;
    else if (x==4)
        figureSelected = ELLIPSE;
    else if (x==5)
        figureSelected = QUAD;
    else if (x==6)
        figureSelected = NONE;
    else if (x==7)
        figureSelected = CURVE;
    lastAction = NO;
    picked=-1;
    pointPicked = -1;
}

void TW_CALL undo(void *args){
    if (figureSelected!=CURVE){
    picked=-1;
    pointPicked = -1;
    lastAction=NO;
    if (states1.empty()){
        return;
    }
    
    state figAnt;
    stack <state> aux;
    figAnt=states1.top();
    states1.pop();
    switch (figAnt.action){
        case ADD:
            figures.pop_back();
            break;
        case MOVE:
            while (states1.top().mType != figAnt.mType ){
                aux.push(states1.top());
                states1.pop();
            }
            figAnt=states1.top();
            figures[figAnt.pos]->setVertex(0, figAnt.mVertices[0][0], figAnt.mVertices[0][1]);
            figures[figAnt.pos]->setVertex(1, figAnt.mVertices[1][0], figAnt.mVertices[1][1]);
            if (figures[figAnt.pos]->getType()==TRIANGLE)
                figures[figAnt.pos]->setVertex(2, figAnt.mVertices[2][0], figAnt.mVertices[2][1]);
            while (!aux.empty()){
                states1.push(aux.top());
                aux.pop();
            }
            break;
        case SHOWBBOX:
            while (states1.top().mType != figAnt.mType ){
                aux.push(states1.top());
                states1.pop();
            }
            userInterface->setmBbox(states1.top().bBox);
            while (!aux.empty()){
                states1.push(aux.top());
                aux.pop();
            }
            break;
        case FILL:
            while (states1.top().mType != figAnt.mType ){
                aux.push(states1.top());
                states1.pop();
            }
            userInterface->setmFill(states1.top().fFigure);
            while (!aux.empty()){
                states1.push(aux.top());
                aux.pop();
            }
            break;
        case CHANGEFILL:
            while (states1.top().mType != figAnt.mType ){
                aux.push(states1.top());
                states1.pop();
            }
            userInterface->setFigureColor(states1.top().fColor);
            while (!aux.empty()){
                states1.push(aux.top());
                aux.pop();
            }
            break;
        case CHANGEBORDER:
            while (states1.top().mType != figAnt.mType ){
                aux.push(states1.top());
                states1.pop();
            }
            userInterface->setFigureBorder(states1.top().mColor);
            while (!aux.empty()){
                states1.push(aux.top());
                aux.pop();
            }
            break;
    }
    states2.push(figAnt);
    }
}

void TW_CALL redo (void * args)
{
   /*
    picked=-1;
    pointPicked = -1;
    lastAction=NO;
    if (states2.empty()){
        return;
    }
    
    state figAnt;
    stack <state> aux;
    figAnt=states2.top();
    states2.pop();
    switch (figAnt.action){
        case ADD:
            
            
            
            figures.pop_back();
            break;
        case MOVE:
            while (states2.top().mType != figAnt.mType ){
                aux.push(states2.top());
                states2.pop();
            }
            figAnt=states2.top();
            figures[figAnt.pos]->setVertex(0, figAnt.mVertices[0][0], figAnt.mVertices[0][1]);
            figures[figAnt.pos]->setVertex(1, figAnt.mVertices[1][0], figAnt.mVertices[1][1]);
            if (figures[figAnt.pos]->getType()==TRIANGLE)
                figures[figAnt.pos]->setVertex(2, figAnt.mVertices[2][0], figAnt.mVertices[2][1]);
            while (!aux.empty()){
                states2.push(aux.top());
                aux.pop();
            }
            break;
        case SHOWBBOX:
            while (states2.top().mType != figAnt.mType ){
                aux.push(states2.top());
                states2.pop();
            }
            userInterface->setmBbox(states2.top().bBox);
            while (!aux.empty()){
                states2.push(aux.top());
                aux.pop();
            }
            break;
        case FILL:
            while (states2.top().mType != figAnt.mType ){
                aux.push(states2.top());
                states2.pop();
            }
            userInterface->setmFill(states2.top().fFigure);
            while (!aux.empty()){
                states2.push(aux.top());
                aux.pop();
            }
            break;
        case CHANGEFILL:
            while (states2.top().mType != figAnt.mType ){
                aux.push(states2.top());
                states2.pop();
            }
            userInterface->setFigureColor(states2.top().fColor);
            while (!aux.empty()){
                states2.push(aux.top());
                aux.pop();
            }
            break;
        case CHANGEBORDER:
            while (states2.top().mType != figAnt.mType ){
                aux.push(states2.top());
                states2.pop();
            }
            userInterface->setFigureBorder(states2.top().mColor);
            while (!aux.empty()){
                states2.push(aux.top());
                aux.pop();
            }
            break;
    }
    states1.push(figAnt);
*/
}

void TW_CALL degreeElev (void * args)
{
    if (picked > -1 && figures[picked]->getType()==CURVE){
        figures[picked]->elevateDegree();
        }
}

void TW_CALL deletePoint (void *args)
{
    if (picked>-1 && pointPicked>-1 && figures[picked]->getType()==CURVE && figures[picked]->getnVertices()>1)
    {
        figures[picked]->deletePoint(pointPicked);
    }
}

void TW_CALL subdividirSet(const void *value, void *clientData)
{
    userInterface->setT(*(const float*)value);    
}

void TW_CALL subdividirGet(void *value, void *clientData)
{
    *(float *)value = userInterface->getT();  // for instance
    
}

void TW_CALL divide (void * args){
    float t=userInterface->getT();
    if (picked>-1 && figures[picked]->getType()==CURVE && t!=0 && t!=1){
        int nVertices = figures[picked]->getnVertices();
        float** mVertices = figures[picked]->getmVertices();
        float** cPoints= new float*[nVertices];
        pointPair nuevo;
        pointPair punto;
        std::vector <pointPair> curva2;
        std::vector<pointPair> curva1;
        int i,j,act;
        float a,b;
        act=nVertices;
        int last=nVertices-1;
        t = userInterface->getT();
        memcpy(cPoints, mVertices, sizeof(float*)*nVertices);
        for (i=0; i<nVertices; i++){
            cPoints[i] = (float*) malloc (sizeof(float)*2);
            memcpy(cPoints[i], mVertices[i], sizeof(float)*2);
        }
        punto.x=cPoints[0][0];
        punto.y=cPoints[0][1];
        curva1.push_back(punto);
        punto.x=cPoints[last][0];
        punto.y=cPoints[last][1];
        curva2.push_back(punto);
        for (i=0; i<nVertices; i++){
            for (j=0; j<act-i-1;j++){
                a=(1-t)*cPoints[j][0]+t*cPoints[j+1][0];
                b=(1-t)*cPoints[j][1]+t*cPoints[j+1][1];
                cPoints[j][0]=a;
                cPoints[j][1]=b;
                nuevo.x=a;
                nuevo.y=b;
                }
            last--;
            if (last>-1){
                punto.x=cPoints[0][0];
                punto.y=cPoints[0][1];
                curva1.push_back(punto);
                punto.x=cPoints[last][0];
                punto.y=cPoints[last][1];
                curva2.push_back(punto);
            }
        }
        //agregar la figura
        subd(curva1, curva2);
    }
}

CUserInterface::CUserInterface()
{
    lastAction=NO;
    mBbox = false;
    mFill = false;
    mFigureColor[0]=1.0f;
    mFigureColor[1]=1.0f;
    mFigureColor[2]=1.0f;
    
    mFigureBorder[0]=1.0f;
    mFigureBorder[1]=1.0f;
    mFigureBorder[2]=1.0f;
    
    degree=0;
    t=0;
    mUserInterface = TwNewBar("Figure");
    
    TwDefine("Figure refresh = '0.0001f'");
    TwDefine("Figure resizable = false");
    TwDefine("Figure fontresizable = false");
    TwDefine("Figure movable = false");
    TwDefine("Figure visible = true");
    TwDefine("Figure position = '20 20'");
    TwDefine("Figure size = '220 500'");
    
    //Variables dentro del menu

    TwAddSeparator(mUserInterface, NULL, " group='Figures' ");
    TwAddSeparator(mUserInterface, NULL, "group='Actions'");
    TwAddSeparator(mUserInterface, NULL, "group='Drawing'");
    TwAddSeparator(mUserInterface, NULL, "group='CurveActions'");

    
    
    
    TwAddVarCB(mUserInterface, "degree", TW_TYPE_FLOAT, degreeSet, degreeGet, &degree, "label = 'Rotacion' min=0 max=360 group='CurveActions'");
    TwAddVarCB(mUserInterface, "bbox", TW_TYPE_BOOLCPP, bboxSet, bboxGet, &mBbox, "label = 'Bounding Box' true=Enabled false=Disabled group='Drawing'");
    TwAddVarCB(mUserInterface, "Fill", TW_TYPE_BOOLCPP, mfillSet, mfillGet, &mFill, "label = 'Fill' true=Enabled false=Disabled group='Drawing'");
    TwAddVarCB(mUserInterface, "color", TW_TYPE_COLOR3F, fColorSet, fColorGet, &mFigureColor[0], "label = Relleno group='Drawing'");
    TwAddVarCB(mUserInterface, "border", TW_TYPE_COLOR3F, fBorderSet, fBorderGet, &mFigureBorder[0], "label = Borde group='Drawing'");
    
    TwAddButton(mUserInterface, "Line", buttonPressed, (void*)1, "group = 'Figures'");
    TwAddButton(mUserInterface, "Circle", buttonPressed, (void*)2, "group = 'Figures'");
    TwAddButton(mUserInterface, "Triangle", buttonPressed, (void*)3, "group = 'Figures'");
    TwAddButton(mUserInterface, "Ellipse", buttonPressed, (void*)4, "group = 'Figures'");
    TwAddButton(mUserInterface, "Quad", buttonPressed, (void*)5, "group = 'Figures'");
    TwAddButton(mUserInterface, "Curve", buttonPressed, (void*)7, "group = 'Figures'");
    TwAddButton(mUserInterface, "Elevar Grado", degreeElev, NULL, "group='CurveActions'");
    TwAddButton(mUserInterface, "Eliminar Punto", deletePoint, NULL, "group='CurveActions'");
    TwAddVarCB(mUserInterface, "t", TW_TYPE_FLOAT, subdividirSet, subdividirGet, &t, "label = Subdividir min=0 max=1 step = 0.01 group='CurveActions'");
    TwAddButton(mUserInterface, "Dividir", divide, NULL, "label = 'Dividir Curva' group='CurveActions'");
    

    TwAddButton(mUserInterface, "Pick", buttonPressed, (void*)6, "group='Actions'");

    
    TwAddButton(mUserInterface, "Undo", undo, NULL, "group='Actions'");
    TwAddButton(mUserInterface, "Redo", redo, NULL, "group='Actions'");
    
    TwAddButton(mUserInterface, "Save", save, NULL, "group='Actions'");
    TwAddButton(mUserInterface, "Load", load, NULL, "group='Actions'");
    TwDefine(" Figure/CurveActions opened=false "); // fold the group 'Properties'
    

}

CUserInterface::~CUserInterface()
{
}

void CUserInterface::reshape()
{
    TwWindowSize(gWidth, gHeight);
}

void CUserInterface::show()
{
    TwDefine("Figure visible = true");
}

void CUserInterface::hide()
{
    TwDefine("Figure visible = false");
}

void CUserInterface::setFigureColor(float *color)
{
    mFigureColor[0] = color[0];
    mFigureColor[1] = color[1];
    mFigureColor[2] = color[2];
}

void CUserInterface::setFigureBorder(float *color)
{
    mFigureBorder[0] = color[0];
    mFigureBorder[1] = color[1];
    mFigureBorder[2] = color[2];
}

void CUserInterface::setFigureType(std::string type)
{
    mFigureType = type;
    if (type=="Curve")
        TwDefine(" Figure/CurveActions opened=true "); // fold the group 'Properties'
    else
        TwDefine(" Figure/CurveActions opened=false "); // fold the group 'Properties'
}

float* CUserInterface::getFigureColor()
{
    return mFigureColor;
}

float* CUserInterface::getFigureBorder()
{
    return mFigureBorder;
}

bool CUserInterface::getmFill(){
    return mFill;
}

void CUserInterface::setmFill(bool option){
    mFill = option;
}

void CUserInterface::setmBbox(bool option){
    mBbox = option;
}


bool CUserInterface::getmBbox(){
    return mBbox;
}

int CUserInterface::getLast(){
    return lastAction;
}

void CUserInterface::setLastAction(TwActions ac){
    lastAction = ac;
}

void CUserInterface::setCurveDegree(float value)
{
    degree =  value;
}

float CUserInterface::getDegree()
{
    return degree;
}

void CUserInterface::setT(float val){
    t= val;
}

float CUserInterface::getT(){
    return t;
}

void CUserInterface::hideGroup()
{
    TwDefine(" Figure/CurveActions opened=false "); // fold the group 'Properties'
}