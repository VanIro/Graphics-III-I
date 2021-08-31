#pragma once

#pragma once

#include "Coord.h"
#include "matrixFunc.h"
#include "transform.h"

extern float deltaTime;

//mat4f calculateLookAt(vect4& pos, vect4& target, vect4& up);
static enum class Camera_Movement {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARD,
    BACK,
    WKEY,
    AKEY,
    SKEY,
    DKEY,
    VKEY
};

class Camera
{
private:
    void updateCameraVectors();

public:
    //camera values
    float zoom;

    vect4 Position;
    vect4 Front;
    vect4 Up;
    vect4 Right;
    float Speed;

    float viewMat[4][4];
    float IviewMat[4][4];
    float projectionMat[4][4];

    bool viewFree_flag;

    bool front_flag;

    vec2i  MousePosRec;


    float thetaX, thetaY, thetaZ;

    Camera();
    void setViewMatrix(vect4 from, vect4 to, vect4 up);
    mat4f getViewMatrix();
    void setProjectionMatrix(vect4 from, float zplane);
    mat4f getProjectionMatrix(mat4f&);
    void refreshViewMatrix();
    void processKeyboard(Camera_Movement direction, float deltaTime); 
    void mouseMoveEvent(int x, int y);
    void proccessMouse();
    void turn_flags_off();
};
