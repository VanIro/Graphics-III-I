#include "Model.h"
#include"include_globals.h"
#include "light.h"

extern const unsigned int WIDTH;
extern const unsigned int HEIGHT;
extern float ZNEAR,ZFAR,XLEFT,XRIGHT,YUP,YDOWN;
extern Light* LLight;

void Model::draw()
{
    //drawWireframe_model(ftriangles);
    draw_model(ftriangles, Shade);
}

void Model::oldLoad(std::string filename)
{
    std::ifstream file;
    file.open(filename);
    if (file.fail())
    {
        std::cout << "File cannot be opened \n";
        exit(-1);
    }
    // Local cache of verts
    std::vector<vect4> verts;

    while (!file.eof())
    {
        char line[128];
        file.getline(line, 128);

        std::stringstream s;
        s << line;

        char junk;

        if (line[0] == 'v')
        {
            vect4 v;
            s >> junk >> v.x >> v.y >> v.z;
            verts.push_back(v);
        }

        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            triangles.push_back(Triangle{ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
        }
    }
    ftriangles = triangles;
}

void Model::Load(std::string filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
    {
        std::cout << "Cannot Read" << std::endl;
        exit(-1);
    }
    std::string line;
    std::vector<vect4> verts;
    std::vector<vect4> normals;
    std::vector<vec2i> textures;
    float minX, maxX, minY, maxY, minZ, maxZ;
    minX = maxX = minY = maxY = minZ = maxZ = 0;
    int count = 1;
    int n_iter = 0;
    while (!in.eof())
    {
        //if (count > 10000) break;
        n_iter++;
        //get one line at a time
        std::getline(in, line);
        //string object
        std::istringstream iss(line.c_str());

        char trash;
        if (!line.compare(0, 2, "v ")) //starts with v<space>
        {

            // std::cout << count;
            iss >> trash; // first character is v
            vect4 v;
            // followed by xyz co-ords
            iss >> v.x; v.x += 3;
            if (v.x < minX)minX = v.x; if (v.x > maxX)maxX = v.x;
            iss >> v.y; v.y -= 8;
            if (v.y < minY)minY = v.y; if (v.y > maxY)maxY = v.y;
            iss >> v.z; v.z -= 3;
            if (v.z < minZ)minZ = v.z; if (v.z > maxZ)maxZ = v.z;
            v.w = 1;
            // maths::printvec(v);
            verts.push_back(v);
            count++;
        }
        //else if (!line.compare(0, 3, "vt ")) //starts with vt<space>
        //{
        //    iss >> trash >> trash; //Ignore vt
        //    vec2i uv;
        //    iss >> uv.x;
        //    iss >> uv.y;
        //    textures.push_back(uv);
        //}

        else if (!line.compare(0, 3, "vn ")) //starts with vn<space>
        {
            iss >> trash >> trash;
            vect4 n;
            iss >> n.x;
            iss >> n.y;
            iss >> n.z;
            n.w = 0;
            normals.push_back(n);
        }
        else if (!line.compare(0, 2, "f ")) //starts with f<space>
        {
            std::vector<vect4> f;
            vect4 temp;

            iss >> trash; //first charecter is f

            while (iss >> temp.x >> trash >> temp.y >> trash >> temp.z)
                // in the form vert/vertTex/norm (vert is read, the rest are treated as trash)
            {
                //in wavefront obj all indices start at 1, not zero
                temp.x--; //vert
                temp.y--; //texture
                temp.z--; // normal
                f.push_back(temp);
            }
            Triangle tri;
            tri.setVertex(verts[f[0].x], verts[f[1].x], verts[f[2].x]);
            // std::cout  << f[0][0] <<'\n';
            //tri.setTexCoords(textures[f[0].y], textures[f[1].y], textures[f[2].y]);
            tri.setNormals(normals[f[0].z], normals[f[1].z], normals[f[2].z]);
            triangles.push_back(tri);
        }
    }
    for (size_t i = 0; i < triangles.size() - 1; i++)
    {
        triangles[i].zbuffer = (triangles[i].vertices[0].z + triangles[i].vertices[1].z + triangles[i].vertices[2].z);
    }
    ftriangles = triangles;
    std::cout << "\n" << minX << " --> " << maxX;
    std::cout << "\n" << minY << " --> " << maxY;
    std::cout << "\n" << minZ << " --> " << maxZ;
}

//void Model::modelToScreen()
//{
//    for (int i = 0; i < triangles.size(); i++)
//    {
//        for (int j = 0; j < 3; j++)
//        {
//            triangles[i].vertices[j] = triangles[i].vertices[j].Convert_to_Screen();
//        }
//    }
//}

void Model::translate_model(vect4 pt)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            transate_polygon(triangles[i].vertices[j], pt);
        }
    }
}

void Model::scale_model(float pt, char dim )
{
    if (dim == 'X')
        for (int i = 0; i < triangles.size(); i++)
            for (int j = 0; j < 3; j++)
                scale_polygon(triangles[i].vertices[j], pt, 'X');
    if (dim == 'Y')
        for (int i = 0; i < triangles.size(); i++)
            for (int j = 0; j < 3; j++)
                scale_polygon(triangles[i].vertices[j], pt, 'Y');
    if (dim == 'Z')
        for (int i = 0; i < triangles.size(); i++)
            for (int j = 0; j < 3; j++)
                scale_polygon(triangles[i].vertices[j], pt, 'Z');
    else
        for (int i = 0; i < triangles.size(); i++)
            for (int j = 0; j < 3; j++)
                scale_polygon(triangles[i].vertices[j], pt);
}

void Model::rotate_model(char XYZ, float angle)
{
    if (XYZ == 'X')
        for (int i = 0; i < triangles.size(); i++)
            for (int j = 0; j < 3; j++) {
                rotateX(triangles[i].vertices[j], angle);
                rotateX(triangles[i].normals[j], angle);
            }
    else if (XYZ == 'Y')
        for (int i = 0; i < triangles.size(); i++)
            for (int j = 0; j < 3; j++) {
                rotateY(triangles[i].vertices[j], angle);
                rotateY(triangles[i].normals[j], angle);
            }
    else if (XYZ == 'Z')
        for (int i = 0; i < triangles.size(); i++)
            for (int j = 0; j < 3; j++) {
                rotateZ(triangles[i].vertices[j], angle);
                rotateZ(triangles[i].normals[j], angle);
            }
}

void Model::rotate_modelY2(float angle, float yref)
{

    for (int i = 0; i < triangles.size(); i++)
        for (int j = 0; j < 3; j++)
            rotateY2(triangles[i].vertices[j], angle, yref);
}

bool is_in_depthBox(vect4 vertices[3]) {
    if (vertices[0].z < ZNEAR && vertices[0].z > ZFAR)
        if (vertices[1].z < ZNEAR && vertices[1].z > ZFAR)
            if (vertices[2].z < ZNEAR && vertices[2].z > ZFAR)
                return true;
    return false;
}
bool is_in_viewPlane(vect4 vertices[3]) {

    bool flag = false;
    //bool flag2 = true;
    for(int i=0;i<3;i++)
        if (vertices[i].x > XLEFT && vertices[i].x < XRIGHT) {
            flag = true;
        }
    if (!flag) return false;
    flag = false;
    for (int i = 0; i < 3; i++)
        if (vertices[i].y > YDOWN && vertices[i].y < YUP) {
            flag = true;
        }
    if (!flag) return false;

    return true;
}
void Model::transformModel(mat4f& viewMat, mat4f& projection)
{
    ftriangles.clear();
    int cullCount = 0;
    int clipped = 0;
    int faceCount = 0;
    for (auto& tri : triangles)
    {
        Triangle temptri = tri;

        bool culled = backFaceDetection(temptri);
        //bool culled = Culling(temptri);
        if (culled) {
            cullCount++;
            continue;
        }

        // view transformation
        temptri.vertices[0] = mul(viewMat, temptri.vertices[0]);
        temptri.vertices[1] = mul(viewMat, temptri.vertices[1]);
        temptri.vertices[2] = mul(viewMat, temptri.vertices[2]);

        //std::cout<< temptri.vertices[0] <<std::endl;
        //std::cout << temptri.vertices[1] << std::endl;
        //std::cout << temptri.vertices[2] << std::endl;

        //Projection Transformation
        if (is_in_depthBox(temptri.vertices)) {
            temptri.vertices[0] = mulProj(projection, temptri.vertices[0]);
            temptri.vertices[1] = mulProj(projection, temptri.vertices[1]);
            temptri.vertices[2] = mulProj(projection, temptri.vertices[2]);
            if (is_in_viewPlane(temptri.vertices)) {


                tri.color = vec3{ 0, 255, 200 }.normalize();
                if (Shade) {
                    temptri.setIntensity(0, calcIntensity(Ka, Kd, Ks, ns, tri.vertices[0], LLight->getPosition(), view, tri.normals[0], Ia, LLight->getIntensities()));
                    temptri.setIntensity(1, calcIntensity(Ka, Kd, Ks, ns, tri.vertices[1], LLight->getPosition(), view, tri.normals[1], Ia, LLight->getIntensities()));
                    temptri.setIntensity(2, calcIntensity(Ka, Kd, Ks, ns, tri.vertices[2], LLight->getPosition(), view, tri.normals[2], Ia, LLight->getIntensities()));
                }
                else {
                    flatShading(tri);
                }

                /*std::cout << "£" << temptri.vertices[0] << std::endl;
                std::cout << temptri.vertices[1] << std::endl;
                std::cout << temptri.vertices[2] << std::endl;*/
                for (int i = 0; i < 3; i++)
                    transate_polygon(temptri.vertices[i], { 800 / 2,800 / 2,0,0 });
                ftriangles.push_back(temptri);
                faceCount++;
            }
            else {
                clipped++;
                //std::cout << "\nclipped!";
            }
        }
        else {
            clipped++;
            //std::cout << "\n not in view box... " << temptri.vertices[0] << temptri.vertices[1] << temptri.vertices[2];
        }
    }
    //std::cout << "\n" << clipped << " Clipped , " << cullCount << "Culled\n"<<faceCount<<" ( "<< 100*faceCount/float(clipped+cullCount+faceCount)<<"% ) faces";


    /*sort(ftriangles.begin(), ftriangles.end(), [](Triangle& t1, Triangle& t2)
    {
        float z1 = (t1.vertices[0].z + t1.vertices[1].z + t1.vertices[2].z) / 3.0f;
        float z2 = (t2.vertices[0].z + t2.vertices[1].z + t2.vertices[2].z) / 3.0f;
        return z1 > z2;
    });*/

    //for (auto& tri : ftriangles)
    //{

    //    tri.color = vec3{ 0, 255, 200 }.normalize();
    //    if (Shade)
    //        ;// Shading(tri);
    //    else
    //        flatShading(tri);
    //}
}

bool Model::backFaceDetection(Triangle& tri)
{
    vect4 v1 = tri.vertices[0];
    vect4 v2 = tri.vertices[1];
    vect4 v3 = tri.vertices[2];

    vect4 n1 = tri.normals[0];
    vect4 n2 = tri.normals[1];
    vect4 n3 = tri.normals[2];


    vect4 view1 = (camera->Position - v1).normalize();
    vect4 view2 = (camera->Position - v2).normalize();
    vect4 view3 = (camera->Position - v3).normalize();

    //std::cout<< view1.x <<std::endl;
    //std::cout << view1.y << std::endl;
    //std::cout << view1.z << std::endl;
    //std::cout << view2.x << std::endl;
    //std::cout << view2.y << std::endl;
    //std::cout << view2.z << std::endl;
    //std::cout << view3.x << std::endl;
    //std::cout << view3.y << std::endl;
    //std::cout << view3.z << std::endl;

    float dotProduct1 = dotProduct(n1, view1);
    float dotProduct2 = dotProduct(n2, view2);
    float dotProduct3 = dotProduct(n3, view3);

    /*if (dotProduct1 < 0 || dotProduct2 < 0 || dotProduct3 < 0)
    {
        return false;
    }
    return true;*/
    if (n1.z < 0 || n2.z < 0 || n3.z < 0) {
        return false;
    }
    return true;
}

bool Model::Culling(Triangle& tri)
{
    vect4 line1 = tri.vertices[1] - tri.vertices[0];
    vect4 line2 = tri.vertices[2] - tri.vertices[0];
    vect4 normal = line1.crossProduct(line2);

    normal = normal.normalize();

    float val = dotProduct(normal, (tri.vertices[0] - camera->Position));
    if (val < 0)
        return true;
    return false;
}


//bool Model::backfaceDetectionNormalize(Triangle& tri)
//{
//    vect4 v1 = tri.vertices[0], v2 = tri.vertices[1], v3 = tri.vertices[2];
//    vect4 centroid;
//    centroid.x = (v1.x + v2.x + v3.x) / 3;
//    centroid.y = (v1.y + v2.y + v3.y) / 3;
//    centroid.z = (v1.z + v2.z + v3.z) / 3;
//
//    vect4 V = (camera->Position - centroid).normalize();
//    // vect4 V = (vect4{0,0,100} - centroid).normalize();
//
//    v2 = v2 - v1;
//    v3 = v3 - v1;
//
//    vect4 normal = v2.crossProduct(v3);
//    normal = normal.normalize();
//    // std::cout<<normal;
//
//    float value = dotProduct(normal, V);
//
//    if (value < 0)
//    {
//        ftriangles.push_back(tri);
//        return false;
//    }
//
//    return true;
//}

float Model::calIntensity(vect4 point, vect4 Normal, vect4 View)
{
    float i = 0.0;
    vect4 position = { 500, 600, -200 };
    vect4 Ldir = (light - point).normalize();// (position - point).normalize();
    // std::cout << point.x << "\t" << point.y << "\t" << point.z << "\n";
    float ambientInt = 0.3;
    float pointInt = 0.5;

    float ambientConstant = 0.7;
    float diffuseConstant = 0.7;
    float specularConstant = 0.8;

    float ambientLight = ambientConstant * ambientInt;

    float diffuseLight = std::max(0.0f, diffuseConstant * pointInt * dotProduct(Normal, Ldir));

    // Point R = maths::sub(maths::mul(Normal, (2 * maths::dot(Normal, Ldir))), Ldir);
    vect4 R = ((Normal * (2 * dotProduct(Normal, Ldir))) - Ldir).normalize();
    float specularExp = 32;
    float specularLight = specularConstant * pointInt * pow(dotProduct(R, View), specularExp);

    float tmp = ambientLight + specularLight + diffuseLight;
    tmp = tmp > 1 ? 1 : tmp;
    return tmp;
}

void Model::flatShading(Triangle& tri)
{
    vect4 v1 = tri.vertices[0];
    vect4 v2 = tri.vertices[1];
    vect4 v3 = tri.vertices[2];

    vect4 centroid;
    centroid.x = (v1.x + v2.x + v3.x) / 3;
    centroid.y = (v1.y + v2.y + v3.y) / 3;
    centroid.z = (v1.z + v2.z + v3.z) / 3;

    // std::cout << centroid[0] <<"\t";

    vect4 view = (camera->Position - centroid).normalize();
    // Point view = (Point{0,0,1000} - centroid).normalize();

    // generating the normal vector of a triangle
    vect4 ver1 = centroid - v2;
    vect4 ver2 = centroid - v3;

    vect4 normal = (ver1.crossProduct(ver2)).normalize();

    float intensity = calIntensity(centroid, normal, view);
    // std::cout << "The intensity: " << intensity << "\n";
    vec3 newColor = tri.color * intensity;

    tri.color = newColor;
}

//void Model::Shading(Triangle& tri)
//{
//    std::vector<float>intensity(3);
//    int count = 0;
//    for (auto& vertex : tri.vertices)
//    {
//        vect4 view = (vect4{ 0, 0, 10 } - vertex).normalize();
//        intensity[count] = calIntensity(vertex, tri.normals[count], view);
//        count++;
//    }
//    tri.setIntensity(intensity);
//}

//void Model::updateTransform(mat4f& view, mat4f& projection)
//{
//    mat4f rotation = mul(Matrix_MakeRotationX(camera->thetaX), mul(Matrix_MakeRotationY(camera->thetaY), Matrix_MakeRotationZ(camera->thetaZ)));
//    mat4f translate = Translation(0.0f, 0.0f, 5.0f);
//    mat4f matWorld = mul(translate, rotation);
//
//
//    ftriangles.clear();
//    // ftriangles = triangles;
//    int cullCount = 0;
//
//    for (auto& tri : triangles)
//    {
//        // Triangle temptri = tri;
//
//        Triangle triProjected, triTransformed, triViewed;
//
//        // World Matrix Transform
//        triTransformed.vertices[0] = mul(matWorld, tri.vertices[0]);
//        triTransformed.vertices[1] = mul(matWorld, tri.vertices[1]);
//        triTransformed.vertices[2] = mul(matWorld, tri.vertices[2]);
//        triTransformed.normals[0] = mul(rotation, tri.normals[0]);
//        triTransformed.normals[1] = mul(rotation, tri.normals[1]);
//        triTransformed.normals[2] = mul(rotation, tri.normals[2]);
//
//        //Convert World Space --> View Space
//        triViewed.vertices[0] = mul(view, triTransformed.vertices[0]);
//        triViewed.vertices[1] = mul(view, triTransformed.vertices[1]);
//        triViewed.vertices[2] = mul(view, triTransformed.vertices[2]);
//
//        triProjected.vertices[0] = mul(projection, triViewed.vertices[0]);
//        triProjected.vertices[1] = mul(projection, triViewed.vertices[1]);
//        triProjected.vertices[2] = mul(projection, triViewed.vertices[2]);
//
//        // bool culled = backfaceDetectionNormalize(tri);
//        // bool culled = backFaceCulling(temptri);
//        // if(!culled)
//        //     continue;
//
//        // // view transformation
//        // temptri.vertices[0] = mul(view, temptri.vertices[0]);
//        // temptri.vertices[1] = mul(view, temptri.vertices[1]);
//        // temptri.vertices[2] = mul(view, temptri.vertices[2]);
//
//        // //Projection Transformation
//        // temptri.vertices[0] = mul(projection, temptri.vertices[0]);
//        // temptri.vertices[1] = mul(projection, temptri.vertices[1]);
//        // temptri.vertices[2] = mul(projection, temptri.vertices[2]);
//
//        ftriangles.push_back(triProjected);
//    }
//
//    //------------------- painters algorithm    ---------------------------
//    // painterSort(ftriangles, 0, ftriangles.size());
//
//    sort(ftriangles.begin(), ftriangles.end(), [](Triangle& t1, Triangle& t2)
//        {
//            float z1 = (t1.vertices[0].z + t1.vertices[1].z + t1.vertices[2].z) / 3.0f;
//            float z2 = (t2.vertices[0].z + t2.vertices[1].z + t2.vertices[2].z) / 3.0f;
//            return z1 > z2;
//        });
//
//    for (auto& tri : ftriangles)
//    {
//        // Point color = Point{220, 220, 220}.normalize();
//
//        tri.color = vec3{ 179, 60, 0 }.normalize();
//        if (Shade)
//            Shading(tri);
//        else
//            phongIlluminationModel(tri);
//    }
//}
