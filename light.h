#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Light {
private:
	std::vector<float> light_position;
	std::vector<float> light_ambient;
	std::vector<float> light_diffuse;
	std::vector<float> light_specular;

public:

	Light(std::vector<float> color, std::vector<float> position);

	void init();
	std::vector<float> getPosition();
	std::vector<float> getColor();
};
