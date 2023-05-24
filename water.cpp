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

	vertexCount = 6;

	dudvTexture = readTexture("./dudv_map.png");
	normalTexture = readTexture("./normal_map.png");
}


void Water::render(Camera camera, waterFrameBuffers fbos, Light light) {
	spWater->use();
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 0.01f, 5000.0f);
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(25.0f, -0.2f, -25.0f));
	M = glm::scale(M, glm::vec3(100.0f, 100.0f, 100.0f));
	glUniformMatrix4fv(spWater->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spWater->u("V"), 1, false, glm::value_ptr(camera.getPosistion()));
	glUniformMatrix4fv(spWater->u("M"), 1, false, glm::value_ptr(M));

	vector<float> cameraPosition = { camera.getWidth(), camera.getHeight(), camera.getDepth()};
	vector<float> lightPosition = light.getPosition();
	lightPosition = { lightPosition[0], lightPosition[1], lightPosition[2] };
	vector<float> lightColor = light.getColor();
	lightColor = { lightColor[0], lightColor[1], lightColor[2] };

	glUniform3fv(spWater->u("cameraPosition"), 1, cameraPosition.data());
	glUniform3fv(spWater->u("lightPosition"), 1, lightPosition.data());
	glUniform3fv(spWater->u("lightColor"), 1, lightColor.data());

	glEnableVertexAttribArray(spWater->a("vertex"));
	glVertexAttribPointer(spWater->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbos.getReflectionTexture());
	glUniform1i(spWater->u("tex"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbos.getRefractionTexture());
	glUniform1i(spWater->u("tex2"), 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);
	glUniform1i(spWater->u("dudvMap"), 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalTexture);
	glUniform1i(spWater->u("normalMap"), 3);

	glUniform1f(spWater->u("moveFactor"), moveFactor);


	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(spWater->a("vertex"));
}

void Water::updateMoveFactor() {
	moveFactor += waveSpeed;
	moveFactor = fmod(moveFactor, 1);
}