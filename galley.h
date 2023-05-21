#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>

#include "constants.h"
#include "oarAnimation.h"
#include "customModel.h"


using namespace std;

class Galley {
private:
	CustomModel oar = CustomModel("./model/oar.obj");
	CustomModel galley = CustomModel("./model/galley.obj");

public:
	Galley();
	
	void render(float leftOarsAngle, float rightOarsAngle, glm::mat4 camera);
};