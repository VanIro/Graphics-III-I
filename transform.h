#pragma once

#include "Coord.h"


#include"vect.h"

void copyMatrix(float to[][4], float from[][4]);
void unitMatrix(float matrix[][4]);
void composeTransform(float transformMat2[][4], float transformMat[][4]);
void ortho_projMatrix(float matrix[][4], float Zvp);
void perspective_projMatrix(float matrix[][4], vect4 prp, float Zvp);
void scaleXYZMatrix(float matrix[][4], float sx, float sy, float sz);
void translateMatrix(float matrix[][4], float x, float y, float z);
void rotateMatrixXYZ(float matrix[][4], float deg_angle, char XYZ);
void rotateMatrixAxis(float matrix[][4], vect4 axis, float deg_angle, float Imatrix[][4]=nullptr);
void shearingMatrixXYZ(float matrix[][4], float par1, float par2, char axisXYZ, float ref = 0);
void reflectionMatrixXYZ(float matrix[][4], char XYZ);
void viewMatrix(float viewMat[][4], vect4 from, vect4 to, vect4 up, float IviewMat[][4]=nullptr);
void display_transform(float mat[][4]);

//Required transformations
extern const int mat_sz;

void transate_polygon(vect4& pt, vect4 translate);
void scale_polygon(vect4& pt, float scale, char XYZ);
void scale_polygon(vect4& pt, float scale);
void rotateX(vect4& pt, float angle);
void rotateY(vect4& pt, float angle);
void rotateZ(vect4& pt, float angle);
void matrix_product(vect4& p, float matrix[4][4]);//[mat_sz][mat_sz]);
void rotateY2(vect4& pt, float angle, float yt);
