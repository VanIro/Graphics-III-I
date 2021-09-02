#pragma once

#include "model.h"
#include "Camera.h"
#include "Light.h"

Camera* camera = new Camera();
//Model* model_zaha = new Model;
Model* model_base = new Model;
Model* model_blocks = new Model;
Model* model_glass = new Model;
Model* model_cloud = new Model;
int NumModels = 4;
//Model* models[3] = { model_base, model_blocks, model_glass };
Model* models[4] = { model_base, model_blocks, model_glass, model_cloud };
Light* LLight = new Light();
