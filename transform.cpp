#include"transform.h"

void matrix_product(vect4& p, float matrix[4][4])
{
    vect4 temp;
    temp.x = matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z + matrix[0][3];
    temp.y = matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z + matrix[1][3];
    temp.z = matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z + matrix[2][3];
    p = std::move(temp);
}

void transate_polygon(vect4& pt, vect4 translate)
{
    pt = pt + translate;
}

void scale_polygon(vect4& pt, float scale)
{

    pt = pt.scaleProduct(scale);
}

void scale_polygon(vect4& pt, float scale, char XYZ)
{
    if (XYZ == 'X')    pt = pt.scaleProduct(scale, 1, 1);
    else if (XYZ == 'Y')    pt = pt.scaleProduct(1, scale, 1);
    else if (XYZ == 'Z')    pt = pt.scaleProduct(1, 1, scale);
}

void rotateX(vect4& pt, float angle)
{
    angle = deg_to_rad(angle);
    float composite[][4] = { {1, 0, 0, 0},
                             {0, cos(angle), -sin(angle), 0},
                             {0, sin(angle), cos(angle), 0},
                             {0, 0, 0, 1} };
    matrix_product(pt, composite);
}

void rotateY(vect4& pt, float angle)
{
    angle = deg_to_rad(angle);
    float composite[4][4] = { {cos(angle), 0, sin(angle), 0},
                             {0, 1, 0, 0},
                             {-sin(angle),0 , cos(angle), 0},
                             {0, 0, 0, 1} };
    matrix_product(pt, composite);
}

void rotateZ(vect4& pt, float angle)
{
    angle = deg_to_rad(angle);
    float composite[4][4] = { {cos(angle), -sin(angle), 0, 0},
                             {sin(angle), cos(angle), 0, 0},
                             {0, 0, 1, 0},
                             {0, 0, 0, 1} };
    matrix_product(pt, composite);
}


void rotateY2(vect4& pt, float angle, float yt)
{
    //angle = deg_to_rad(angle);
    float composite2[4][4];

    float composite[4][4];
    translateMatrix(composite, 0, -yt, 0);
    rotateMatrixXYZ(composite2, angle, 'Y');
    composeTransform(composite2, composite);
    translateMatrix(composite2, 0, yt, 0);
    composeTransform(composite2, composite);

    matrix_product(pt, composite);
}


//extra...



void copyMatrix(float to[][4], float from[][4]) {
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) to[i][j] = from[i][j];
}

void unitMatrix(float matrix[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) matrix[i][j] = 1;
			else matrix[i][j] = 0;
		}
	}
}

void composeTransform(float transformMat2[][4], float transformMat[][4]) {
	float tmpMatrix[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmpMatrix[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				tmpMatrix[i][j] += transformMat2[i][k] * transformMat[k][j];
			}
		}
	}
	copyMatrix(transformMat, tmpMatrix);
}

void ortho_projMatrix(float matrix[][4], float Zvp) {
	unitMatrix(matrix);
	matrix[2][2] = 0;
	matrix[2][3] = Zvp;
}

void oblique_projMatrix(float matrix[][4], float alpha_d, float phi_d, float Zvp) {
	unitMatrix(matrix);
	phi_d *= 3.1416 / 180.0f;
	alpha_d *= 3.1416 / 180.0f;
	float cot_alpha_d = 1 / tanf(alpha_d);
	float cos_phi_d = cosf(phi_d);
	float sin_phi_d = sinf(phi_d);
	matrix[0][2] = cot_alpha_d * cos_phi_d;
	matrix[1][2] = cot_alpha_d * sin_phi_d;
	matrix[0][3] = -Zvp * cot_alpha_d * cos_phi_d;
	matrix[1][3] = -Zvp * cot_alpha_d * sin_phi_d;
	matrix[2][2] = 0;
	matrix[2][3] = Zvp;
}

void perspective_projMatrix(float matrix[][4], vect4 prp, float Zvp) {
	unitMatrix(matrix);
	matrix[0][0] = Zvp - prp.z;
	matrix[0][2] = prp.x;
	matrix[0][3] = -Zvp * prp.x;

	matrix[1][1] = Zvp - prp.z;
	matrix[1][2] = prp.y;
	matrix[1][3] = -Zvp * prp.y;

	matrix[2][2] = Zvp;
	matrix[2][3] = -Zvp * prp.z;

	matrix[3][2] = 1;
	matrix[3][3] = -prp.z;
}

void scaleXYZMatrix(float matrix[][4], float sx, float sy, float sz) {
	unitMatrix(matrix);
	matrix[0][0] = sx;
	matrix[1][1] = sy;
	matrix[2][2] = sz;
}

void translateMatrix(float matrix[][4], float x, float y, float z) {
	unitMatrix(matrix);
	matrix[0][3] = x;
	matrix[1][3] = y;
	matrix[2][3] = z;
}

void rotateMatrixXYZ(float matrix[][4], float deg_angle, char XYZ) {
	unitMatrix(matrix);
	deg_angle *= 3.1416 / float(180);
	XYZ = XYZ - 'X' + 1;
	matrix[(XYZ + 0) % 3][(XYZ + 0) % 3] = cos(deg_angle);
	matrix[(XYZ + 0) % 3][(XYZ + 1) % 3] = -sin(deg_angle);

	matrix[(XYZ + 1) % 3][(XYZ + 0) % 3] = sin(deg_angle);
	matrix[(XYZ + 1) % 3][(XYZ + 1) % 3] = cos(deg_angle);
}

void display_transform(float mat[][4]);
void rotateMatrixAxis(float matrix[][4], vect4 axis, float deg_angle, float Imatrix[][4]) {
	float transformMat[4][4];
	float transformMat2[4][4];
	float prec = 0.001;
	float pi2th = 3.1416 / 2;
	float mag = pow(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z, 0.5f);
	axis.x /= mag;
	axis.y /= mag;
	axis.z /= mag;
	//std::cout << "axis : " << axis.x << "i " << axis.y << "j " << axis.z << "k \n";
	float alpha, beta;
	if (abs(axis.z) > prec) {
		alpha = atan2f(axis.y, axis.z) * 90 / pi2th;
	}
	else {
		if (abs(axis.y) > prec) {
			alpha = (axis.y > 0) ? 90:-90;//pi2th : -pi2th;
		}
		else {
			if (abs(axis.x) > prec) {
				//std::cout << "rotX angle = " << deg_angle << "\n";
				if(axis.x >= 0) rotateMatrixXYZ(transformMat, deg_angle, 'X');
				else rotateMatrixXYZ(transformMat, -deg_angle, 'X');
			}
			else
				unitMatrix(transformMat);
			copyMatrix(matrix, transformMat);
			//display_transform(matrix);
			return;
		}
	}
	if (abs(axis.x) > prec) {
		beta = atan2f(axis.x, pow(axis.y * axis.y + axis.z * axis.z, 0.5f)) * 90 / pi2th;
	}
	else beta = 0;
	//std::cout <<"alpha = "<< alpha<<" beta = "<< beta<< "\n";
	rotateMatrixXYZ(transformMat, -alpha, 'X');
	rotateMatrixXYZ(transformMat2, -beta, 'Y');
	composeTransform(transformMat2, transformMat);
	rotateMatrixXYZ(transformMat2, deg_angle, 'Z');
	composeTransform(transformMat2, transformMat);
	rotateMatrixXYZ(transformMat2, beta, 'Y');
	composeTransform(transformMat2, transformMat);
	rotateMatrixXYZ(transformMat2, alpha, 'X');
	composeTransform(transformMat2, transformMat);
	copyMatrix(matrix, transformMat);
	//std::cout << "\n" <<alpha*90/pi2th<<" " << beta * 90 / pi2th << "\n";
	//display_transform(matrix);
}

void shearingMatrixXYZ(float matrix[][4], float par1, float par2, char axisXYZ, float ref) {
	unitMatrix(matrix);
	int j = axisXYZ - 'X';
	if (j < 0 || j>2) return;
	int i = (j + 1) % 3;
	matrix[i][j] = par1;
	matrix[i][3] = -par1 * ref;

	i = (i + 1) % 3;
	matrix[i][j] = par2;
	matrix[i][3] = -par2 * ref;

}

void reflectionMatrixXYZ(float matrix[][4], char XYZ) {//float A, float B, float C, float D = 0) {
	unitMatrix(matrix);
	int i = XYZ - 'X';
	matrix[i][i] = -1;
}

void viewMatrix(float viewMat[][4], vect4 from, vect4 to, vect4 up, float IviewMat[][4]) {
	unitMatrix(viewMat);
	vect4& axis = to;
	axis = to - from;
	axis = axis.normalize();
	vect4 right = up.crossProduct(axis);

	viewMat[0][0] = right.x; viewMat[0][1] = right.y; viewMat[0][2] = right.z;
	viewMat[1][0] = up.x; viewMat[1][1] = up.y; viewMat[1][2] = up.z;
	viewMat[2][0] = axis.x; viewMat[2][1] = axis.y; viewMat[2][2] = axis.z;

	float transMat[4][4];

	if (IviewMat != nullptr) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				IviewMat[i][j] = viewMat[j][i];
		translateMatrix(transMat, from.x, from.y, from.z);
		composeTransform(transMat, IviewMat);
	}

	translateMatrix(transMat, -from.x, -from.y, -from.z);
	composeTransform(viewMat, transMat);
	copyMatrix(viewMat, transMat);
}

void display_transform(float mat[][4] ) {
	std::cout << "\n";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << "\t" << mat[i][j];
		}
		std::cout << "\n";
	}
}