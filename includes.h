
#include "model.h"
//Declaration of useful variables or functions
float lastX = 800;
float lastY = 800;
bool mouseLeftDown = false;
const unsigned int SWIDTH = 800;
const unsigned int SHEIGHT = 800;

//Camera* camera;
//Model* model;


//float deltaTime = 0.0f;

void drawModel();

//void processKeys(unsigned char key, float x, float y);
void processMouse(int xpos, int ypos);
void updateFunction(int val);

void myinit(int argc, char** argv);