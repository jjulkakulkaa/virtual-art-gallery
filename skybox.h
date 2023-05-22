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

class SkyBox {
private:
	CustomModel skybox = CustomModel("model/anime-starry-night/source/skybox.obj");

public:
	SkyBox();

	void render(glm::mat4 cameraMatrix, glm::vec4 vector);
};