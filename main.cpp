
#include "includes.h"
#include "include_globals.h"
#include "models_define.h"
#include "eventProcess.h"
#include"transform.h"
extern float ZNEAR, ZFAR;
//extern enum Camera_Movement;

//extern int testTest;
int main(int argc, char** argv)
{
    //std::cout << atan2f(0, 1); return 0;
    myinit(argc, argv);

    //camera = new Camera();
    //Model* model = model_zaha;// new Model;

    model_base->Load("obj/Ibase.obj");//model_base->Load("obj/baseOnly.obj");
    model_base->Load("obj/Istructure.obj");//model_blocks->Load("obj/blocksOnly.obj");
    model_base->Load("obj/Iglass.obj");//model_glass->Load("obj/glassOnly2.obj");
    
    //model_zaha->Load("obj/colored1.obj");
    //model_zaha->Load("obj/smooth_zaha.obj");
    //model_zaha->Load("obj/building - Copy.obj");

    //model_zaha->camera = camera;

    for (int i = 0; i < NumModels; i++) {
        models[i]->scale_model(10);
        //models[i]->scale_model(1.5);
        //models[i]->scale_model(2.5,'Z');
        //models[i]->scale_model(1.5, 'Y');
        //models[i]->rotate_model('Z', 335);
        //models[i]->rotate_model('Y', 100);
        models[i]->rotate_model('Z', 180);
        models[i]->translate_model({-50,30,150,0});
    }

    /*std::cout << "\nmmm" << vect4::a << " --> " << vect4::X;
    std::cout << "\n" << vect4::b << " --> " << vect4::Y;
    std::cout << "\n" << vect4::c << " --> " << vect4::Z;*/


    glutDisplayFunc(drawModel);

    glutSpecialFunc(specialKeyboard);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseEvent);
    glutPassiveMotionFunc(mouseMotionProcess);
    int fps = 60;
    glutTimerFunc(1000 / fps, updateFunction, 0);
    // glutMotionFunc(processMouse);

    // glutMouseFunc(mouseCB);
    // glutMotionFunc(mouseMotionCB);

    glutMainLoop();
}


void myinit(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Heydar Aliyev");

    glClearColor(0.39, 0.6, 0.69, 0.0);
    glViewport(0, 0, SWIDTH, SHEIGHT);
    glLoadIdentity();
    //gluOrtho2D(0, SWIDTH, 0, SHEIGHT);
    glOrtho(0, WIDTH, 0, HEIGHT, -200, 200);
}

void drawModel()
{
    //clear z Buffer
    int z_buffer_clear = -2000;
    //int z_buffer_clear = 2000;
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
            z_buffer[i][j] = z_buffer_clear;

    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame) / 10;

    lastFrame = currentFrame;

    glClear(GL_COLOR_BUFFER_BIT);
    mat4f view = camera->getViewMatrix();
    mat4f projection;
    camera->getProjectionMatrix(projection);
    //projection = Perspective(90.0f, (float)SHEIGHT / SWIDTH);
    //display_transform(projection.matrix4);
    
    //model_zaha->transformModel(view, projection);
    //model_zaha->draw();

    for (int i = 0; i < NumModels; i++) {
        models[i]->transformModel(view, projection);
        models[i]->draw();
    }

    LLight->transform(view, projection);
    LLight->draw();

    std::cout << "\nFPS...  "<<100/deltaTime<<"\n";

    glutSwapBuffers();
    glFlush();

    //updateFunction(0);

}

void updateFunction(int val)
{
    int fps = 60;
    glutTimerFunc(1000 / fps, updateFunction, 0);
    //model_zaha->rotate_model('X', 5);
    //model_zaha->rotate_model('Y', 10);
    //model_zaha->rotate_model('X', -10);
    glutPostRedisplay();
}


