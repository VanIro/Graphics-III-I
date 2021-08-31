#pragma once

#include "lineFunc.h"
#include "matrixFunc.h"
#include "vertex.h"




template <typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha);

vect4 calcIntensity(vect4& Ka, vect4& Kd, vect4& Ks, float ns, vect4& point, vect4 light, vect4& view, vect4& normal, vect4& Ia, vect4 Il);

void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
void DrawFlatTopTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2);
void DrawFlatBottomTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2);
void DrawFlatTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2, const Vertex& dv0, const Vertex& dv1, Vertex itEdge1);

void fillTriangle(vect4 v1, vect4 v2, vect4 v3, vec3 c);
void fillBottomFlatTriangle(vect4 v1, vect4 v2, vect4 v3, vec3 c);
void fillTopFlatTriangle(vect4 v1, vect4 v2, vect4 v3, vec3 c);

void wireframe_draw(vect4 v1, vect4 v2, vect4 v3, vec3 c);
void drawWireframe_model(std::vector<Triangle>& model);
void draw_model(std::vector<Triangle>& model, bool Shade);

