#include "skybox.h"

SkyBox::SkyBox() {
	skybox.loadModel();
}

void SkyBox::render(glm::mat4 cameraMatrix, glm::vec4 vector) {
	glm::mat4 baseMatrix = glm::mat4(1.0f);
	baseMatrix = glm::rotate(baseMatrix, PI, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 0.01f, 5000.0f);

	skybox.draw(P, cameraMatrix, baseMatrix, vector);
}