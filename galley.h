#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>

#include "camera.h"
#include "constants.h"
#include "oarAnimation.h"
#include "customModel.h"


using namespace std;

class Galley {
private:
	CustomModel oar = CustomModel("./model/oar.obj");
	CustomModel galley = CustomModel("./model/galley.obj");
	glm::mat4 galleyMatrix;

public:
	Galley();
	
	void render(float leftOarsAngle, float rightOarsAngle, Camera camera, glm::vec4 vector, int direction);
	void move(int direction, Camera camera);

};