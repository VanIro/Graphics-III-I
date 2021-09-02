
#include"colorOptions.h"
#include"vect.h"


//const unsigned int SSWIDTH = 800;
//const unsigned int SSHEIGHT = 800;

extern const unsigned int WIDTH = 800;
extern const unsigned int HEIGHT = 800;
extern const unsigned int ZHEIGHT = 500;
float z_buffer[WIDTH][HEIGHT];

float deltaTime = 0.0f;

float ZFAR = -400.0f;
float ZNEAR = 700.0f;
float XRIGHT = WIDTH / 2.0f;
float XLEFT = -XRIGHT;
float YUP = HEIGHT/2.0f;
float YDOWN = -YUP;

const int mat_sz = 4;

float deg_to_rad(float degree)
{
    return (3.1416 * degree / 180);
}

float rad_to_deg(float radian)
{
    return (180 * radian) / 3.1416;
}

float lightX = 500.0f;
float lightY = 200.0f;
float lightZ = -500.0f;

vect4 view = vect4{ 0.0f, 0.0f, 1.0f };
vect4 Ka = vect4{ 0.19125f, 0.19125f, 0.19225f };
//vect4 Kd = vect4{ 0.50754f, 0.50754f, 0.50754f };
//vect4 Ks = vect4{ 0.508273f, 0.508273f, 0.508273f };
vect4 light = vect4{ lightX, lightY, lightZ };
//vect4 light = vect4{ lightX, lightY, lightZ };
//float ns = 300.0f;
vect4 Ia = vect4{ 0.5f, 0.5f, 0.5f };
vect4 Il = vect4{ 0.9f, 0.9f, 0.9f };


float vect4::a = 0;// = b = c = X = Y = Z = 0;
float vect4::b = 0;
float vect4::c = 0;
float vect4::X = 0;
float vect4::Y = 0;
float vect4::Z = 0;

vec3 WHITE = { 1.0f, 1.0f, 1.0f };
vec3 RED = { 1.0f, 0.0f, 0.0f };
vec3 GREEN = { 0.0f, 1.0f, 0.0f };
vec3 BLUE = { 0.0f, 0.0f, 1.0f };
vec3 YELLOW = { 1.0f, 1.0f, 0.0f };
vec3 BLACK = { 0.0f, 0.0f, 0.0f };
vec3 ORANGE = { 1.0f, 0.5f, 0.0f };
vec3 PURPLE = { 1.0f, 0.0f, 1.0f };

float dotProduct(vect4 pt1, vect4 pt2)
{
    return { pt1.x * pt2.x + pt1.y * pt2.y + pt1.z * pt2.z };
}

vec2i MousePos;

vect4 camera_right = {0,0,0,0};
vect4 camera_up = { 0,0,0,0 };
vect4 camera_front = { 0,0,0,0 };

bool depth_buff_check(int x, int y , float z) {
    if (z_buffer[x][y] < z) {
        z_buffer[x][y] = z;
        return true;
    }

}