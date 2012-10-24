#ifndef __UTIL__
#define __UTIL__

#include "includes.h"
#include "instricCalc.h"

#define COLOR_RANGE 256

//Constants
const int WIDTH = 800;
const int HEIGHT = 600;
const int DEPTH_Z = 500;
const int FRAME_RATE = 30;
const int SECOND = 1000;
const GLfloat angle = 1;

const int fontSize = 32;

void initSDL(); //Initializes SDL
void configSDLGL(int w, int h); //Configures SDL & OPENGL contexts
void deinit();


void draw();
void frameControl();
bool handle_keyboard_event(SDL_Event &event);
void handle_mouse_event(SDL_Event &event);
void handle_resize_event(SDL_Event &event);

void loadRodriguezMatrix(GLfloat* matrix, Vector3 u, GLfloat angle);
void loadTrans(GLfloat * matrix, Vector3 t);
void copyMatrix(GLfloat* dst, GLfloat* src);
void multMatrix(GLfloat* result, GLfloat* mA, GLfloat* mB);
void loadIdentity(GLfloat * matrix);
void transpose(GLfloat * matrix);

void calcExtrinsicParams(GLfloat* extrinsic);



/*
SDL_Surface* renderText(string text);
*/
GLuint loadTexture(SDL_Surface* sfc);

//GLuint loadTexture(string filename);




#endif
