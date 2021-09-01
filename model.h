#pragma once

#include<fstream>
#include<sstream>
//#include<algorithm>

#include "camera.h"
#include "transform.h"
#include "triangleRaster.h"



class Model
{
private:
    std::vector<vect4> vertices_list;
    std::vector<vect4> fvertices_list;
    std::vector<vect4> normals_list;
    std::vector<int> vert_norm_indices;

    std::vector<vect4> intensities_list;

    std::vector<Triangle> triangles;
    std::vector<Triangle> ftriangles;

    //std::vector<std::vector<vect4>> vertices;

public:
    void oldLoad(std::string);
    void Load(std::string);
    void modelToScreen();
    void rotate_model(char XYZ,float);
    void rotate_modelY2(float angle, float yref);
    void translate_model(vect4);
    void scale_model(float, char dim = 'N');
    bool backFaceDetection(Triangle& tri);
    void transformModel(mat4f&, mat4f&);
    //bool backfaceDetectionNormalize(Triangle& tri);
    bool Culling(Triangle& tri);
    //void updateTransform(mat4f& view, mat4f& projection);
    bool is_in_depthBox(int vertices[3]);
    bool is_in_depthBox(vect4 vertices[3]);
    bool is_in_viewPlane(int vertices[3]); 
    bool is_in_viewPlane(vect4 vertices[3]);

    void draw();
    float calIntensity(vect4, vect4, vect4);
    void flatShading(Triangle&);
    void Shading(Triangle&);
    bool Shade = true;

    Camera* camera2;
};

