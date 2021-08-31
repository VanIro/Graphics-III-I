#pragma once

#include "Coord.h"

//Required matrix transformations
vect4 mulProj(mat4f, vect4& p);

vect4 mul(float matrix[4][4], vect4& p);

vect4 mul(mat4f matrix, vect4& p);

vect4 mul(float matrix[][4], vect4 p);

mat4f mul(mat4f a, mat4f b);

void mul(float a[4][4], float b[4][4], float c[4][4]);

mat4f perspectiveMatrix(vect4 eye);

mat4f Perspective(float fov, float aspect);

mat4f rotateMatrix(float yaw, float pitch = 0, float roll = 0);

mat4f orthoprojectMatrix();

std::vector<float> interpolate(float i0, float d0, float il, float dl);

vect4 getnormal(vect4 point1, vect4 point2, vect4 point3);

static mat4f RotView(vect4 right, vect4 up, vect4 dir);

static mat4f TranslateView(vect4 pos);

mat4f Matrix_PointAt(vect4& pos, vect4& target, vect4& up);

mat4f Matrix_QuickInverse(mat4f& m);


mat4f Matrix_MakeRotationX(float fAngleRad);
mat4f Matrix_MakeRotationY(float fAngleRad);

mat4f Matrix_MakeRotationZ(float fAngleRad);

static mat4f Translation(float tx, float ty, float tz);
