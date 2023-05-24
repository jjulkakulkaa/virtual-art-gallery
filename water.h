#pragma once

#include "shaderprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "constants.h"
#include "waterFrameBuffers.h"
#include <vector>
#include <iostream>
#include "customModel.h"
#include "camera.h"
#include "light.h"

class Water {
private:
	std::vector<float> vertices;
	std::vector<float> texCoords;
	int vertexCount;
	GLuint dudvTexture;
	GLuint normalTexture;
	float moveFactor = 0;
	float waveSpeed = 0.0005f;
public:
	Water();

	void render(Camera camera, waterFrameBuffers fbos, Light light);
	void updateMoveFactor();
};
