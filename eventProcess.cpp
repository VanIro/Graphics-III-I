#include "eventProcess.h"
#include "models_define.h"
#include "KEEYS.h" //already through models_define.h
extern float deltaTime;
extern vec2i MousePos;

void mouseMotionProcess(int x, int y){
    MousePos.x = x;
    MousePos.y = y;
    camera->mouseMoveEvent(x, y);
    LLight->mouseMoveEvent(x, y);
}

void mouseEvent(int button, int state, int x, int y) {

}



void specialKeyboard(int key, int x, int y)
{
    //std::cout << "Key Pressed" << std::endl;

    switch (key)
    {
    case GLUT_KEY_UP:
        camera->processKeyboard(Camera_Movement::UP, deltaTime);
        break;
    case GLUT_KEY_DOWN:
        camera->processKeyboard(Camera_Movement::DOWN, deltaTime);
        break;
    case GLUT_KEY_LEFT:
        camera->processKeyboard(Camera_Movement::LEFT, deltaTime);
        break;
    case GLUT_KEY_RIGHT:
        camera->processKeyboard(Camera_Movement::RIGHT, deltaTime);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    //std::cout << "Key Pressed" << std::endl;

    switch (key)
    {
    case 'w':
    case 'W':
        camera->processKeyboard(Camera_Movement::WKEY, deltaTime);
        break;
    case 's':
    case 'S':
        camera->processKeyboard(Camera_Movement::SKEY, deltaTime);
        break;
    case 'a':
    case 'A':
        camera->processKeyboard(Camera_Movement::AKEY, deltaTime);
        break;
    case 'd':
    case 'D':
        camera->processKeyboard(Camera_Movement::DKEY, deltaTime);
        break;
    case 'v':
    case 'V':
        LLight->turn_flags_off();
        camera->processKeyboard(Camera_Movement::VKEY, deltaTime);
        break;
    case 'f':
    case 'F':
        camera->processKeyboard(Camera_Movement::FORWARD, deltaTime);
        break;
    case 'b':
    case 'B':
        camera->processKeyboard(Camera_Movement::BACK, deltaTime);
        break;
    case 'l':
    case 'L':
        LLight->processKeyboard(KEEYS::LKEY);
        camera->turn_flags_off();
        break;
    case 'z':
    case 'Z':
        LLight->processKeyboard(KEEYS::ZKEY);
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}