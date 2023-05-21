#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"

namespace oam {
	glm::mat4 rotateRightOars(float angle, glm::mat4 modelMatrix);
	glm::mat4 rotateLeftOars(float angle, glm::mat4 modelMatrix);
}
