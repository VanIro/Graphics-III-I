#pragma once

#include <GL/glut.h>

#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>

#include <array>

#include "colorOptions.h"
#include"vect.h"

template <typename T>
inline T wrap_angle(T theta)
{
    if (theta >= 360)
        theta = 0;
    return theta;

}



struct mat4f
{
    float matrix4[4][4];
};


struct Triangle {
    vect4 vertices[3];
    int vert_indices[3];
    vect4 normals[3];
    vec2i texCoords[3];
    vec3 color;
    vect4 vertex_intensity[3];
    float zbuffer;

    Triangle(const Triangle& tri)
    {
        for (int i = 0; i < 3; i++) {
            vertices[i] = tri.vertices[i];
            vert_indices[i] = tri.vert_indices[i];
            normals[i] = tri.normals[i];
            texCoords[i] = tri.texCoords[i];
            vertex_intensity[i] = tri.vertex_intensity[i];

        }
        color = tri.color;
    }

    Triangle()
    {
        color = { 0.8,0.8,0.8 };

        vertices[0] = vect4{ 0, 0, 0 };
        vertices[1] = vect4{ 0, 0, 0 };
        vertices[2] = vect4{ 0, 0, 0 };
    }

    Triangle(vect4 a, vect4 b, vect4 c)
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }

    Triangle(vect4 a, vect4 b, vect4 c, vec3 mcolor)
    {
        color = mcolor;
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }

    void setVertex(vect4 a, vect4 b, vect4 c)
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }
    void setVertIndices(int a, int b, int c) {
        vert_indices[0] = a;
        vert_indices[1] = b;
        vert_indices[2] = c;
    }

    void setNormals(vect4 na, vect4 nb, vect4 nc)
    {
        normals[0] = na;
        normals[1] = nb;
        normals[2] = nc;
        normals[0].w = normals[1].w = normals[2].w = 0;
    }

    void setTexCoords(vec2i uv1, vec2i uv2, vec2i uv3)
    {
        texCoords[0] = uv1;
        texCoords[1] = uv2;
        texCoords[2] = uv3;

    }

    //void setIntensity(std::vector<float> intense);

    void setIntensity(std::vector<float> intense)
    {
        vertex_intensity[0] = { vertices[0].x, vertices[0].y, vertices[0].z, intense[0] };
        vertex_intensity[1] = { vertices[1].x, vertices[1].y, vertices[1].z, intense[1] };
        vertex_intensity[2] = { vertices[2].x, vertices[2].y, vertices[2].z, intense[2] };
        // std::cout<<vertex_intensity[0]<<" "<<vertex_intensity[1]<<" "<<vertex_intensity[2];
    }
    void setIntensity(int index,vect4 intense)
    {
        vertex_intensity[index] = intense;
        // std::cout<<vertex_intensity[0]<<" "<<vertex_intensity[1]<<" "<<vertex_intensity[2];
    }
    void Rasterize1();
    void Rasterize2();
};