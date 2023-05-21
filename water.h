#pragma once

#include "shaderprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

float vertices[] = {
	-1.0f, 0.0f, -1.0f, 1.0f,
	 1.0f, 0.0f, -1.0f, 1.0f,
	 1.0f, 0.0f,  1.0f, 1.0f,

	 1.0f, 0.0f,  1.0f, 1.0f,
	-1.0f, 0.0f,  1.0f, 1.0f,
	-1.0f, 0.0f, -1.0f, 1.0f


};
int vertexCount = 6;
class Water {
public:

	void draw(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
		spWater->use();
		glUniformMatrix4fv(spWater->u("P"), 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(spWater->u("V"), 1, false, glm::value_ptr(V));
		glUniformMatrix4fv(spWater->u("M"), 1, false, glm::value_ptr(M));

		glEnableVertexAttribArray(spWater->a("vertex"));
		glVertexAttribPointer(spWater->a("vertex"), 4, GL_FLOAT, false, 0, vertices);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glDisableVertexAttribArray(spWater->a("vertex"));
	 }
};
