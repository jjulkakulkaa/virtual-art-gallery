#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "myCube.h"
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"

namespace oam {
	glm::mat4 rotateRightOars(float angle, glm::mat4 modelMatrix) {

		angle = fmod(angle, PI);

		if (angle > 0 && angle <= PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (angle > PI / 6 && angle <= 2 * PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, angle - (PI / 6), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (angle > 2 * PI / 6 && angle <= 4 * PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, -(angle - (2 * PI / 6)), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (angle > 4 * PI / 6 && angle <= 5 * PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, -PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, PI / 6 - (angle - (4 * PI / 6)), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (angle > 5 * PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, -PI / 6 + (angle - (5 * PI / 6)), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		return modelMatrix;
	}

	glm::mat4 rotateLeftOars(float angle, glm::mat4 modelMatrix) {

		angle = fmod(angle, PI);

		if (angle > 0 && angle <= PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (angle > PI / 6 && angle <= 2 * PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, -PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, angle - (PI / 6), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (angle > 2 * PI / 6 && angle <= 4 * PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, -PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, (angle - (2 * PI / 6)), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (angle > 4 * PI / 6 && angle <= 5 * PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, PI / 6 - (angle - (4 * PI / 6)), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (angle > 5 * PI / 6) {
			modelMatrix = glm::rotate(modelMatrix, PI / 6 - (angle - (5 * PI / 6)), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		return modelMatrix;
	}
}