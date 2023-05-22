#pragma once

#include "shaderprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "constants.h"
#include "waterFrameBuffers.h"
#include <vector>
#include <iostream>

class Water {
private:
	std::vector<float> vertices;
	std::vector<float> texCoords;
	int vertexCount;
public:
	Water();

	void render(glm::mat4 V, waterFrameBuffers fbos);
};
