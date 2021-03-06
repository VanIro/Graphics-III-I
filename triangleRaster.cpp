#include"triangleRaster.h"
#include"include_globals.h"
#include"colors_define.h"
//#include"light.h"

//const unsigned int SSWIDTH = 800;
//const unsigned int SSHEIGHT = 800;
//
// 
//float lightX = 500.0f;
//float lightY = 200.0f;
//float lightZ = -500.0f;
//
//vect4 view = vect4{ 0.0f, 0.0f, 1.0f };
//vect4 Ka = vect4{ 0.19125f, 0.19125f, 0.19225f };
//vect4 Kd = vect4{ 0.50754f, 0.50754f, 0.50754f };
//vect4 Ks = vect4{ 0.508273f, 0.508273f, 0.508273f };
//vect4 light = vect4{ lightX, lightY, lightZ };
////vect4 light = vect4{ lightX, lightY, lightZ };
//float ns = 300.0f;
//vect4 Ia = vect4{ 0.5f, 0.5f, 0.5f };
//vect4 Il = vect4{ 0.9f, 0.9f, 0.9f };

extern const unsigned int WIDTH;
extern const unsigned int HEIGHT;
//extern Light* LLight;

template <typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha)
{
    return src + (dst - src) * alpha;
}

vect4 calcIntensity(vect4& Ka, vect4& Kd, vect4& Ks, float ns, vect4& point, vect4 light, vect4& view, vect4& normal, vect4& Ia, vect4 Il)
{
    //calculate ambient color
    vect4 ambientColor = Ka * Ia;
    vect4 lightVec = light - point;
    vect4 unitLight = lightVec.normalize();
    //calculate diffuse color
    vect4 diffuseColor = Kd * Il * dotProduct(normal, unitLight);
    vect4 reflection = (normal * 2.0 * dotProduct(normal, unitLight)) - unitLight;
    float dot_R_V = dotProduct(view, reflection);
    float dot_R_V_powered = pow(dotProduct(view, reflection), ns);
    vect4 specularColor;
    if (diffuseColor.x < 0) {
        diffuseColor.x = 0;
        specularColor.x = 0;
        diffuseColor.x *= -1;
    }
    else if (dot_R_V < 0) specularColor.x = 0;
    else specularColor.x = 1;
    if (diffuseColor.y < 0) {
        diffuseColor.y = 0;
        specularColor.y = 0;
        diffuseColor.y *= -1;
    }
    else if (dot_R_V < 0) specularColor.y = 0;
    else specularColor.y = 1;
    if (diffuseColor.z < 0) {
        diffuseColor.z = 0;
        specularColor.z = 0;
        diffuseColor.z *= -1;
    }
    else if (dot_R_V < 0) specularColor.z = 0;
    else specularColor.z = 1;
    //calculate specular color
    specularColor = specularColor * Ks * Il * dot_R_V_powered;
    vect4 intensity = ambientColor + diffuseColor + specularColor;
    //intensity.clipTo1();
    if (intensity.x > 1) intensity.x = 1;
    if (intensity.y > 1) intensity.y = 1;
    if (intensity.z > 1) intensity.z = 1;
    return intensity;
}

void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
    // using pointers so we can swap (for sorting purposes)
    const Vertex* pv0 = &v0;
    const Vertex* pv1 = &v1;
    const Vertex* pv2 = &v2;
    // sorting vertices by y
    if (pv1->pos.y < pv0->pos.y)
        std::swap(pv0, pv1);
    if (pv2->pos.y < pv1->pos.y)
        std::swap(pv1, pv2);
    if (pv1->pos.y < pv0->pos.y)
        std::swap(pv0, pv1);
    if (pv0->pos.y == pv1->pos.y) // natural flat top
    {
        // sorting top vertices by x
        if (pv1->pos.x < pv0->pos.x)
            std::swap(pv0, pv1);
        DrawFlatTopTriangle(*pv0, *pv1, *pv2);
    }
    else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
    {
        // sorting bottom vertices by x
        if (pv2->pos.x < pv1->pos.x)
            std::swap(pv1, pv2);
        DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
    }
    else // general triangle
    {
        // find splitting vertex interpolant
        const float alphaSplit =
            (pv1->pos.y - pv0->pos.y) /
            (pv2->pos.y - pv0->pos.y);
        const auto vi = interpolate(*pv0, *pv2, alphaSplit);
        if (pv1->pos.x < vi.pos.x) // major right
        {
            DrawFlatBottomTriangle(*pv0, *pv1, vi);
            DrawFlatTopTriangle(*pv1, vi, *pv2);
        }
        else // major left
        {
            DrawFlatBottomTriangle(*pv0, vi, *pv1);
            DrawFlatTopTriangle(vi, *pv1, *pv2);
        }
    }
}
// does flat *TOP* tri-specific calculations and calls DrawFlatTriangle
void DrawFlatTopTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2)
{
    // calulcate dVertex / dy
    // change in interpolant for every 1 change in y
    const float delta_y = it2.pos.y - it0.pos.y;
    const auto dit0 = (it2 - it0) / delta_y;
    const auto dit1 = (it2 - it1) / delta_y;
    // create right edge interpolant
    auto itEdge1 = it1;
    // call the flat triangle render routine
    DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
}
// does flat *BOTTOM* tri-specific calculations and calls DrawFlatTriangle
void DrawFlatBottomTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2)
{
    // calulcate dVertex / dy
    // change in interpolant for every 1 change in y
    const float delta_y = it2.pos.y - it0.pos.y;
    const auto dit0 = (it1 - it0) / delta_y;
    const auto dit1 = (it2 - it0) / delta_y;
    // create right edge interpolant
    auto itEdge1 = it0;
    // call the flat triangle render routine
    DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
}
// does processing common to both flat top and flat bottom tris
// scan over triangle in screen space, interpolate attributes,
// invoke ps and write pixel to screen
bool depth_buffer_check(int, int, float);
void DrawFlatTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2, const Vertex& dv0, const Vertex& dv1, Vertex itEdge1)
{
    // create edge interpolant for left edge (always v0)
    auto itEdge0 = it0;
    // calculate start and end scanlines
    const int yStart = (int)ceil(it0.pos.y - 0.5f);
    const int yEnd = (int)ceil(it2.pos.y - 0.5f); // the scanline AFTER the last line drawn
    // do interpolant prestep
    itEdge0 += dv0 * (float(yStart) + 0.5f - it0.pos.y);
    itEdge1 += dv1 * (float(yStart) + 0.5f - it0.pos.y);
    for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
    {
        // calculate start and end pixels
        const int xStart = (int)ceil(itEdge0.pos.x - 0.5f);
        const int xEnd = (int)ceil(itEdge1.pos.x - 0.5f); // the pixel AFTER the last pixel drawn
        // create scanline interpolant startpoint
        // (some waste for interpolating x,y,z, but makes life easier not having
        //  to split them off, and z will be needed in the future anyways...)
        auto iLine = itEdge0;
        // calculate delta scanline interpolant / dx
        const float dx = itEdge1.pos.x - itEdge0.pos.x;
        const auto diLine = (itEdge1 - iLine) / dx;
        // prestep scanline interpolant
        iLine += diLine * (float(xStart) + 0.5f - itEdge0.pos.x);
        for (int x = xStart; x < xEnd; x++, iLine += diLine)
        {
            // invoke pixel shader and write resulting vec3 value
            vec3 color = { iLine.color.x, iLine.color.y, iLine.color.z };
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                if (z_buffer[x][y] < iLine.pos.z) {
                    z_buffer[x][y] = iLine.pos.z;
                    putpixel(x, y, color);
                }
            //putpixel(x, y, color);
            /*float tex_width = getWidth();
            float tex_height = getHeight();
            int tex_xclamp = tex_width - 1;
            int tex_yclamp = tex_height - 1;
            Point colorVec = GetPixel(int(std::min(iLine.tc.x * tex_width + 0.5f, tex_xclamp)), int(std::min(iLine.tc.y * tex_height + 0.5f, tex_yclamp)));
            Color color = { colorVec.x, colorVec.y, colorVec.z };
            putPixel(x, y, color); */
        }
    }
}

void fillTriangle(vect4 v1, vect4 v2, vect4 v3, vec3 c)
{
    if (v1.y > v2.y) { std::swap(v2, v1); }
    if (v1.y > v3.y) { std::swap(v3, v1); }
    if (v2.y > v3.y) { std::swap(v3, v2); }

    if (int(v2.y) == int(v3.y)) { fillBottomFlatTriangle(v1, v2, v3, c); }
    else if (int(v1.y) == int(v2.y)) { fillTopFlatTriangle(v1, v2, v3, c); }
    else
    {
        // general case - split the triangle in a topflat and bottom-flat one
        vect4* v4 = new vect4({
            (v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)),v2.y, 0
            });
        fillBottomFlatTriangle(v1, v2, *v4, c);
        fillTopFlatTriangle(v2, *v4, v3, c);
    }
}

void fillBottomFlatTriangle(vect4 v1, vect4 v2, vect4 v3, vec3 c)
{
    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    // cout <<"a"<< (v2.x - v1.x) / (v2.y - v1.y)<<endl;
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float curx1 = v1.x;
    float curx2 = v1.x;

    SketchLine(v1.x, v1.y, v2.x, v2.y, c);
    SketchLine(v2.x, v2.y, v3.x, v3.y, c);
    SketchLine(v3.x, v3.y, v1.x, v1.y, c);

    for (int scanlineY = v1.y; scanlineY < v2.y - 0.5f; scanlineY++)
    {
        // if (scanlineY == v2.y)
        // {
        //     std::cout << "x" << curx1 << "," << curx2 << std::endl;
        //     std::cout << v2.y << endl;
        // }
        SketchLine(curx1, scanlineY, curx2, scanlineY, c);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void fillTopFlatTriangle(vect4 v1, vect4 v2, vect4 v3, vec3 c)
{
    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    float curx1 = v3.x;
    float curx2 = v3.x;
    SketchLine(v1.x, v1.y, v2.x, v2.y, c);
    SketchLine(v2.x, v2.y, v3.x, v3.y, c);
    SketchLine(v3.x, v3.y, v1.x, v1.y, c);

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
    {
        SketchLine(curx1, scanlineY, curx2, scanlineY, c);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void wireframe_draw(vect4 v1, vect4 v2, vect4 v3, vec3 c)
{
    SketchLine(v1.x, v1.y, v2.x, v2.y, c);
    SketchLine(v2.x, v2.y, v3.x, v3.y, c);
    SketchLine(v3.x, v3.y, v1.x, v1.y, c);
}

void drawWireframe_model(std::vector<Triangle>& model)
{
    for (int i = 0; i < model.size(); i++)
    {
        // wireframe_draw(model[i].vertices[0], model[i].vertices[1], model[i].vertices[2], model[i].color);
        wireframe_draw(model[i].vertices[0], model[i].vertices[1], model[i].vertices[2], WHITE);
    }
}

void draw_model(std::vector<Triangle>& model, bool Shade)
{
    if (!Shade)
    {
        for (int i = 0; i < model.size(); i++)
        {
            fillTriangle(model[i].vertices[0], model[i].vertices[1], model[i].vertices[2], model[i].color);

        }
    }
    else
    {
        for (int i = 0; i < model.size(); i++)
        {
            vect4 vc1 = model[i].vertex_intensity[0];
            vect4 vc2 = model[i].vertex_intensity[2];
            vect4 vc3 = model[i].vertex_intensity[1];
            /*vect4 vc1 = calcIntensity(Ka, Kd, Ks, ns, model[i].vertices[0], light, view, model[i].normals[0], Ia, Il);
            vect4 vc2 = calcIntensity(Ka, Kd, Ks, ns, model[i].vertices[1], light, view, model[i].normals[1], Ia, Il);
            vect4 vc3 = calcIntensity(Ka, Kd, Ks, ns, model[i].vertices[2], light, view, model[i].normals[2], Ia, Il);*/
            Vertex v1(model[i].vertices[0], model[i].normals[0], vc1 * model[i].color);
            Vertex v2(model[i].vertices[1], model[i].normals[1], vc2 * model[i].color);
            Vertex v3(model[i].vertices[2], model[i].normals[2], vc3 * model[i].color);
            DrawTriangle(v1, v2, v3);
        }
    }

}


//void Triangle::Rasterize()
//{
//    // Sort the points so that y0 <= yl <= y2
//    auto P0 = vertex_intensity[0];
//    auto P1 = vertex_intensity[1];
//    auto P2 = vertex_intensity[2];
//
//    if (P1.y < P0.y)
//    {
//        std::swap(P1, P0);
//    }
//    if (P2.y < P0.y)
//    {
//        std::swap(P2, P0);
//    }
//    if (P2.y < P1.y)
//    {
//        std::swap(P2, P1);
//    }
//
//    //  Compute the x coordinates of the triangle edges
//    std::vector<float> x01 = interpolate(P0.y, P0.x, P1.y, P1.x);
//    std::vector<float> h01 = interpolate(P0.y, P0.w, P1.y, P1.w);
//
//    std::vector<float> x12 = interpolate(P1.y, P1.x, P2.y, P2.x);
//    std::vector<float> h12 = interpolate(P1.y, P1.w, P2.y, P2.w);
//
//    std::vector<float> x02 = interpolate(P0.y, P0.x, P2.y, P2.x);
//    std::vector<float> h02 = interpolate(P0.y, P0.w, P2.y, P2.w);
//
//    //  Concatenate the short sides
//    x01.pop_back();
//    std::vector<float> x012;
//    x012.resize(x01.size() + x12.size());
//    std::copy(x01.begin(), x01.end(), x012.begin());
//    std::copy(x12.begin(), x12.end(), x012.begin() + x01.size());
//
//    // std::cout<<"------------------------------------------------\n";
//    h01.pop_back();
//
//    std::vector<float> h012;
//    h012.resize(h01.size() + h12.size());
//    std::copy(h01.begin(), h01.end(), h012.begin());
//    std::copy(h12.begin(), h12.end(), h012.begin() + h01.size());
//
//    //  Determine which is left and which is right
//    int m = x012[x012.size() / 2];
//    std::vector<float> x_left, x_right, h_left, h_right;
//
//    if (x02[m] < x012[m])
//    {
//        x_left = x02;
//        h_left = h02;
//
//        x_right = x012;
//        h_right = h012;
//    }
//    else
//    {
//        x_left = x012;
//        h_left = h012;
//
//        x_right = x02;
//        h_right = h02;
//    }
//    // printVec(x_left);
//    // std::cout<<"------------------------------------------------\n";
//    // printVec(x_right);
//
//    // Draw the horizontal segments
//    for (float y = P0.y; y < P2.y - 1; y++)
//    {
//        auto x_l = x_left[y - P0.y];
//        auto x_r = x_right[y - P0.y];
//
//        auto h_segment = interpolate(x_l, h_left[y - P0.y], x_r, h_right[y - P0.y]);
//
//        for (float x = x_l; x < x_r; x++)
//        {
//            vec3 shadedColor = color * h_segment[x - x_l];
//            // maths::printvec(shadedColor);
//            putpixel(x, y, shadedColor);
//        }
//    }
//}
//
//void Triangle::newRasterize()
//{
//
//    auto a = vertex_intensity[0];
//    auto b = vertex_intensity[1];
//    auto c = vertex_intensity[2];
//    // vertex arr[3] = { A,B,C };
//    if (a.x > b.y)
//    {
//        std::swap(b, a);
//    }
//    if (a.x > c.y)
//    {
//        std::swap(c, a);
//    }
//    if (b.y > c.y)
//    {
//        std::swap(c, b);
//    }
//
//    float dx1, di1, dr1, dg1, db1, ddp1, ddp2, ddp3, dx2, dr2, dg2, db2, dx3, dr3, dg3, db3, dr, dg, db, ddp;
//    if (b.y - a.y > 0)
//    {
//        dx1 = (b.x - a.x) / (b.y - a.y);
//        ddp1 = (b.w - a.w) / (b.y - a.y);
//    }
//    else
//        dx1 = dr1 = dg1 = db1 = 0, ddp1 = 0;
//    ;
//
//    if (c.y - a.y > 0)
//    {
//        dx2 = (c.x - a.x) / (c.y - a.y);
//        ddp2 = (c.w - a.w) / (c.y - a.y);
//    }
//    else
//        dx2 = dr2 = dg2 = db2 = ddp2 = 0;
//
//    if (c.y - b.y > 0)
//    {
//        dx3 = (c.x - c.x) / (c.y - b.y);
//        ddp3 = (c.w - b.w) / (c.y - b.y);
//    }
//    else
//        dx3 = dr3 = dg3 = db3 = ddp3 = 0;
//    vect4 S, E, P;
//    S = a;
//    E = a;
//    if (dx1 > dx2)
//    {
//        for (; S.y <= b.y; S.y++, E.y++)
//        {
//            if (E.x - S.x > 0)
//            {
//                ddp = (E.w - S.w) / (E.x - S.x);
//            }
//            else
//                dr = dg = db = ddp = 0;
//            P = S;
//            for (; P.x < E.x; P.x++)
//            {
//                //MULTIPLY
//                vec3 ncolor = color * P.w;
//                putpixel(P.x, P.y, ncolor);
//                P.w += ddp;
//            }
//            //MULTIPLY ??
//            S.x += dx2;
//            S.w += ddp2;
//            E.x += dx1;
//            E.w += ddp1;
//        }
//
//        E = b;
//        for (; S.y <= c.y; S.y++, E.y++)
//        {
//            if (E.x - S.x > 0)
//            {
//                //MULTIPLY
//                vec3 ncolor = color * P.w;
//                ddp = (E.w - S.w) / (E.x - S.x);
//            }
//            else
//                dr = dg = db = 0;
//            P = S;
//            for (; P.x <= E.x; P.x++)
//            {
//                //multiply
//                vec3 ncolor = color * P.w;
//                putpixel(P.x, P.y, ncolor);
//                P.w += ddp;
//            }
//            S.x += dx2;
//            S.w += ddp2;
//            E.x += dx3;
//            E.w += ddp3;
//        }
//    }
//    else
//    {
//        for (; S.y <= b.y; S.y++, E.y++)
//        {
//            if (E.x - S.x > 0)
//            {
//                //MULTIPLY
//                vec3 ncolor = color * P.w;
//                ddp = (E.w - S.w) / (E.x - S.x);
//            }
//            else
//                dr = dg = db = ddp = 0;
//
//            P = S;
//            for (; P.x < E.x; P.x++)
//            {
//                vec3 ncolor = color * P.w;
//                //multiply
//                putpixel(P.x, P.y, ncolor); //doubtful-----------------------------------------------------------------------
//                P.w += ddp;
//            }
//            S.x += dx1;
//            S.w += ddp1;
//            E.x += dx2;
//            E.w += ddp2;
//        }
//
//        S = b;
//        for (; S.y <= c.y; S.y++, E.y++)
//        {
//            if (E.x - S.x > 0)
//            {
//                //MULTIPLY
//                ddp = (E.w - S.w) / (E.x - S.x);
//            }
//            else
//                dr = dg = db = ddp = 0;
//
//            P = S;
//            for (; P.x < E.x; P.x++)
//            {
//                vec3 ncolor = color * P.w;
//                putpixel(P.x, P.y, ncolor);
//                //MULTIPLY
//                P.w += ddp;
//            }
//            S.x += dx3;
//            S.w += ddp3;
//            E.x += dx2;
//            E.w += ddp2;
//        }
//    }
//}