#pragma once
#include <math.h>
#include <iostream>
#include"colorOptions.h"

extern const int mm;

float deg_to_rad(float degree);

float rad_to_deg(float radian);

struct vec2i
{
    float x, y;
};

struct vect4
{
    float x = 0, y = 0, z = 0, w =1;
    /*float xfactor = WIDTH / 8.0f;
    float yfactor = HEIGHT / 8.0f;
    float zheight = ZHEIGHT / 8.0f;*/
    static float a, b, c, X, Y, Z; 

    /*vect4 Convert_to_Screen()
    {

        x = (x ) * xfactor;
        y = (y ) * yfactor;

        z = (z)*zheight;

        if (x < a)a = x; if (y < b)b = y; if (z < c)c = x;
        if (x > X)X = x; if (y > Y)Y = y; if (z > Z)Z = z;
        return { x,y,z };
    }*/
    vect4() {}
    vect4(float xx, float yy, float zz) {
        x = xx; y = yy; z = zz; 
    }
    vect4(float xx, float yy, float zz,float ww) {
        x = xx; y = yy; z = zz; w = ww;
    }
    vect4 inverse()
    {
        return { -x, -y, -z, w };
    }
    vect4 operator+(vect4 translate)
    {
        return { x + translate.x, y + translate.y, z + translate.z, w };
    }
    vect4 operator-(vect4 sub)
    {
        return { x - sub.x, y - sub.y, z - sub.z, w };
    }
    vect4 operator*(float mul)
    {
        return { float(mul * x), float(mul * y), float(mul * z), w };
    }
    vect4 operator*(vect4 mul)
    {
        return { float(mul.x * x), float(mul.y * y), float(mul.z * z), w };
    }
    vect4 operator*(vec3 mul)
    {
        return { float(mul.x * x), float(mul.y * y), float(mul.z * z), w };
    }
    vect4 operator/(float div)
    {
        return { float(x / div), float(y / div), float(z / div), float(w/div) };
    }
    vect4 scaleProduct(float pt)
    {
        return { x * pt, y * pt, z * pt, w };
    }
    vect4 scaleProduct(float sX, float sY, float sZ)
    {
        return { x * sX, y * sY, z * sZ, w };
    }
    vect4 crossProduct(vect4 pt)
    {
        vect4 ret;
        ret.x = y * pt.z - z * pt.y;
        ret.y = z * pt.x - x * pt.z;
        ret.z = x * pt.y - y * pt.x;
        return ret;
    }

    friend std::ostream& operator<<(std::ostream& out, vect4 vec)
    {
        out << "( " << vec.x << ", " << vec.y << ", " << vec.z <<" , " << vec.w << " )\n";
        return out;
    }

    vect4 normalize()
    {
        float mag = x * x + y * y + z * z;
        mag = pow(mag, 0.5);
        return { x / mag, y / mag, z / mag };
    }
};

struct Vector2f {
    float x, y;
    Vector2f() {}
    Vector2f(float xi, float yi) { x = xi; y = yi; }
    //Vector2f(Vector3f v) { return { v.x,v.y }; }
    Vector2f operator*(float f) {
        return Vector2f{ x * f,y * f };
    }
    Vector2f operator-() {
        return Vector2f{ -x ,-y };
    }
    Vector2f operator-(Vector2f b) {
        return Vector2f{ x - b.x ,y - b.y };
    }
    Vector2f operator+(Vector2f b) {
        return Vector2f{ x + b.x ,y + b.y };
    }
    Vector2f operator+=(Vector2f b) {
        return Vector2f{ x + b.x ,y + b.y };
    }
    Vector2f operator-=(Vector2f b) {
        return Vector2f{ x - b.x ,y - b.y };
    }
    Vector2f operator*=(Vector2f b) {
        return Vector2f{ x * b.x ,y * b.y };
    }
    Vector2f operator/=(Vector2f b) {
        return Vector2f{ x / b.x ,y / b.y };
    }
};

float dotProduct(vect4 pt1, vect4 pt2);