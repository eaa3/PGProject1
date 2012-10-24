#include "shapes.h"


Cube::Cube(GLfloat edge)
{
    this->edge = edge;
    pos[0] = pos[1] = pos[2] = 0.0f;
    angle[0] = angle[1] = angle[2] = 0.0f;

    //Front-face
    this->color[0] = {1.0f,0.0f,0.0f};
    //Back-face
    this->color[1] = {1.0f,0.0f,0.0f};
    //Right-face
    this->color[2] = {0.0f,1.0f,0.0f};
    //Left-face
    this->color[3] = {0.0f,1.0f,0.0f};
    //Up-face
    this->color[4] = {0.5f,0.5f,0.5f};
    //Down-face
    this->color[5] = {0.0f,0.0f,1.0f};
}

GLfloat& Cube::operator[](int i)
{
    return this->pos[i];
}

void Cube::draw()
{
    //glPushMatrix();
    //glLoadIdentity();

    glTranslatef(this->pos[0],this->pos[1], this->pos[2]);

    glRotatef(this->angle[0], 1.0f, 0.0f, 0.0f);
    glRotatef(this->angle[1], 0.0f, 1.0f, 0.0f);
    glRotatef(this->angle[2], 0.0f, 0.0f, 1.0f);




    glBegin(GL_QUADS);
    //Front-face
    glColor3fv(this->color[0]);

    glVertex3f(-edge/2, edge/2, 0);
    glVertex3f(edge/2, edge/2, 0);
    glVertex3f(edge/2, -edge/2, 0);
    glVertex3f(-edge/2, -edge/2, 0);

    //Back-face
    glColor3fv(this->color[1]);
    glVertex3f(-edge/2, edge/2, -edge);
    glVertex3f(edge/2, edge/2, -edge);
    glVertex3f(edge/2, -edge/2, -edge);
    glVertex3f(-edge/2, -edge/2, -edge);

    //Right-face
    glColor3fv(this->color[2]);
    glVertex3f(edge/2, edge/2, 0);
    glVertex3f(edge/2, edge/2, -edge);
    glVertex3f(edge/2, -edge/2, -edge);
    glVertex3f(edge/2, -edge/2, 0);

    //Right-face
    glColor3fv(this->color[3]);
    glVertex3f(-edge/2, edge/2, 0);
    glVertex3f(-edge/2, edge/2, -edge);
    glVertex3f(-edge/2, -edge/2, -edge);
    glVertex3f(-edge/2, -edge/2, 0);

    //Up-face
    glColor3fv(this->color[4]);
    glVertex3f(-edge/2, edge/2, 0);
    glVertex3f(-edge/2, edge/2, -edge);
    glVertex3f(edge/2, edge/2, -edge);
    glVertex3f(edge/2, edge/2, 0);

    //Down-face
    glColor3fv(this->color[5]);
    glVertex3f(-edge/2, -edge/2, 0);
    glVertex3f(-edge/2, -edge/2, -edge);
    glVertex3f(edge/2, -edge/2, -edge);
    glVertex3f(edge/2, -edge/2, 0);

    glEnd();

    // glPopMatrix();




}

Color3f::Color3f()
{
    this->color[0] = 0.0f;
    this->color[1] = 0.0f;
    this->color[2] = 0.0f;
}

Color3f::Color3f(GLfloat r, GLfloat g, GLfloat b)
{
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
}

Color3f::operator GLfloat*()
{
    return this->color;
}

Grid::Grid(int w, int h,Vector3 pos, Color3f color)
{
    this->w = w;
    this->h = h;
    this->color = color;
    this->pos = pos;
}

void Grid::draw()
{
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);

    //render grid
    glLineWidth(1);
    glColor3fv(this->color);
    glBegin(GL_LINES);
    for(int i = -w; i <= w; ++i)
    {
        glVertex3f(i,0,-h);
        glVertex3f(i,0,h);

        glVertex3f(h,0,i);
        glVertex3f(-h,0,i);
    }
    glEnd();

    glPopMatrix();
}

void Grid::draw(GLuint texture)
{

    glPushMatrix();

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    // Background Image
    glBindTexture(GL_TEXTURE_2D, texture);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);

    glPushMatrix();

    glLoadIdentity();
    glOrtho(0,1,1,0,-1,1);

    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0,0);

    glTexCoord2f(1,0);
    glVertex2f(1,0);

    glTexCoord2f(1,1);
    glVertex2f(1,1);

    glTexCoord2f(0,1);
    glVertex2f(0,1);
    glEnd();
    // End Background Image


    glDisable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();

    /*
    glEnable(GL_TEXTURE_2D );
    glPushMatrix();
    glTranslatef(-w,h,0);
    glRotatef(90, 0,1,0);
    glRotatef(90, 1,0,0);
    glRotatef(5, 0, 0 , 1);


    glTranslatef(pos[0], pos[1]+5, pos[2]);


    //render grid
    glScalef(-1,1,1);

    glColor3f(1.0f,1.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2i(0,0);
    glVertex3f(-w,0,-h);

    glTexCoord2i(1,0);
    glVertex3f(w, 0, -h);

    glTexCoord2i(1,1);
    glVertex3f(w,0,h);

    glTexCoord2i(0,1);
    glVertex3f(-w,0,h);



    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D );
    */

}



BezierSurface::BezierSurface(float colorR, float colorG, float colorB, int nptscontrole, Vector3 trans)
{
    this->colorR = colorR;
    this->colorG = colorG;
    this->colorB = colorB;

    this->nptscontrole = nptscontrole;

    contI=0;
    contJ=0;

    this->trans = Vector3::ZERO;

    inicializaControle();
    cacheComb();

    this->angle[0] = this->angle[1] = this->angle[2] = 0;
}

void BezierSurface::desenharBezier()
{
    GLfloat tempu;
    GLfloat tempv;
    GLfloat coord_x=0;
    GLfloat coord_y=0;
    GLfloat coord_z=0;
    GLfloat anterior_x=0;
    GLfloat anterior_y=0;
    GLfloat anterior_z=0;
    bool primeiro=false;
    bool primeiroDeCada=false;
    desenhaControle();

    glColor3f(0.1f,1.0f,0.1f);
    glPointSize(0.5);
    int resU=30;
    int resV=30;
    float incU = 1.0/resU, incV=1.0/resV;
    Ponto galera[200];

    for(float u=0; u<=1; u+=incU)
    {
        int aa=0;
        for(float v=0; v<=1; v+=incV, aa++)
        {
            coord_x=0;
            coord_y=0;
            coord_z=0;
            for(int i = 0; i<N; i++)
            {
                tempu= (GLfloat) (c[N][i+1]*pow((1-u),(N-1)-i)*pow(u,i));
                for(int j = 0; j<M; j++)
                {
                    tempv= (GLfloat) (c[M][j+1]*pow((1-v),(M-1)-j)*pow(v,j));
                    coord_x+=tempu*tempv*controle[i][j].x;
                    coord_y+=tempu*tempv*controle[i][j].y;
                    coord_z+=tempu*tempv*controle[i][j].z;

                }
            }
            glBegin(GL_LINES);
            if(primeiro)
            {
                glVertex3f(anterior_x,anterior_y,anterior_z);
                glVertex3f(coord_x,coord_y,coord_z);


            }
            if(primeiroDeCada)
            {
                glVertex3f(galera[aa].x,galera[aa].y,galera[aa].z);
                glVertex3f(coord_x,coord_y,coord_z);
            }
            glEnd();



            anterior_x=coord_x;
            anterior_y=coord_y;
            anterior_z=coord_z;
            galera[aa].x=coord_x;
            galera[aa].y=coord_y;
            galera[aa].z=coord_z;

            primeiro=true;
            glBegin(GL_POINTS);
            glVertex3f(coord_x, coord_y, coord_z);
            glEnd();


        }

        primeiro=false;
        primeiroDeCada=true;
    }




}

void BezierSurface::desenhaControle()
{
    float k=3;
    for(int i = 0; i<N; i++)
    {
        for(int j = 0; j<M; j++)
        {
            if(i==contI&&j==contJ)glColor3f(1.0f,0.0f,0.0f);	// Color Blue
            else glColor3f(0.0f,0.0f,1.0f);	// Color Blue
            caixinha(controle[i][j].x, controle[i][j].y, controle[i][j].z,0.1);
        }
    }
}

void BezierSurface::inicializaControle()
{

    int cont1=0;
    int cont2=0;

    float k=3;
    for(int i = 0; i<N; i++)
    {
        for(int j = 0; j<M; j++)
        {
            controle[i][j].x=i*k;
            controle[i][j].y=6.0;
            controle[i][j].z=j*k;
        }
    }

}

void BezierSurface::caixinha(float x,float y,float z,float largura)
{

    //glColor3f(1.0f,0.5f,0.0f);	// Color Orange
    //glColor3f(0.0f,1.0f,0.0f);	// Color Blue
    //glColor3f(1.0f,0.0f,0.0f);	// Color Red
    //glColor3f(1.0f,1.0f,0.0f);	// Color Yellow
    //glColor3f(1.0f,0.0f,1.0f);	// Color Violet

    glBegin(GL_QUADS);		// Draw The Cube Using quads

    glVertex3f( x+largura, y+largura,z-largura);	// Top Right Of The Quad (Top)
    glVertex3f( x-largura, y+largura,z-largura);	// Top Left Of The Quad (Top)
    glVertex3f(x-largura,  y+largura, z+largura);	// Bottom Left Of The Quad (Top)
    glVertex3f( x+largura, y+largura,z+largura);	// Bottom Right Of The Quad (Top)
    // glColor3f(colorR,colorG,colorB);	// Color Blue
    glVertex3f( x+largura, y-largura,z+largura);	// Top Right Of The Quad (Bottom)
    glVertex3f(x-largura,  y-largura, z+largura);	// Top Left Of The Quad (Bottom)
    glVertex3f(x-largura,  y-largura, z-largura);	// Bottom Left Of The Quad (Bottom)
    glVertex3f( x+largura, y-largura,z-largura);	// Bottom Right Of The Quad (Bottom)
    // glColor3f(colorR,colorG,colorB);	// Color Blue
    glVertex3f( x+largura, y+largura,z+largura);	// Top Right Of The Quad (Front)
    glVertex3f( x-largura, y+largura,z+largura);	// Top Left Of The Quad (Front)
    glVertex3f( x-largura, y-largura,z+largura);	// Bottom Left Of The Quad (Front)
    glVertex3f( x+largura, y-largura,z+largura);	// Bottom Right Of The Quad (Front)
    // glColor3f(colorR,colorG,colorB);	// Color Blue
    glVertex3f( x+largura, y-largura,z-largura);	// Top Right Of The Quad (Back)
    glVertex3f( x-largura, y-largura,z-largura);	// Top Left Of The Quad (Back)
    glVertex3f( x-largura, y+largura,z-largura);	// Bottom Left Of The Quad (Back)
    glVertex3f( x+largura, y+largura,z-largura);	// Bottom Right Of The Quad (Back)
    // glColor3f(colorR,colorG,colorB);	// Color Blue
    glVertex3f( x-largura, y+largura,z+largura);	// Top Right Of The Quad (Left)
    glVertex3f( x-largura, y+largura,z-largura);	// Top Left Of The Quad (Left)
    glVertex3f( x-largura, y-largura,z-largura);	// Bottom Left Of The Quad (Left)
    glVertex3f( x-largura, y-largura,z+largura);	// Bottom Right Of The Quad (Left)
    //  glColor3f(colorR,colorG,colorB);	// Color Blue
    glVertex3f( x+largura, y+largura,z-largura);	// Top Right Of The Quad (Right)
    glVertex3f( x+largura, y+largura,z+largura);	// Top Left Of The Quad (Right)
    glVertex3f( x+largura, y-largura,z+largura);	// Bottom Left Of The Quad (Right)
    glVertex3f( x+largura, y-largura,z-largura);	// Bottom Right Of The Quad (Right)
    glEnd();			// End Drawing The Cube



}

void BezierSurface::cacheComb()
{
    c[2][1]=c[2][2]=c[1][1]=1;
    c[2][0]=0;
    for(int i=3; i<=nptscontrole; ++i)
    {
        c[i][0]=0;
        for(int j=1; j<=i; ++j)
        {
            c[i][j]=c[i-1][j]+c[i-1][j-1];
        }
    }
}

void BezierSurface::draw()
{
    Vector3 v = Vector3(((N-1)*3)/2, 6.0f, ((M-1)*3)/2);

    glPushMatrix();

    glTranslatef(trans[0], trans[1], trans[2]);
    glRotatef(angle[0], 1,0,0);
    glRotatef(angle[1], 0,1,0);

    glTranslatef(-v[0], -v[1], -v[2]);

    glNormal3f(0.0f,1.0f,0.0f);
    this->desenharBezier();

    glPopMatrix();
}

void BezierSurface::selectNextCtrlPoint()
{
    contJ++;
    if(contJ==M)
    {
        contJ=0;
        contI++;
    }
    if(contI==N)contI=0;
}

void BezierSurface::checkEvent(int type)
{
    switch(type)
    {
    case SDLK_KP3:
        controle[contI][contJ].x++;
        break;
    case SDLK_KP1:
        controle[contI][contJ].x--;
        break;
    case SDLK_KP2:
        controle[contI][contJ].z--;
        break;
    case SDLK_KP5:
        controle[contI][contJ].z++;
        break;
    case SDLK_KP_MINUS:
        controle[contI][contJ].y--;
        break;
    case SDLK_KP_PLUS:
        controle[contI][contJ].y++;
        break;
    case SDLK_LCTRL:
        selectNextCtrlPoint();
        break;
    case SDLK_0:
        angle[0]++;
        break;
    case SDLK_1:
        angle[0]--;
        break;
    case SDLK_2:
        angle[1]++;
        break;
    case SDLK_3:
        angle[1]--;
        break;
    case SDLK_4:
        trans += Vector3(1,0,0);
        break;
    case SDLK_5:
        trans -= Vector3(1,0,0);
        break;
    case SDLK_6:
        trans += Vector3(0,1,0);
        break;
    case SDLK_7:
        trans -= Vector3(0,1,0);
        break;
    case SDLK_8:
        trans += Vector3(0,0,1);
        break;
    case SDLK_9:
        trans -= Vector3(0,0,1);
        break;


    default:
        break;

    }
}

