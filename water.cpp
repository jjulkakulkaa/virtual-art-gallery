#include "water.h"

Water::Water() {
	vertices = {
	-1.0f, 0.0f, -1.0f, 1.0f,
	 1.0f, 0.0f, -1.0f, 1.0f,
	 1.0f, 0.0f,  1.0f, 1.0f,

	 1.0f, 0.0f,  1.0f, 1.0f,
	-1.0f, 0.0f,  1.0f, 1.0f,
	-1.0f, 0.0f, -1.0f, 1.0f
	};

	texCoords = {
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	vertexCount = 6;
}


void Water::render(glm::mat4 V, waterFrameBuffers fbos) {
	spWater->use();
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 0.01f, 5000.0f);
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(100.0f, 100.0f, 100.0f));
	glUniformMatrix4fv(spWater->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spWater->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spWater->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spWater->a("vertex"));
	glVertexAttribPointer(spWater->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbos.getReflectionTexture());
	glUniform1i(spWater->u("reflectionTexture"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbos.getRefractionTexture());
	glUniform1i(spWater->u("refractionTexture"), 1);



	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(spWater->a("vertex"));
	glDisableVertexAttribArray(spWater->a("textureCoords"));
}