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

class Terrain {
private:
	CustomModel terrainModel = CustomModel("./model/terrain/terrain.obj");
public:
	Terrain();
	
	void render(glm::mat4 camera, glm::vec4 vector);
};
