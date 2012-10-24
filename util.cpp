#include "util.h"
#include "Model3D.h"

SDL_Surface *screen = NULL;
//TTF_Font* font = NULL;
//const char* fontFilename = "supermario.ttf";

//Variables for frame control
long long t0, t;
int frame, frame_prev;

stringstream ss;


vector<GLuint> texture;

Vector3 Ri(1,0,0);
Vector3 Rj(0,1,0);
Vector3 Rk(0,0,1);
Vector3 C(0,10,-100);

GLfloat cangle[3] = {0,0,0};

Cube c(20);
Grid grid(100,100,Vector3(0,-10,0), Color3f(0.7f,1.0f,0.7f));
Model3D model;

GLfloat ambientColor[] = {0.3f, 0.4f, 0.3f, 1.0f};

//Positioned light
GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f};

//Directed diffuse light
GLfloat lightColor1[] = {5.0f, 2.0f, 2.0f, 1.0f};
GLfloat lightPos1[] = {1.0f, 2.0f, 1.0f, 0.0f};

//Directed specular light
GLfloat lightColor2[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPos2[] = {C[0], C[1], C[2], 0.0f};
GLfloat specularity[4]= {0.9f,0.9f,0.9f,1.0};
GLint specMaterial = 40;


string modelFile[8];
int currentModel = 0;
GLfloat extrinsic[16];

BezierSurface bezierSfc;

GLuint tex;

GLuint loadTexture(SDL_Surface* sfc)
{
    GLuint texture;			// This is a handle to our texture object
    GLenum texture_format;
    GLint  nOfColors;
    SDL_Surface* surface = sfc;

    if ( surface )
    {

        // Check that the image's width is a power of 2
        if ( (surface->w & (surface->w - 1)) != 0 )
        {
            printf("warning: image.bmp's width is not a power of 2\n");
        }

        // Also check if the height is a power of 2
        if ( (surface->h & (surface->h - 1)) != 0 )
        {
            printf("warning: image.bmp's height is not a power of 2\n");
        }

        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            else
                texture_format = GL_BGRA;
        }
        else if (nOfColors == 3)       // no alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            else
                texture_format = GL_BGR;
        }
        else
        {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }

        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );

        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    }
    else
    {
        printf("SDL could not load image.bmp: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

// Free the SDL_Surface only if it was successfully created
    if ( surface )
    {
        SDL_FreeSurface( surface );
    }

    return texture;
}

void initSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    //TTF_Init();

    //font = TTF_OpenFont(fontFilename, fontSize);

    t0 = t = SDL_GetTicks();
    frame = 0;

    SDL_EnableKeyRepeat(200, 25);

    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"),NULL);

    //freopen("CON","w", stdout);
}

void configSDLGL(int w, int h)
{
    const SDL_VideoInfo *info = SDL_GetVideoInfo();

    int bpp = info->vfmt->BitsPerPixel;
    Uint32 flags = SDL_OPENGL|SDL_RESIZABLE;

    //SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    //SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    SDL_SetVideoMode(w,h,bpp,flags);

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(0, (GLfloat) w, (GLfloat) h, 0, -100, 800 );
    gluPerspective(45.0, ((double)w) / (double)h, 0.1,  DEPTH_Z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    //Lighting configs
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);



    //For rendering text
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LINE_SMOOTH);

    ifstream in("loadModel.txt");

    while( in >> modelFile[currentModel] ) currentModel = (currentModel+1)%8;


    model.load(modelFile[currentModel]);
    in.close();

    bezierSfc.trans += Vector3(0,20,0);

    tex = loadTexture(SDL_LoadBMP("cheetos.bmp"));
}

void deinit()
{
    //for(int i = 0; i < texture.size(); i++) glDeleteTextures(1, &texture[i]);

    glDeleteTextures( 1, &tex );
    //TTF_Quit();
    SDL_Quit();
}

void drawCameraAxis()
{
    glPushMatrix();

    glTranslatef(0 ,50, 100);
    //glTranslatef(1, 0, 50);
    //glTranslatef(C[0] + Rj[0] , C[1] + Rj[1], C[2] + Rj[2]);
    //glTranslatef(C[0] + Rk[0]*10 + 1 , C[1] + Rk[1]*10, C[2] + Rk[2]*10);
    glLineWidth(4);

    glBegin(GL_LINES);
    Vector3 v = Vector3::ZERO;
    glColor3f(1.0f,0.0f,0.0f);
    glVertex3fv(v);
    glVertex3fv(Ri*10);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3fv(v);
    glVertex3fv(Rj*10);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3fv(v);
    glVertex3fv(Rk*10);

    glEnd();

    glPopMatrix();
}

void drawModel()
{
    glPushMatrix();




    glTranslatef(model.translation[0], model.translation[1], model.translation[2]);

    glRotatef(model.angle[0], 1,0,0);
    glRotatef(model.angle[1], 0,1,0);
    glRotatef(model.angle[2], 0,0,1);

    glScalef(model.scaleFactor, model.scaleFactor, model.scaleFactor);
    glScalef(1,1,-1);

    /*MEU DESENHO */
    for(int i = 0; i < model.face.size(); i++)
    {
        glBegin(GL_POLYGON);
        glColor3f(1.0f,0.5,0);
        for(int j = 0; j < model.face[i].v.size(); j++ )
        {
            glNormal3fv(model.normal[model.face[i].vn[j]]);
            glVertex3fv(model.vertex[model.face[i].v[j]]);
        }
        glEnd();
    }
    /*Fim desenho*/

    glPopMatrix();

}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    Ri.normalize();
    Rj.normalize();
    Rk.normalize();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    loadIdentity(extrinsic);

    //My way of doing the things
    calcExtrinsicParams(extrinsic);

    glLoadMatrixf(extrinsic);



    /*
    GLfloat m[] = { 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, -1, 0,
                    0, 0, 0, 1
                  };



    glMultMatrixf(m);

    Vector3 u;

    u = Vector3(1,0,0);

    loadRodriguezMatrix(m,u,-cangle[0]);

    glMultMatrixf(m);

    u = Vector3(0,1,0);

    loadRodriguezMatrix(m,u,-cangle[1]);

    glMultMatrixf(m);

    u = Vector3(0,0,1);

    loadRodriguezMatrix(m,u,-cangle[2]);

    glMultMatrixf(m);

    loadTrans(m,C*(-1));

    glMultMatrixf(m);

    glGetFloatv(GL_MODELVIEW_MATRIX,extrinsic);
    */



    //Ajusting light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    glMaterialfv(GL_FRONT,GL_SPECULAR, specularity);

    glMateriali(GL_FRONT,GL_SHININESS,specMaterial);

    glLightfv(GL_LIGHT2, GL_SPECULAR, lightColor2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightPos2);


    //Draw operations
    drawCameraAxis();

    //c.draw();

    grid.draw();

    grid.draw(tex);


    drawModel();

    bezierSfc.draw();


    SDL_GL_SwapBuffers();
    frameControl();


}

void frameControl()
{
    //Frame Control
    long long dt = (SECOND/FRAME_RATE) - (SDL_GetTicks() - t0);
    if( dt > 0 ) SDL_Delay(dt);
    t0 = SDL_GetTicks();

    frame++;

    if( SDL_GetTicks() - t >= SECOND )
    {
        frame_prev = frame;
        frame = 0;

        t = SDL_GetTicks();
        ss.precision(2);
        ss << fixed;
        ss << " FPS(" << frame_prev << ") 04/05/2012 [PGProject by eaa3,grrbm,amng,lfss2] CurrentModel(" << currentModel << ") Ri(" << Ri[0] << ", " << Ri[1] << "," << Ri[2] << ") " << " Rj(" << Rj[0] << ", " << Rj[1] << "," << Rj[2] << ")" << " Rk(" << Rk[0] << ", " << Rk[1] << "," << Rk[2] << ")" << " T(" << C[0] << ", " << C[1] << "," << C[2] << ")" ;
        SDL_WM_SetCaption(ss.str().c_str(),"OPENGL+SDL");
        ss.str("");
    }


}

bool handle_keyboard_event(SDL_Event &event)
{
    Ri[0] = extrinsic[0];
    Ri[1] = extrinsic[1];
    Ri[2] = extrinsic[2];
    Rj[0] = extrinsic[4];
    Rj[1] = extrinsic[5];
    Rj[2] = extrinsic[6];
    Rk[0] = extrinsic[8];
    Rk[1] = extrinsic[9];
    Rk[2] = extrinsic[10];

    Rk = Rk*(-1);

    if( event.type == SDL_QUIT ) return true;

    else if( event.type == SDL_KEYDOWN )
    {
        bezierSfc.checkEvent(event.key.keysym.sym);

        switch( event.key.keysym.sym )
        {
        case SDLK_ESCAPE:
            return true;
            break;
        case SDLK_a:

            C -= Ri;

            break;
        case SDLK_d:

            C += Ri;

            break;
        case SDLK_s:

            C -= Rk;

            break;
        case SDLK_w:

            C += Rk;

            break;
        case SDLK_PAGEUP:

            C += Rj;

            break;
        case SDLK_PAGEDOWN:

            C -= Rj;

            break;
        case SDLK_RIGHT:

            cangle[1]--;

            //Ri.rotate(angle, Rj);
            //Rk.rotate(angle, Rj);
            //Rj = Ri.cross(Rk*(-1));
            //c.angle[1]++;
            //if( c.angle[1] > 360 ) c.angle[1] = 0;


            break;
        case SDLK_LEFT:

            cangle[1]++;

            //Ri.rotate(-angle, Rj);
            //Rk.rotate(-angle, Rj);
            //Rj = Ri.cross(Rk*(-1));

            //c.angle[1]--;
            //if( c.angle[1] < -360) c.angle[1] = 0;


            break;
        case SDLK_UP:

            cangle[0]++;

            //Rj.rotate(-angle, Ri);
            //Rk.rotate(-angle, Ri);
            //Ri = Rj.cross(Rk);

            //c.angle[0]++;
            //if( c.angle[0] > 360 ) c.angle[0] = 0;


            break;

        case SDLK_DOWN:

            cangle[0]--;

            //Rj.rotate(angle, Ri);
            //Rk.rotate(angle, Ri);
            //Ri = Rj.cross(Rk);

            //c.angle[0]--;
            //if( c.angle[0] < -360) c.angle[0] = 0;

            break;
        case SDLK_q:

            cangle[2]--;

            Ri.rotate(-angle, Rk);
            Rj.rotate(-angle, Rk);
            Rk = Ri.cross(Rj);

            break;
        case SDLK_e:

            cangle[2]++;

            Ri.rotate(angle, Rk);
            Rj.rotate(angle, Rk);
            Rk = Ri.cross(Rj);

            break;
        case SDLK_y:
            model.angle[0]++;

            if( model.angle[0] > 360 ) model.angle[0] = 0;
            break;
        case SDLK_h:
            model.angle[0]--;

            if( model.angle[0] < -360 ) model.angle[0] = 0;
            break;
        case SDLK_g:
            model.angle[1]++;

            if( model.angle[1] > 360 ) model.angle[1] = 0;
            break;
        case SDLK_j:
            model.angle[1]--;

            if( model.angle[1] < -360 ) model.angle[1] = 0;
            break;
        case SDLK_t:
            model.angle[2]++;

            if( model.angle[2] > 360 ) model.angle[2] = 0;
            break;


        case SDLK_u:
            model.angle[2]--;

            if( model.angle[2] < -360 ) model.angle[2] = 0;
            break;
        case SDLK_SPACE:

            model.scaleFactor += 0.01;
            break;
        case SDLK_n:

            model.scaleFactor -= 0.01;

            if( model.scaleFactor <= 0 ) model.scaleFactor = 0.01;
            break;
        case SDLK_f:

            model.translation += Vector3(0,0,1);
            break;
        case SDLK_v:

            model.translation -= Vector3(0,0,1);
            break;
        case SDLK_c:

            model.translation -= Vector3(1,0,0);
            break;
        case SDLK_b:
            model.translation += Vector3(1,0,0);
            break;
        case SDLK_x:
            model.translation += Vector3(0,1,0);
            break;
        case SDLK_z:
            model.translation -= Vector3(0,1,0);
            break;
        case SDLK_TAB:
            currentModel = (currentModel+1)%8;
            model.load(modelFile[currentModel]);
            break;
        default:
            break;
        }


    }

    return false;
}

void handle_mouse_event(SDL_Event &event)
{
    if( event.type == SDL_MOUSEMOTION )
    {
        if( event.motion.xrel > 0  && event.motion.state == SDL_BUTTON(1))
        {

        }
        else if( event.motion.xrel < 0 && event.motion.state == SDL_BUTTON(1)  )
        {

        }


        if( event.motion.yrel > 0  && event.motion.state == SDL_BUTTON(1) )
        {

        }
        else if( event.motion.yrel < 0 && event.motion.state == SDL_BUTTON(1)  )
        {

        }
    }
    else if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        if( event.button.button == SDL_BUTTON_LEFT )
        {


        }
        else if( event.button.button == SDL_BUTTON_RIGHT )
        {

        }
    }

}

void handle_resize_event(SDL_Event &event)
{
    if( event.type == SDL_VIDEORESIZE )
    {
        configSDLGL(event.resize.w, event.resize.h);
    }
}

void loadRodriguezMatrix(GLfloat* matrix, Vector3 u, GLfloat angle)
{
    angle = toRad(angle);

    //CosTeta
    GLfloat c = cosf(angle);
    //SinTeta
    GLfloat s = sinf(angle);
    //One Minus cosTeta
    GLfloat omc = 1.0f - c;

    matrix[0] = u[0]*u[0]*omc + c;
    matrix[1] = u[0]*u[1]*omc - u[2]*s;
    matrix[2] = u[0]*u[2]*omc + u[1]*s;
    matrix[3] = 0;

    matrix[4] = u[1]*u[0]*omc + u[2]*s;
    matrix[5] = c + u[1]*u[1]*omc;
    matrix[6] = u[1]*u[2]*omc - u[0]*s;
    matrix[7] = 0;

    matrix[8] = u[2]*u[0]*omc - u[1]*s;
    matrix[9] = u[2]*u[1]*omc + u[0]*s;
    matrix[10] = c + u[2]*u[2]*omc;
    matrix[11] = 0;

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;

}

void loadTrans(GLfloat * matrix, Vector3 t)
{

    for(int i = 0; i <= 15; i++)
    {
        matrix[i] = 0;
    }

    matrix[0] = 1;
    matrix[5] = 1;
    matrix[10] = 1;

    matrix[12] = t[0];
    matrix[13] = t[1];
    matrix[14] = t[2];
    matrix[15] = 1;
}

void loadIdentity(GLfloat * matrix)
{
    for(int i = 0; i <= 15; i++)
    {
        matrix[i] = 0;
    }

    matrix[0] = 1;
    matrix[5] = 1;
    matrix[10] = 1;
    matrix[15] = 1;

}

void multMatrix(GLfloat* result, GLfloat* mA, GLfloat* mB)
{
    GLfloat val = 0.0f;

    for(int i=0; i< 4 ; i++)
    {
        for(int k=0; k<4; k++)
        {
            for(int j=0; j<4 ; j++)
            {
                val += mA[i*4 + j]*mB[j*4 + k];
            }

            result[i*4 + k] = val;
            val = 0.0f;

        }

    }
}

void copyMatrix(GLfloat* dst, GLfloat* src)
{
    for(int i = 0; i < 16; i++) dst[i] = src[i];
}

void transpose(GLfloat * matrix)
{
    GLfloat temp[16];

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            temp[i*4 + j ] = matrix[j*4 + i];
        }
    }

    copyMatrix(matrix, temp);


}

void calcExtrinsicParams(GLfloat* extrinsic)
{
    //Vector around which we gonna eventually rotate
    Vector3 u;

    //Initial basis
    GLfloat m[] = { 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, -1, 0,
                    0, 0, 0, 1
                  };

    //Matriz used as temporary storage place to the operation results...
    GLfloat result[16];
    memset(result,0,16*sizeof(GLfloat));

    //Multiply extrinsic by m and stores the result
    multMatrix(result, extrinsic, m);

    //Copies the result to exstrinsic
    copyMatrix(extrinsic, result); //Now our base is m
    memset(result,0,16*sizeof(GLfloat));

    //Now we gonna evaluate the camera coordenate system, which are some rotation around the canonic base

    //Rotation around x-axis( How much is the camera inclined related to x-axis?)
    u = Vector3(1,0,0);

    loadRodriguezMatrix(m,u,cangle[0]);

    multMatrix(result, extrinsic, m);//Multiply and store the result
    copyMatrix(extrinsic, result);//Copies the result to extrinsic matrix
    memset(result,0,16*sizeof(GLfloat));

    //Rotation around y-axis( How much is the camera inclined related to y-axis?)
    u = Vector3(0,1,0);

    loadRodriguezMatrix(m,u,cangle[1]);

    multMatrix(result, extrinsic, m);
    copyMatrix(extrinsic, result);
    memset(result,0,16*sizeof(GLfloat));

    //Rotation around z-axis( How much is the camera inclined related to z-axis?)
    u = Vector3(0,0,1);

    loadRodriguezMatrix(m,u,cangle[2]);

    multMatrix(result, extrinsic, m);
    copyMatrix(extrinsic, result);
    memset(result,0,16*sizeof(GLfloat));

    //Besides the rotation, the camera is situated in some place of the world different from the origin, probabbly.
    //Therefore, to finally complete our mission to obtain the exstrinsic parameters of the camera we have to translate the origin of the world
    //So that from the point of view of the camera the origin of the world become it's own position in the world
    //That's because of the following notion, if we have a point in the world, say x, and you want to discover the coordinates
    //of this point transformed in coordinates of the camera we shall compute it as follow: xcam = R(x - c), where c is the
    //position of the camera inside the world coordinates. Note that x - c is a translation still in the coordinates of the world,
    //in homogeneous coordinates that translation would be T.

    loadTrans(m,C*(-1));

    transpose(m);

    multMatrix(result, extrinsic, m);
    copyMatrix(extrinsic, result);
    memset(result,0,16*sizeof(GLfloat));


    transpose(extrinsic);//Because opengl works only with transposed matrices...

    /*
    What we just did was the following: we computed the rotations along the three axis Rx, Ry, Rz and composed them in just one
    rotation R = Rx*Ry*Rz, in that order. Also, we computed the translation matrix, that's the translation I talked about before, then composed it
    with our previous transformation R, so that extrinsic = R*T. Summaring, we obtained the extrinsic matrix of our camera, which is a composition
    of translation and then a rotation.*/

}

