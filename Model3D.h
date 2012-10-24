#ifndef MODEL3D_H
#define MODEL3D_H

#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include "Vector3.h"

using namespace std;

struct Face
{
    vector< int > v;
    vector< int > vn;
};

class Model3D
{
public:
    Model3D();
    Model3D(string filename);
    virtual ~Model3D();

    vector< Vector3 > vertex;
    vector< Vector3 > normal;

    vector< Face > face;

    GLfloat angle[3];
    GLfloat scaleFactor;

    Vector3 translation;

    void load(string filename);
};

#endif // MODEL3D_H
