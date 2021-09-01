#pragma once

#include "model.h"
#include "Camera.h"
#include "Light.h"

Camera* camera = new Camera();
//Model* model_zaha = new Model;
Model* model_base = new Model;
Model* model_blocks = new Model;
Model* model_glass = new Model;
int NumModels = 3;
Model* models[3] = { model_base, model_blocks, model_glass };
Light* LLight = new Light();
