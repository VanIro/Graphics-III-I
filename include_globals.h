#pragma once

#include"vect.h"

//extern const unsigned int WIDTH;
//extern const unsigned int HEIGHT;

extern const unsigned int WIDTH ;
extern const unsigned int HEIGHT ;
extern const unsigned int ZHEIGHT;
extern float z_buffer[][800];

extern float deltaTime;

extern float lightX;
extern float lightY;
extern float lightZ;

extern vect4 view;
extern vect4 Ka;
extern vect4 Kd;
extern vect4 Ks;
extern vect4 light;
//vect4 light = vect4{ lightX, lightY, lightZ };
extern float ns;
extern vect4 Ia;
extern vect4 Il;


