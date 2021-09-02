#pragma once
#include "vect.h"
#include "transform.h"
#include "matrixFunc.h"

#include "KEEYS.h"

extern vec2i MousePos;
extern float deltaTime;
extern vect4 camera_up, camera_right, camera_front;
extern float ZNEAR, ZFAR;

class Light {
public:
	vect4 Position;
    vect4 drawPosition;
	vect4 Il;
    bool draw_flag;
    bool move_flag;
    bool z_dir;
    char rgba;
    vec2i MousePosRec;
    float rad;
    float light_incr = 0.1;
    Light() {
        Position = vect4(275,-273,840,0);
        Il = { 0.9f, 0.9f, 0.9f };
        draw_flag = false;
        move_flag = false;
        z_dir = false;
        rgba = 'a';
        rad = 50 / float(abs(ZNEAR - ZFAR));
    }
    void translate(vect4 pt)
    {
        Position = Position + pt;
    }
    void setPosition(float x, float y, float z) {
        Position.x = x;
        Position.y = y;
        Position.z = z;
    }
    vect4 getPosition() {
        return Position;
    }
    void setIntensities(float r, float g, float b) {
        Position.x = r;
        Position.y = g;
        Position.z = b;
    }
    vect4 getIntensities() {
        return Il;
    }
    void move(float x, float y, float z) {
        Position.x += x;
        Position.y += y;
        Position.z += z;
    }
    void transform(mat4f& viewMat, mat4f& projection) {
        drawPosition = mul(viewMat, Position);
        drawPosition = mulProj(projection, drawPosition);
    }
    void draw() {
        if (!draw_flag)
            return;
        //glColor3d(1, 0.65, 0);
        glColor3d(Il.x, Il.y, Il.z);
        glPushMatrix();
        glTranslatef(drawPosition.x+800/2, drawPosition.y+800/2, 0);

        //glTranslatef(3.0, 0.0, -6.0);
        glutSolidSphere(rad*abs(drawPosition.z), 50, 50);
        glPopMatrix();
    }
    void processKeyboard(KEEYS key) {
        switch (key){

        case KEEYS::LKEY:
            std::cout << "\nL key pressed";
            if (!move_flag) {
                move_flag = true;
                MousePosRec = MousePos;
            }
            else {
                move_flag = false;
                z_dir = false;
            }
            break;
        case KEEYS::ZKEY:
            if (move_flag) {
                z_dir = !z_dir;
            }
            break;
        case KEEYS::OKEY:
            draw_flag = !draw_flag;
            break;
        case KEEYS::ONE:
            rgba = 'r';
            break;
        case KEEYS::TWO:
            rgba = 'g';
            break;
        case KEEYS::THREE:
            rgba = 'b';
            break;
        case KEEYS::FOUR:
            rgba = 'a';
            break;
        case KEEYS::PLUS:
            if (rgba == 'r' || rgba == 'a') {
                Il.x += light_incr;
                if (Il.x > 1) Il.x = 1;
            }
            if (rgba == 'g' || rgba == 'a') {
                Il.y += light_incr;
                if (Il.y > 1) Il.y = 1;
            }
            if (rgba == 'b' || rgba == 'a') {
                Il.z += light_incr;
                if (Il.z > 1) Il.z = 1;
            }
            if (rgba == '#') {

            }
                
            //std::cout << "plus";

            break;
        case KEEYS::MINUS:
            if (rgba == 'r' || rgba == 'a') {
                Il.x -= light_incr;
                if (Il.x < 0) Il.x = 0;
            }
            if (rgba == 'g' || rgba == 'a') {
                Il.y -= light_incr;
                if (Il.y < 0) Il.y = 0;
            }
            if (rgba == 'b' || rgba == 'a') {
                Il.z -= light_incr;
                if (Il.z < 0) Il.z = 0;
            }
            if (rgba == '#') {

            }
            //std::cout << "minus";
            break;
        }
    }
    void mouseMoveEvent(int x, int y) {
        //std::cout << "mousemove <- camera called";
        if (move_flag) {
            //std::cout << "mousemove <- camera flag is true";
            float delx = x - MousePosRec.x; MousePosRec.x = x;
            float dely = y - MousePosRec.y; MousePosRec.y = y;
            float speed = 0.06;
            delx = delx * speed * deltaTime; //delx *= -1;
            dely = -dely * speed * deltaTime; //dely *= -1;
            //std::cout << "\n "<<delx<<" , " << dely << " z_flag : " << z_dir;
            if (z_dir) {
                Position = Position + camera_front*dely;
                //std::cout << "\nz :  [ " << dely << " ]" << (camera_front * dely);
                return;
            }
            Position = Position + camera_right * delx;
            Position = Position + camera_up * dely;
            //std::cout << "light Pos : "<<Position;
            //std::cout << "\nx : [ "<<delx<< " ]" << (camera_right * delx) << "y : [ " << dely << " ]" << (camera_up * dely);
        }
    }
    void turn_flags_off() {
        move_flag = z_dir = false;
    }
};

