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

    vect4 Kdm, Ksm;
    float nsm;
    vec3 colorm;
    bool ambient;
    vect4 Iam;

    //std::vector<std::vector<vect4>> vertices;

public:
    Model() {

        Kdm = vect4{ 0.50754f, 0.50754f, 0.50754f };
        Ksm = vect4{ 0.508273f, 0.508273f, 0.508273f };
        nsm = 100.0f;
        colorm = vec3{ 200, 205, 200 }.normalize();
        ambient = false;
        Iam = {1,1,1};
    }

    void oldLoad(std::string);
    void Load(std::string);
    void modelToScreen();
    void rotate_model(char XYZ,float);
    void rotate_modelY2(float angle, float yref);
    void translate_model(vect4);
    void scale_model(float, char dim = 'N');
    bool backFaceDetection(Triangle& tri);
    void transformModel(mat4f&, mat4f&);
    void set_ambience_maj(bool val) {
        ambient = val;
    }
    void set_ambience(float r, float g, float b){
        Iam = vect4(r, g, b);
    }
    //bool backfaceDetectionNormalize(Triangle& tri);
    bool Culling(Triangle& tri);
    //void updateTransform(mat4f& view, mat4f& projection);
    bool is_in_depthBox(int vertices[3]);
    bool is_in_depthBox(vect4 vertices[3]);
    bool is_in_viewPlane(int vertices[3]); 
    bool is_in_viewPlane(vect4 vertices[3]);

    void setColor(float r, float g, float b) {
        colorm.x = r;
        colorm.y = g;
        colorm.z = b;
        colorm = colorm.normalize();
    }
    void setKd(float r, float g, float b){
        Kdm.x = r;
        Kdm.y = g;
        Kdm.z = b;
    }
    void setKs(float r, float g, float b) {
        Ksm.x = r;
        Ksm.y = g;
        Ksm.z = b;
    }
    void setNs(float n){
        nsm = n;
    }
    void draw();
    float calIntensity(vect4, vect4, vect4);
    void flatShading(Triangle&);
    void Shading(Triangle&);
    bool Shade = true;

    Camera* camera2;
};

