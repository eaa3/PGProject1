#ifndef _SHAPES_
#define _SHAPES_


#include "Vector3.h"
#include "SDL/SDL.h"
#include <SDL/SDL_opengl.h>
#include "util.h"

struct Color
{
    GLfloat r,g,b;
};

struct Color3f
{
    //0->R,1->G,2->B
    GLfloat color[3];

    Color3f();

    Color3f(GLfloat r, GLfloat g, GLfloat b);


    operator GLfloat*();

};

struct Cube
{
    GLfloat edge;
    Vector3 pos;
    GLfloat angle[3];
    Color3f color[6];

    Cube(GLfloat edge = 0.0f);

    GLfloat& operator[](int i);

    void draw();
};

struct Grid
{
    int w,h;
    Color3f color;
    Vector3 pos;

    Grid(int w, int h, Vector3 pos, Color3f color);


    void draw();
    void draw(GLuint texture);
};


struct Ponto
{
    GLfloat  x, y, z;
};

const int N = 10;
const int M = 10;

class BezierSurface
{
public:

    BezierSurface(float colorR = 0.0f, float colorG = 1.0f, float colorB = 0.0f , int nptscontrole = 50, Vector3 trans = Vector3::ZERO);

    void draw();
    void checkEvent(int type);
    void selectNextCtrlPoint();


    Vector3 trans;
    GLfloat angle[3];

private:
    void inicializaControle();
    void desenhaControle();
    void desenharBezier();
    void caixinha(float x,float y,float z,float largura);
    void cacheComb();


    float colorR;
    float colorG;
    float colorB;
    int nptscontrole;
    unsigned long long c[100][100];


    Ponto controle[N][M];
    double resultado;
    int contI;
    int contJ;



};




#endif
