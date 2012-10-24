#include "util.h"


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
Vector3 ri = Ri,rj = Rj,rk = Rk;
Vector3 ri2 = Ri, rj2 = Rj, rk2 = Rk;

GLfloat cangle[3] = {0,0,0};

Cube c(20);
Grid grid(100,100,Vector3(0,-10,0), Color3f(0.9f,0.9f,0.9f));

ObjModel* model = NULL;

void initSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    TTF_Init();

    //font = TTF_OpenFont(fontFilename, fontSize);

    t0 = t = SDL_GetTicks();
    frame = 0;

    SDL_EnableKeyRepeat(200, 25);

    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"),NULL);


    //freopen("CON","w",stdout);
}

void configSDLGL(int w, int h)
{
    const SDL_VideoInfo *info = SDL_GetVideoInfo();

    int bpp = info->vfmt->BitsPerPixel;
    Uint32 flags = SDL_OPENGL|SDL_RESIZABLE;

    //SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


    SDL_SetVideoMode(w,h,bpp,flags);

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(0, (GLfloat) w, (GLfloat) h, 0, 0, 800 );
    gluPerspective(45.0, ((double)w) / (double)h, 0.1,  DEPTH_Z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glEnable(GL_DEPTH_TEST);


    //
    //  glEnable(GL_COLOR_MATERIAL);
    //  glDisable(GL_COLOR_MATERIAL);
    //  glEnable(GL_FLAT);

    //For rendering text
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


    glEnable(GL_NORMALIZE);
    //glEnable(GL_LINE_SMOOTH);





}

void deinit()
{
    //for(int i = 0; i < texture.size(); i++) glDeleteTextures(1, &texture[i]);

    TTF_Quit();
    SDL_Quit();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Ri.normalize();
    //Rj.normalize();
    //Rk.normalize();
    /*
    ri = Ri;
    rj = Rj;
    rk = Rk;

    rk.rotate(cangle[1], Rj );
    ri.rotate(cangle[1], Rj );

    rj = rk.cross(ri);

    rj.rotate(cangle[0], Ri);
    rk.rotate(cangle[0], Ri);

    ri = rk.cross(rj);
    */
    //ri = rj.cross(rk);
    //rj = rk

    Vector3 T = C;

    T.transform(ri, rj, rk);

    GLfloat m[] = { ri[0], ri[1], ri[2], 0,
                    rj[0], rj[1], rj[2], 0,
                    -rk[0], -rk[1], -rk[2], 0,
                    T[0], T[1], T[2], 1
                  };

    GLfloat m2[] = { 1.0f , 0.0f                 , 0.0f                     ,0.0f,
                     0.0f , cosf(toRad(cangle[0])),  -sinf(toRad(cangle[0])) ,0.0f,
                     0.0f , sinf(toRad(cangle[0])),  cosf(toRad(cangle[0])) ,0.0f,
                     0.0f , 0.0f                  ,  0.0f                   ,1.0f
                   };
    glMultMatrixf(m2);
    glMultMatrixf(m);

    /*
    GLfloat m[] = { 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, -1, 0,
                    0, 0, 0, 1
                  };
                  */

    //glMultMatrixf(m);

    //glRotatef(cangle[0], 1,0,0);
    //glRotatef(cangle[1], 0,1,0);
    //glRotatef(cangle[2], 0,0,1);


    //glTranslatef(-C[0],-C[1],-C[2]);

    c.draw();

    grid.draw();

    glPushMatrix();

    glTranslatef(10 ,c.edge/2 + 10, -c.edge/2);
    glLineWidth(4);

    glBegin(GL_LINES);
    Vector3 v = Vector3::ZERO;
    glColor3f(1.0f,0.0f,0.0f);
    glVertex3fv(v);
    glVertex3fv(ri*10);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3fv(v);
    glVertex3fv(rj*10);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3fv(v);
    glVertex3fv(rk*10);

    glEnd();

    glTranslatef(-10, -0 , c.edge/2);
    //render world axis
    glLineWidth(3);
    glColor3f(1, 0, 0); //RED = x
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(5,0,0);
    glEnd();
    glColor3f(0, 1, 0); //GREEN = y
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,5,0);
    glEnd();
    glColor3f(0, 0, 1); //BLUE = z
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,5);
    glEnd();

    glPopMatrix();

    drawModel(model);

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
        ss << " FPS(" << frame_prev << ")" << "Cube(" << c[0] << ", " << c[1] << "," << c[2] << ")" << " C(" << C[0] << ", " << C[1] << "," << C[2] << ") " << " Ri(" << Ri[0] << ", " << Ri[1] << "," << Ri[2] << ") " << " Rj(" << Rj[0] << ", " << Rj[1] << "," << Rj[2] << ")" << " Rk(" << Rk[0] << ", " << Rk[1] << "," << Rk[2] << ")" << " T(" << C[0] << ", " << C[1] << "," << C[2] << ")" ;
        SDL_WM_SetCaption(ss.str().c_str(),"OPENGL+SDL");
        ss.str("");
    }


}

bool handle_keyboard_event(SDL_Event &event)
{

    if( event.type == SDL_QUIT ) return true;

    else if( event.type == SDL_KEYDOWN )
    {
        Vector3 right(ri2[0], rj2[0], rk2[0]);
        Vector3 up(ri2[1],rj2[1],rk2[1]);
        Vector3 foward(ri2[2],rj2[2],rk2[2]);

        switch( event.key.keysym.sym )
        {
        case SDLK_ESCAPE:
            return true;
            break;
        case SDLK_a:

            //C -= Ri;

            C += right;
            //C -= ri;

            break;
        case SDLK_d:


            C -= right;
            //C += Ri;
            //C += ri;

            break;
        case SDLK_s:

            C -= foward;
            //C -= Rk;
            //C -= rk;

            break;
        case SDLK_w:
            C += foward;
            //C += Rk;
            //C += rk;

            break;
        case SDLK_PAGEUP:
            C -= up;
            //C += Rj;
            //C -= rj;

            break;
        case SDLK_PAGEDOWN:
            C += up;
            //C -= Rj;
            //C += rj;

            break;
        case SDLK_RIGHT:
            cangle[1]+=angle;


            rk.rotate(angle, rj );
            ri.rotate(angle, rj );

            rk2.rotate(angle, rj2 );
            ri2.rotate(angle, rj2 );


            //rj = rk.cross(ri);

            /*OLD FORBIDEN*/
            //cangle[1]--;

            //Ri.rotate(angle, Rj);
            //Rk.rotate(angle, Rj);
            //c.angle[1]++;
            //if( c.angle[1] > 360 ) c.angle[1] = 0;


            break;
        case SDLK_LEFT:

            cangle[1]-=angle;

            rk.rotate(-angle, rj );
            ri.rotate(-angle, rj );

            rk2.rotate(-angle, rj2 );
            ri2.rotate(-angle, rj2 );

            //rj = rk.cross(ri);

            //cangle[1]++;

            //Ri.rotate(-angle, Rj);
            //Rk.rotate(-angle, Rj);

            //c.angle[1]--;
            //if( c.angle[1] < -360) c.angle[1] = 0;


            break;
        case SDLK_UP:

            cangle[0] +=angle;

            rj2.rotate(-angle, ri2);
            rk2.rotate(-angle, ri2);
            //  rj.rotate(-angle, ri);
            // rk.rotate(-angle, ri);

            //ri = rk.cross(rj);
            //cangle[0]++;

            //Rj.rotate(-angle, Ri);
            //Rk.rotate(-angle, Ri);


            //c.angle[0]++;
            //if( c.angle[0] > 360 ) c.angle[0] = 0;


            break;
        case SDLK_DOWN:
            cangle[0]-=angle;

            rj2.rotate(angle, ri2);
            rk2.rotate(angle, ri2);
            //    rj.rotate(angle, ri);
            //   rk.rotate(angle, ri);

            //ri = rk.cross(rj);
            //cangle[0]--;

            //Rj.rotate(angle, Ri);
            //Rk.rotate(angle, Ri);

            //c.angle[0]--;
            //if( c.angle[0] < -360) c.angle[0] = 0;

            break;
        case SDLK_i:

            cangle[2]++;

            //Ri.rotate(-angle, Rk);
            //Rj.rotate(-angle, Rk);

            break;
        case SDLK_o:

            cangle[2]--;

            //Ri.rotate(angle, Rk);
            //Rj.rotate(angle, Rk);

            break;
        case SDLK_v:
            c.angle[0]--;
            if( c.angle[0] < -360) c.angle[0] = 0;
            break;
        case SDLK_b:
            c.angle[0]++;
            if( c.angle[0] > 360) c.angle[0] = 0;
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

void drawModel(ObjModel * model)
{/*
    if ( model != NULL)
    {
        glBegin(GL_TRIANGLES);
        glColor3f(0,0,1);

        GLfloat x,y,z;
        GLfloat nx, ny, nz;

        for(int i = 0; i < model->nTriangle; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                nx = model->NormalArray[model->TriangleArray[i].Normal[j]].x;
                ny = model->NormalArray[model->TriangleArray[i].Normal[j]].y;
                nz = model->NormalArray[model->TriangleArray[i].Normal[j]].z;

                glNormal3f(nx,ny,nz);


                x =  model->VertexArray[model->TriangleArray[i].Vertex[j]].x;
                y =  model->VertexArray[model->TriangleArray[i].Vertex[j]].y;
                z =  model->VertexArray[model->TriangleArray[i].Vertex[j]].z;

                glVertex3f(x,y,z);
            }
        }

        glEnd();
    }
    */

}


