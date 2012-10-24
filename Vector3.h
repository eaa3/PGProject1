#ifndef VECTOR3_H
#define VECTOR3_H

#include <gl/gl.h>
#include <cmath>

GLfloat toRad(GLfloat angle);

class Vector3
{
public:

    static const Vector3 ZERO;

    static const int R_X;
    static const int R_Y;
    static const int R_Z;

    Vector3(GLfloat x, GLfloat y, GLfloat z);
    //Vector3(Vector3& other);
    Vector3();
    virtual ~Vector3();

    operator GLfloat*();

    GLfloat& operator[](int index);

    void operator=(Vector3 other);
    GLfloat operator*(Vector3 other);
    Vector3 operator*(GLfloat k);
    Vector3 operator/(GLfloat k);
    Vector3 operator+(Vector3 other);
    Vector3 operator-(Vector3 other);
    void operator*=(GLfloat k);
    void operator/=(GLfloat k) ;
    void operator+=(Vector3 other);
    void operator-=(Vector3 other);




    GLfloat norm();

    void normalize();

    GLfloat angleBetween(Vector3 other) ;

    Vector3 cross(Vector3 other);

    void rotate(GLfloat angle, int type);
    void rotate(GLfloat angle, Vector3 u);


    void transform(Vector3 vi, Vector3 vj, Vector3 vk);

    Vector3 proj(Vector3 other);



protected:
private:

    GLfloat coord[3];


};

#endif // VECTOR3_H
