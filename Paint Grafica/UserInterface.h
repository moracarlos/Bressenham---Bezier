#pragma once

#include <AntTweakBar.h>
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <string>

enum TwActions{
    BOX,
    PAINT,
    COLOR,
    BORDER,
    NO
};

class CUserInterface
{
    TwBar *mUserInterface;
    float mFigureColor[3];
    float mFigureBorder[3];
    std::string mFigureType;
    bool mBbox;
    bool mFill;
    float degree;
    float t;
    std::string s;
    
public:
    CUserInterface();
    ~CUserInterface();
    void reshape();
    void show();
    void hide();
    void setFigureColor(float *color);
    void setFigureType (std::string type);
    float *getFigureColor();
    void setFigureBorder(float *color);
    float *getFigureBorder();
    bool getmFill();
    void setmFill(bool option);
    void setmBbox(bool option);
    bool getmBbox();
    int getLast();
    void setLastAction(TwActions ac);
    void setCurveDegree(float value);
    float getDegree();
    void setT(float val);
    float getT();
    void hideGroup();
};