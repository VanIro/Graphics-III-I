#include "camera.h"

extern vec2i MousePos;
extern float ZNEAR, ZFAR;
extern vect4 camera_right, camera_up, camera_front;
Camera::Camera()
{
    viewFree_flag = false;

    thetaX = 0.0;
    thetaY = 0.0;
    thetaZ = 0.0;
    Position = { 0.0f, 0.0f, -100.0f };
    //Target = { 0.0f, -1.0f, 1.0f };
    camera_front = Front = { 0.0f, 0.0f, 1.0f };
    camera_up = Up = { 0.0f, 1.0f, 0.0f };
    camera_right = { 1,0,0,0 };
    front_flag = false;
    zoom = 20.0f;
    Speed = 2.0f;
    setViewMatrix(Position, Front + Position, Up);
    setProjectionMatrix(vect4(0,0,ZNEAR*1.1,0), ZNEAR*1.1+300);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    vect4 Right = { 1,0,0,0 };
    vect4 Up = { 0,1,0,0 };
    vect4 Front = { 0,0,1,0 };

    bool refresh_req = false;
    //To process keyboard inputs
    vect4 move(0, 0, 0, 0);
    if (direction == Camera_Movement::UP) {
        move = Up* deltaTime* Speed;
        Position = Position + move;
        refresh_req = true;
    }
    if (direction == Camera_Movement::DOWN) {
        move = Up * deltaTime * -Speed;
        Position = Position + move;
        refresh_req = true;
    }
    if (direction == Camera_Movement::LEFT) {
        move = Right * deltaTime * -Speed;
        Position = Position + move;
        refresh_req = true;
    }
    if (direction == Camera_Movement::RIGHT) {
        move = Right * deltaTime * Speed;
        Position = Position + move;
        refresh_req = true;
    }

    if (direction == Camera_Movement::FORWARD) {
        move = Front * deltaTime * Speed*0.8f;
        std::cout << "front..." << move.x << " " << move.y << " " << move.z << "\n";
        Position = Position + move;
        refresh_req = true;
    }
    if (direction == Camera_Movement::BACK) {
        move = Front * deltaTime * (-Speed)*.8f;
        Position = Position + move;
        std::cout<<Position;
        refresh_req = true;
    }

    if (direction == Camera_Movement::VKEY) {
        if (!viewFree_flag) {
            MousePosRec = MousePos;
            viewFree_flag = true;
        }
        else {
            viewFree_flag = false;
        }
    }

    if (refresh_req) {
        //refreshViewMatrix();
        float tmp_mat[4][4];
        translateMatrix(tmp_mat, move.x, move.y, move.z);
        composeTransform(tmp_mat, viewMat);

        translateMatrix(tmp_mat, -move.x, -move.y, -move.z);
        mul(IviewMat, tmp_mat, IviewMat);
        
    }
}

void Camera::mouseMoveEvent(int x, int y) {
    //std::cout << "mousemove <- camera called";
    if (viewFree_flag) {
        //std::cout << "mousemove <- camera flag is true";
        float delx = x - MousePosRec.x; MousePosRec.x = x;
        float dely = y - MousePosRec.y; MousePosRec.y = y;
        float magntd = powf(delx * delx + dely * dely, 0.5f);
        if (magntd <= 0.001)
            return;
        delx /= magntd; dely /= magntd;
        //std::cout << delx << " " << dely << " " << magntd << "\n";
        dely *= -1;   //if y dirn is downwards <window-y>
        vect4 axis(dely,-delx,0,0);
        //axis = axis * -1; //if z dirn is inwards <coordinate-z>
        //rotate proporp... to magntd about axis
        magntd *= 0.015 * deltaTime; //std::cout << 0.002 * deltaTime;
        float tmp_mat1[4][4], Itmp_mat1[4][4];
        rotateMatrixAxis(tmp_mat1, axis, magntd);

        rotateMatrixAxis(Itmp_mat1, axis, -magntd);
        composeTransform(IviewMat, Itmp_mat1);
        copyMatrix(IviewMat, Itmp_mat1);
        //composeTransform(Itmp_mat1, viewMat, Itmp_mat1);
        //mul(Itmp_mat1, viewMat, Itmp_mat1); //Itmp_mat1=Itmp_mat1*viewMat
        //rotate front, up, right
        camera_front = Front = mul(Itmp_mat1, {0,0,1,0}).normalize();
        camera_right = Right = mul(Itmp_mat1, {1,0,0,0}).normalize();
        camera_up = Up = mul(Itmp_mat1, {0,1,0,0}).normalize();
        composeTransform(tmp_mat1, viewMat);
    }
}

void Camera::setViewMatrix(vect4 from, vect4 to, vect4 upp) {
    Position = from;
    camera_front = Front = (to - from).normalize(); //std::cout << "\n******" << Front<<"\t"<<Position;
    camera_up = Up = (upp - Front * dotProduct(Front, upp)).normalize();
    camera_right = Right = Up.crossProduct(Front);
    viewMatrix(viewMat, from, to, Up,IviewMat);
}

void Camera::refreshViewMatrix() {
    setViewMatrix(Position, Position + Front, Up);
}

mat4f Camera::getViewMatrix()
{
    mat4f mat;
    copyMatrix(mat.matrix4, viewMat);
    return mat;
}

void Camera::setProjectionMatrix(vect4 from, float zplane) {
    perspective_projMatrix(projectionMat, from, zplane);
}

mat4f Camera::getProjectionMatrix(mat4f& mat)
{
    //mat4f mat;
    copyMatrix(mat.matrix4, projectionMat);
    return mat;
}

void Camera::turn_flags_off() {
    viewFree_flag = false;
}